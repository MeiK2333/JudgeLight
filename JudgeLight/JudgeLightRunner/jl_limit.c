#include "jl_limit.h"

#include <Python.h>
#include <sys/resource.h>

#define ERROR(msg)                                                             \
    {                                                                          \
        if (PyErr_Occurred() == NULL) PyErr_SetString(PyExc_SystemError, msg); \
        res = -1;                                                              \
        goto END;                                                              \
    }

/**
 * 为当前进程设置资源限制
 * */
int SetProcessLimit(struct RunnerConfig* rconfig) {
    int res = 0;
    struct rlimit rl;

    /** 设置 CPU 时间限制 */
    if (rconfig->time_limit) {
        rl.rlim_cur = rl.rlim_max = (rlim_t)rconfig->time_limit;
        if (setrlimit(RLIMIT_CPU, &rl) != 0) {
            ERROR("setrlimit RLIMIT_CPU failure!");
        }
    }

    /** 设置实际时间限制 */
    if (rconfig->real_time_limit) {
        struct itimerval rt;
        long t = rconfig->real_time_limit;
        rt.it_interval.tv_sec = 0;
        rt.it_interval.tv_usec = 0;
        rt.it_value.tv_sec = t / 1000;
        rt.it_value.tv_usec = (int)(t % 1000) * 1000;
        if (setitimer(ITIMER_REAL, &rt, (struct itimerval*)0) != 0) {
            ERROR("setitimer ITIMER_REAL failure!");
        }
    }

    /** 设置内存限制 */
    if (rconfig->memory_limit) {
        rl.rlim_cur = rl.rlim_max = (rlim_t)rconfig->memory_limit;
        if (setrlimit(RLIMIT_AS, &rl) != 0) {
            ERROR("setrlimit RLIMIT_AS failure!");
        }
    }

    /** 设置栈限制 */
    if (rconfig->stack_limit) {
        rl.rlim_cur = rl.rlim_max = (rlim_t)rconfig->stack_limit;
        if (setrlimit(RLIMIT_STACK, &rl) != 0) {
            ERROR("setrlimit RLIMIT_STACK failure!");
        }
    }

    /** 设置最大输出文件大小 */
    if (rconfig->output_size_limit) {
        rl.rlim_cur = rl.rlim_max = (rlim_t)rconfig->output_size_limit;
        if (setrlimit(RLIMIT_FSIZE, &rl) != 0) {
            ERROR("setrlimit RLIMIT_FSIZE failure!");
        }
    }

    /** 设置 gid */
    if (rconfig->gid) {
        if (setgid(rconfig->gid) == -1) {
            ERROR("setgid failure!");
        }
    }

    /** 设置 uid */
    if (rconfig->uid) {
        if (setuid(rconfig->uid) == -1) {
            ERROR("setuid failure!");
        }
    }

END:
    return res;
}

int SetStream(struct RunnerConfig* rconfig) {
    // TODO
    return 0;
}
