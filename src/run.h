#ifndef RUN_H
#define RUN_H

#include <unistd.h>

struct Result
{
    long ru_utime;    /* user CPU time used */
    long ru_stime;    /* system CPU time used */
    long ru_maxrss;   /* maximum resident set size */
    long ru_ixrss;    /* integral shared memory size */
    long ru_idrss;    /* integral unshared data size */
    long ru_isrss;    /* integral unshared stack size */
    long ru_minflt;   /* page reclaims (soft page faults) */
    long ru_majflt;   /* page faults (hard page faults) */
    long ru_nswap;    /* swaps */
    long ru_inblock;  /* block input operations */
    long ru_oublock;  /* block output operations */
    long ru_msgsnd;   /* IPC messages sent */
    long ru_msgrcv;   /* IPC messages received */
    long ru_nsignals; /* signals received */
    long ru_nvcsw;    /* voluntary context switches */
    long ru_nivcsw;   /* involuntary context switches */

    long time_used;
    long memory_used;
    int status; /* 程序退出的返回值 */
    int error;  /* 记录是否错误退出 */
    char *reason;
};

struct Result runit(pid_t);

#endif