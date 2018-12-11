#ifndef JL_DATA_H
#define JL_DATA_H

#include <string>

using namespace std;

class JudgeLightData {
   public:
    // "/path/to/data/1.in"
    const char *input_filepath;
    int input_fd;

    // "/path/to/data/1.out"
    const char *output_filepath;
    int output_fd;

    // next
    JudgeLightData *next;
};

#endif
