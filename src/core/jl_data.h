#ifndef JL_DATA_H
#define JL_DATA_H

#include <string>

using namespace std;

class JudgeLightData {
   public:
    // "/path/to/data/1.in"
    string input_filepath;

    // "/path/to/data/1.out"
    string output_filepath;

    // next
    JudgeLightData *next;
};

#endif
