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
int SetProcessLimit(struct RunnerConfig *rconfig) {
    int res = 0;
    struct rlimit rl;

    /** 设置 CPU 时间限制 */
    if (rconfig->time_limit != UNLIMITED) {
        rl.rlim_cur = rl.rlim_max = (rlim_t)(rconfig->time_limit / 1000 + 1);
        if (rconfig->time_limit % 1000 > 800) {
            rl.rlim_cur += 1;
        }
        if (setrlimit(RLIMIT_CPU, &rl) != 0) {
            ERROR("setrlimit RLIMIT_CPU failure!");
        }
    }

    /** 设置实际时间限制 */
    if (rconfig->real_time_limit != UNLIMITED) {
        struct itimerval rt;
        long t = rconfig->real_time_limit;
        rt.it_interval.tv_sec = 0;
        rt.it_interval.tv_usec = 0;
        rt.it_value.tv_sec = t / 1000;
        rt.it_value.tv_usec = (int)(t % 1000) * 1000;
        if (setitimer(ITIMER_REAL, &rt, (struct itimerval *)0) != 0) {
            ERROR("setitimer ITIMER_REAL failure!");
        }
    }

    /** 设置内存限制 */
    if (rconfig->memory_limit != UNLIMITED) {
        rl.rlim_cur = rl.rlim_max = (rlim_t)rconfig->memory_limit * 1024;
        if (setrlimit(RLIMIT_DATA, &rl) != 0) {
            ERROR("setrlimit RLIMIT_DATA failure!");
        }
        rl.rlim_cur = rl.rlim_max = (rlim_t)rconfig->memory_limit * 1024 * 2;
        if (setrlimit(RLIMIT_AS, &rl) != 0) {
            ERROR("setrlimit RLIMIT_AS failure!");
        }
    }

    /** 设置栈限制 */
    if (rconfig->stack_limit != UNLIMITED) {
        rl.rlim_cur = rl.rlim_max = (rlim_t)rconfig->stack_limit;
        if (setrlimit(RLIMIT_STACK, &rl) != 0) {
            ERROR("setrlimit RLIMIT_STACK failure!");
        }
    }

    /** 设置最大输出文件大小 */
    if (rconfig->output_size_limit != UNLIMITED) {
        rl.rlim_cur = rl.rlim_max = (rlim_t)rconfig->output_size_limit;
        if (setrlimit(RLIMIT_FSIZE, &rl) != 0) {
            ERROR("setrlimit RLIMIT_FSIZE failure!");
        }
    }

    /** 设置 gid */
    if (rconfig->gid != UNLIMITED) {
        if (setgid(rconfig->gid) == -1) {
            ERROR("setgid failure!");
        }
    }

    /** 设置 uid */
    if (rconfig->uid != UNLIMITED) {
        if (setuid(rconfig->uid) == -1) {
            ERROR("setuid failure!");
        }
    }

END:
    return res;
}

/**
 * 为当前进程设置流重定向
 * */
int SetStream(struct RunnerConfig *rconfig) {
    int res = 0;
    /** 设置输入流 */
    if (rconfig->input_file_path != NULL) {
        FILE *in_file;
        if ((in_file = fopen(rconfig->input_file_path, "r")) == NULL) {
            ERROR("fopen in_file failure!");
        }
        if (dup2(fileno(in_file), STDIN_FILENO) == -1) {
            ERROR("dup2 in_file failure!");
        }
    }

    /** 设置输出流 */
    if (rconfig->output_file_path != NULL) {
        FILE *out_file;
        if ((out_file = fopen(rconfig->output_file_path, "w")) == NULL) {
            ERROR("fopen out_file failure!");
        }
        if (dup2(fileno(out_file), STDOUT_FILENO) == -1) {
            ERROR("dup2 out_file failure!");
        }
    }

    /** 设置 error 流 */
    if (rconfig->error_file_path != NULL) {
        FILE *err_file;
        if ((err_file = fopen(rconfig->error_file_path, "w")) == NULL) {
            ERROR("fopen err_file failure!");
        }
        if (dup2(fileno(err_file), STDERR_FILENO) == -1) {
            ERROR("dup2 err_file failure!");
        }
    }

END:
    return res;
}
