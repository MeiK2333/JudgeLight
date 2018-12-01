#ifndef JL_CORE_H
#define JL_CORE_H

#include <jl_cycle.h>
#include <iostream>
#include <string>

using namespace std;

#include <jlm_base.h>
#include <jlm_limit.h>

void jl_compile(JudgeLightCycle *);

void jl_run(JudgeLightCycle *);

#endif
