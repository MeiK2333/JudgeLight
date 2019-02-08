#include "jl_runner.h"
#include "jl_limit.h"
#include "jl_memory.h"

#include <Python.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <unistd.h>

/** 运行的主流程 */
int RunIt(struct RunnerConfig *rconfig, struct RunnerStats *rstats) {
#define ERROR(msg)                                                             \
    {                                                                          \
        if (PyErr_Occurred() == NULL) PyErr_SetString(PyExc_SystemError, msg); \
        res = -1;                                                              \
        goto END;                                                              \
    }

    int res = 0;
    pid_t pid;

    if ((pid = fork()) < 0) {
        ERROR("fork failure!");
    } else if (pid == 0) {  // child

        /** 设置资源限制 */
        if (SetProcessLimit(rconfig) != 0) {
            ERROR("SetProcessLimit failure!");
        }

        /** TODO: 设置流重定向 */

        /** 开启 ptrace 监控系统调用，在每次调用暂停时读取内存与时间占用 */
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) != 0) {
            ERROR("ptrace TRACEME failure!")
        }

        /** TODO: 设置系统调用限制 */
    } else {  // parent

        /** TODO: 监控子进程系统调用，读取内存与时间占用，处理运行中的异常 */
    }

END:
    return res;
}