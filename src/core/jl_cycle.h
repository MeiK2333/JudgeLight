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

    // 2 （需要运行的次数 / 数据组数）（从 0 开始）
    int run_count;

    // "main.cpp"
    string filename;

    // 运行时的临时目录
    char* temp_dir;

    // ["g++", "main.cc", "-o", "a.out", "--std=gnu++17", "-Wall", nullptr]
    char** compile_args;

    // 5000(ms)
    int compile_time_limit;

    // 327680(mb)
    int compile_memory_limit;

    // ["./a.out", nullptr]
    char** run_args;

    // data1 -> data2 -> data3 -> nullptr
    JudgeLightData* datas;

    // 1000(ms)
    int time_limit;

    // 65535(kb)
    int memory_limit;

    // 程序退出对应的状态码
    int exit_code;

   public:  // function
    void PushData(JudgeLightData*);
    JudgeLightData* GetData(int);
};

#endif
