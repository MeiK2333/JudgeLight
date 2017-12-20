#include <sys/resource.h>
#include <sys/time.h>
#include <string.h>

#include "limit.h"

int setRunLimit(char *type, unsigned long long l)
{
    struct rlimit rl;
    if (strcmp(type, "cpu") == 0) // 设置CPU运行时间限制，防止死循环执行
    {
        rl.rlim_cur = l / 1000 + 1;
        rl.rlim_max = rl.rlim_cur + 1; // 软限制向上取整，硬限制额外再加一秒
        setrlimit(RLIMIT_CPU, &rl);
    }
    else if (strcmp(type, "time") == 0) // 设置墙上时钟时间限制，防止程序无限sleep
    {
        struct itimerval p_realt;
        p_realt.it_interval.tv_sec = l / 1000 + 1;
        p_realt.it_interval.tv_usec = 0;
        p_realt.it_value = p_realt.it_interval;
        setitimer(ITIMER_REAL, &p_realt, (struct itimerval *)0);
    }
    else if (strcmp(type, "data") == 0) // 设置数据段最大长度
    {
        rl.rlim_cur = l * 1024;
        rl.rlim_max = rl.rlim_cur + 1024;
        setrlimit(RLIMIT_DATA, &rl);
    }
    else if (strcmp(type, "as") == 0) // 设置进程可用储存空间最大长度
    {
        rl.rlim_cur = l * 1024;
        rl.rlim_max = rl.rlim_cur + 1024;
        setrlimit(RLIMIT_AS, &rl);
    }
    else if (strcmp(type, "stack") == 0) // 设置栈空间大小
    {
        rl.rlim_cur = l * 1024;
        rl.rlim_max = rl.rlim_cur + 1024;
        setrlimit(RLIMIT_STACK, &rl);
    }
    return 0;
}