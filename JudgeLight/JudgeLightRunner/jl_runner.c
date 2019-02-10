#include "jl_runner.h"
#include "jl_limit.h"
#include "jl_memory.h"

#include <Python.h>
#include <sys/ptrace.h>
#include <sys/time.h>
#include <sys/types.h>
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
        // if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) != 0) {
        //     ERROR("ptrace TRACEME failure!")
        // }

        /** 开始执行待评测程序 */
        execve(rconfig->exec_file_path, rconfig->exec_args, rconfig->envs);

        ERROR("execve failure!");
    } else {  // parent

        /** TODO: 监控子进程系统调用，读取内存与时间占用，处理运行中的异常 */
        sleep(1);

        gettimeofday(&end, NULL);
        rstats->real_time_used =
            (int)(end.tv_sec * 1000 + end.tv_usec / 1000 - start.tv_sec * 1000 -
                  start.tv_usec / 1000);
    }

END:
    return res;
}