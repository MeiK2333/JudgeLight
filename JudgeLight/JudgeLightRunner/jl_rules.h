#ifndef JL_RULES_H
#define JL_RULES_H

#include "jl_runner.h"

#include <string.h>
#include <sys/user.h>

#if __WORDSIZE == 64
#define REG_SYS_CALL(x) ((x)->orig_rax)
#else
#define REG_SYS_CALL(x) ((x)->orig_eax)
#endif

int SetSyscallRules(struct RunnerConfig *);

int CheckSyscallRule(struct RunnerConfig *, struct user_regs_struct *);

#endif
