#ifndef JL_CORE_H
#define JL_CORE_H

#include <jl_cycle.h>
#include <jl_errno.h>
#include <jlm_base.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

extern JudgeLightCycle *jl_cycle;

extern vector<BaseMiddleware *> middleware_list;

/**
 * GG 时跑路的函数
 * */
void Exit(int);

#endif
