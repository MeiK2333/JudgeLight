#include <jlm_limit.h>
#include <sys/resource.h>
#include <sys/time.h>

void LimitMiddleware::RunChild(int) {
    struct rlimit rl;

    /** 以秒为单位设置（CPU）运行时间限制 */
    /** 1000 -> 2S, 1400 -> 2S, 1500 -> 3S */
    rl.rlim_cur = (jl_cycle->time_limit + 1500) / 1000;
    rl.rlim_max = rl.rlim_cur + 1;
    if (setrlimit(RLIMIT_CPU, &rl) != 0) {
        Exit(SET_LIMIT_TIME_ERROR);
    }

    /**
     * 设置实际运行时间
     * 因为用户程序可能因为状态切换、系统调度等原因运行较久
     * 因此此处限制放宽
     * 但也不能过于宽松，防止用户恶意提交 sleep 等占用资源
     * */
    struct itimerval rt;
    /** 限制时间 * 1.5 + 2500，如果后续出现异常，可以适当再次放宽 */
    long t = int(jl_cycle->time_limit * 1.5) + 2500;
    rt.it_interval.tv_sec = 0;
    rt.it_interval.tv_usec = 0;
    rt.it_value.tv_sec = t / 1000;
    rt.it_value.tv_usec = (int)(t % 1000) * 1000;
    if (setitimer(ITIMER_REAL, &rt, (struct itimerval *)0) != 0) {
        Exit(SET_LIMIT_TIME_ERROR);
    }

    /**
     * 设置内存限制
     * 内存限制比给定值放宽
     * */
    rl.rlim_cur = jl_cycle->memory_limit * 1024 * 2;
    rl.rlim_max = rl.rlim_cur + 1024;
    if (setrlimit(RLIMIT_AS, &rl) != 0) {
        Exit(SET_LIMIT_MEMORY_ERROR);
    }
    if (setrlimit(RLIMIT_DATA, &rl) != 0) {
        Exit(SET_LIMIT_MEMORY_ERROR);
    }

    // TODO
    // 设置最大文件输出限制
    // 超过则 OLE

    // TODO
    // 通过 libseccomp 来进行系统调用的限制
    // 从而确保评测过程安全
}

// TODO
// 为编译过程添加资源限制
// 以保证不会出现：编译时间超长、编译输出巨量信息、编译读取敏感信息等
