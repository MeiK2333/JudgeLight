#ifndef JL_RULES_H
#define JL_RULES_H

#include "jl_runner.h"

#include <string.h>
#include <sys/user.h>

#if __WORDSIZE == 64
#define REG_SYS_CALL(x) ((x)->orig_rax)
#define SYS_CALL_ARG_1(x) ((x)->rdi)
#define SYS_CALL_ARG_2(x) ((x)->rsi)
#define SYS_CALL_ARG_3(x) ((x)->rdx)
#else
#define REG_SYS_CALL(x) ((x)->orig_eax)
#define SYS_CALL_ARG_1(x) ((x)->edi)
#define SYS_CALL_ARG_2(x) ((x)->esi)
#define SYS_CALL_ARG_3(x) ((x)->edx)
#endif

int SetSyscallRules(struct RunnerConfig *);

int CheckSyscallRule(struct RunnerConfig *, struct user_regs_struct *);

#endif
