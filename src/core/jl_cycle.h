#ifndef JL_CYCLE_H
#define JL_CYCLE_H

#include <jl_data.h>
#include <string>

using namespace std;

class JudgeLightCycle {
   public:  // data
    // "print('Hello World!')\n"
    string code;

    // 22
    int code_length;

    // 3 (需要运行的次数 / 数据组数)
    int run_count;

    // "main.cpp"
    string filename;

    // ["g++", "main.cc", "-o", "a.out", "--std=gnu++17", "-Wall", nullptr]
    string* compile_args;

    // 5000(ms)
    int compile_time_limit;

    // 327680(mb)
    int compile_memory_limit;

    // ["./a.out", nullptr]
    string* run_args;

    // data1 -> data2 -> data3 -> nullptr
    JudgeLightData* datas;

    // 1000(ms)
    int time_limit;

    // 65535(kb)
    int memory_limit;

   public:  // function
    void PushData(JudgeLightData*);
};

#endif
