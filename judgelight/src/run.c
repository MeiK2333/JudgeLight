#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <fcntl.h>
#include "run.h"

struct Result runit(pid_t pid)
{
    int status;
    struct rusage ru;
    wait4(pid, &status, 0, &ru);

    struct Result result;
    result.ru_utime = ru.ru_utime.tv_sec * 1000 + ru.ru_utime.tv_usec / 1000;
    result.ru_stime = ru.ru_stime.tv_sec * 1000 + ru.ru_stime.tv_usec / 1000;
    result.ru_maxrss = ru.ru_maxrss;
    result.ru_ixrss = ru.ru_ixrss;
    result.ru_idrss = ru.ru_idrss;
    result.ru_isrss = ru.ru_isrss;
    result.ru_minflt = ru.ru_minflt;
    result.ru_majflt = ru.ru_majflt;
    result.ru_nswap = ru.ru_nswap;
    result.ru_inblock = ru.ru_inblock;
    result.ru_oublock = ru.ru_oublock;
    result.ru_msgsnd = ru.ru_msgsnd;
    result.ru_msgrcv = ru.ru_msgrcv;
    result.ru_nsignals = ru.ru_nsignals;
    result.ru_nvcsw = ru.ru_nvcsw;
    result.ru_nivcsw = ru.ru_nivcsw;
    result.time_used = result.ru_utime + result.ru_stime;
    // result.memory_used = ru.ru_maxrss;
    result.memory_used = ru.ru_minflt * (sysconf(_SC_PAGESIZE) / 1024);
    result.status = status;
    result.error = WIFSIGNALED(status);

    if (WIFSIGNALED(status))
    {
        switch (WTERMSIG(status))
        {
        case SIGSEGV:
            result.reason = "SIGSEGV";
            break;
        case SIGALRM:
            result.reason = "SIGALRM";
            break;
        case SIGXCPU:
            result.reason = "SIGXCPU";
            break;
        default:
            result.reason = "OTHER";
            break;
        }
        result.status = WTERMSIG(status);
    }
    else
    {
        result.reason = "NULL";
    }

    return result;
}