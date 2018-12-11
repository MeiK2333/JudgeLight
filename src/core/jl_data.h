#ifndef JL_DATA_H
#define JL_DATA_H

#include <string>

using namespace std;

class JudgeLightData {
   public:
    // "/path/to/data/1.in"
    char *input_filepath;
    int input_fd;

    // "/path/to/data/1.out"
    char *output_filepath;
    int output_fd;

    // 用于存储程序输出的临时文件
    char *temp_filepath;
    int temp_fd;

    // next
    JudgeLightData *next;
};

#endif
