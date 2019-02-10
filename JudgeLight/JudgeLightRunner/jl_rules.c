#include "jl_rules.h"

#include <sys/ptrace.h>
#include <sys/syscall.h>

/**
 * 设置系统调用规则
 * */
int SetSyscallRules(struct RunnerConfig *rconfig) {
    // TODO
    if (rconfig->allow_system_calls_rule == NULL) {
        /** 不提供则开放所有调用 */
        for (int i = 0; i < 1024; i++) {
            rconfig->system_calls[i] = 0;
        }
        return 0;
    }

    /** 首先禁止所有调用 */
    for (int i = 0; i < 1024; i++) {
        rconfig->system_calls[i] = 1;
    }
    if (strcmp(rconfig->allow_system_calls_rule, "default") == 0 ||
        strcmp(rconfig->allow_system_calls_rule, "c/cpp") == 0 ||
        //     ) {
        //     int allow_syscall[1024] = {
        //         SYS_read,  SYS_write,    SYS_writev,     SYS_readlink,
        //         SYS_fstat, SYS_mmap,     SYS_munmap,     SYS_uname,
        //         SYS_brk,   SYS_lseek,    SYS_exit,       SYS_access,
        //         SYS_close, SYS_mprotect, SYS_arch_prctl, SYS_exit_group,
        //         -1};
        //     /** 放行白名单 */
        //     for (int i = 0; allow_syscall[i] != -1; i++) {
        //         rconfig->system_calls[allow_syscall[i]] = 0;
        //     }
        // } else if (
        strcmp(rconfig->allow_system_calls_rule, "python") == 0) {
        int allow_syscall[1024] = {SYS_dup,
                                   SYS_brk,
                                   SYS_exit,
                                   SYS_read,
                                   SYS_mmap,
                                   SYS_stat,
                                   SYS_write,
                                   SYS_lseek,
                                   SYS_lstat,
                                   SYS_fcntl,
                                   SYS_close,
                                   SYS_fstat,
                                   SYS_futex,
                                   SYS_uname,
                                   SYS_ioctl,
                                   SYS_munmap,
                                   SYS_access,
                                   SYS_getcwd,
                                   SYS_getgid,
                                   SYS_getuid,
                                   SYS_getpid,
                                   SYS_writev,
                                   SYS_sysinfo,
                                   SYS_geteuid,
                                   SYS_getegid,
                                   SYS_readlink,
                                   SYS_mprotect,
                                   SYS_getdents,
                                   SYS_getrandom,
                                   SYS_prlimit64,
                                   SYS_arch_prctl,
                                   SYS_exit_group,
                                   SYS_sigaltstack,
                                   SYS_rt_sigaction,
                                   SYS_rt_sigprocmask,
                                   SYS_set_robust_list,
                                   SYS_set_tid_address,
                                   -1};
        for (int i = 0; allow_syscall[i] != -1; i++) {
            rconfig->system_calls[allow_syscall[i]] = 0;
        }
    }
    return 0;
}

/**
 * 检查系统调用限制
 * */
int CheckSyscallRule(struct RunnerConfig *rconfig,
                     struct user_regs_struct *regs) {
    return rconfig->system_calls[REG_SYS_CALL(regs)];
}