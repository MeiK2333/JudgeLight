#include "jl_runner.h"
#include "jl_limit.h"
#include "jl_memory.h"
#include "jl_rules.h"

#include <Python.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * 运行的主流程
 * */
int RunIt(struct RunnerConfig *rconfig, struct RunnerStats *rstats) {
#define ERROR(msg)                                                             \
    {                                                                          \
        if (PyErr_Occurred() == NULL) PyErr_SetString(PyExc_SystemError, msg); \
        res = -1;                                                              \
        goto END;                                                              \
    }

    int res = 0;
    pid_t pid;
    struct timeval start, end;

    gettimeofday(&start, NULL);

    if ((pid = fork()) < 0) {
        ERROR("fork failure!");
    } else if (pid == 0) {  // child

        /** 设置资源限制 */
        if (SetProcessLimit(rconfig) != 0) {
            ERROR("SetProcessLimit failure!");
        }

        /** 设置流重定向 */
        if (SetStream(rconfig) != 0) {
            ERROR("SetStream failure!");
        }

        /** 开启 ptrace 监控系统调用，在每次调用暂停时读取内存与时间占用 */
        if (rconfig->trace && ptrace(PTRACE_TRACEME, 0, NULL, NULL) != 0) {
            ERROR("ptrace_TRACEME failure!");
        }

        /** 开始执行待评测程序 */
        execve(rconfig->exec_file_path, rconfig->exec_args, rconfig->envs);

        ERROR("execve failure!");
    } else {  // parent

        /** 监控子进程系统调用，读取内存与时间占用，处理运行中的异常 */
        memset(rstats, -1, sizeof(struct RunnerStats));

        rstats->re_flag = 0;
        int status, incall = 0;
        struct rusage ru;
        struct user_regs_struct regs;
        /** 打开文件描述符，以供后续读取文件内容 */
        FILE *fr;
        char status_file[PATH_MAX];
        sprintf(status_file, "/proc/%d/status", pid);
        if ((fr = fopen(status_file, "r")) == NULL) {
            ERROR("fopen failure!");
        }

        struct MemoryStatus ms;

        if (rconfig->trace) {
            /** ptrace 监控子进程的系统调用 */
            while (1) {
                if (wait4(pid, &status, WSTOPPED, &ru) == -1) {
                    ERROR("wait4 [WSTOPPED] failure!");
                }

                /** 如果子进程已经停止，则跳出 */
                if (WIFEXITED(status)) {
                    break;
                }

                /** 非内核产生的调停，代表进程因自身异常停止运行 */
                if (WSTOPSIG(status) != SIGTRAP) {
                    /** 结束 ptrace，waitpid 移除僵尸进程 */
                    ptrace(PTRACE_KILL, pid, NULL, NULL);
                    waitpid(pid, NULL, 0);
                    rstats->re_flag = 1;
                    goto JUDGE_END;
                }

                /** 复制跟踪器信息 */
                if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1) {
                    ERROR("PTRACE_GETREGS failure!");
                }

                if (incall) {
                    /** 检查系统调用是否被允许 */
                    if (CheckSyscallRule(rconfig, &regs) != 0) {
                        /**
                         * 如果 open 和 openat 被禁止，则对于 open 和 openat
                         * 单独处理，不允许创建或写入文件
                         * */
                        if (((REG_SYS_CALL(&regs) == SYS_openat) &&
                             ((SYS_CALL_ARG_3(&regs) &
                               (O_CREAT | O_WRONLY | O_RDWR | O_APPEND)) ==
                              0)) ||
                            ((REG_SYS_CALL(&regs) == SYS_open) &&
                             ((SYS_CALL_ARG_2(&regs) &
                               (O_CREAT | O_WRONLY | O_RDWR | O_APPEND)) ==
                              0))) {
                            /** 如果是仅读取的话则允许执行 */
                        } else {
                            ptrace(PTRACE_KILL, pid, NULL, NULL);
                            waitpid(pid, NULL, 0);
                            rstats->re_flag = 1;
                            rstats->re_syscall = REG_SYS_CALL(&regs);
                            goto JUDGE_END;
                        }
                    }
                    incall = 0;
                } else {
                    incall = 1;
                }

                /** 读取内存 */
                if (MemoryUsage(fileno(fr), &ms) != 0) {
                    ERROR("GetMemoryUsage failure!");
                }
                rstats->memory_used = rstats->memory_used > ms.vm_rss
                                          ? rstats->memory_used
                                          : ms.vm_rss;

                /** 重新启动跟踪 */
                ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
            }
        } else {
            /** 不使用 ptrace */
            if (wait4(pid, &status, 0, &ru) == -1) {
                ERROR("wait4 failure");
            }
            if (WIFSIGNALED(status)) {
                rstats->re_flag = 1;
            }
        }

    JUDGE_END:

        fclose(fr);
        /** 读取 CPU 时间 */
        rstats->time_used =
            ru.ru_utime.tv_sec * 1000 + ru.ru_utime.tv_usec / 1000 +
            ru.ru_stime.tv_sec * 1000 + ru.ru_stime.tv_usec / 1000;

        /** 如果 ptrace 中没有获得内存占用 */
        if (rstats->memory_used == -1) {
            rstats->memory_used = ru.ru_maxrss;
        }

        /** 获取结束信号 */
        rstats->signum = WSTOPSIG(status);

        /** 读取实际运行时间 */
        gettimeofday(&end, NULL);
        rstats->real_time_used =
            (int)(end.tv_sec * 1000 + end.tv_usec / 1000 - start.tv_sec * 1000 -
                  start.tv_usec / 1000);
    }

END:
    return res;
}