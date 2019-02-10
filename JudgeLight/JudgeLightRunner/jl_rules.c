#include "jl_rules.h"

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
    } else if (strcmp(rconfig->allow_system_calls_rule, "default") == 0 ||
               strcmp(rconfig->allow_system_calls_rule, "c/cpp") == 0) {
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