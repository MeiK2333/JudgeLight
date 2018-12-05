#ifndef JL_CYCLE_H
#define JL_CYCLE_H

#include <jl_data.h>
#include <string>

using namespace std;

struct JudgeLightCycle {
    // 3 (需要运行的次数 / 数据组数)
    int run_count;

    // ["g++", "main.cc", "-o", "a.out", "--std=gnu++17", "-Wall", nullptr]
    string* compile_args;

    // 5000(ms)
    int compile_time_limit;

    // 327680(mb)
    int compile_memory_limit;

    // ["./a.out", nullptr]
    string* exec_args;

    JudgeLightData* datas;

    // 1000(ms)
    int time_limit;

    // 65535(kb)
    int memory_limit;
};

#endif
