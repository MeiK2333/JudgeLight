#ifndef JL_RESULT_H
#define JL_RESULT_H

enum JudgeLightResultEnum {
    Accepted = 0,
    WrongAnswer = 1,
    CompileError = 2,
    TimeLimitExceeded = 3,
    MemoryLimitExceeded = 4,
    PresentationError = 5,
    OutputLimitExceeded = 6,
    RuntimeError = 7,
    SystemError = 9,
};

class JudgeLightResult {
   public:
    int time_used;
    int memory_used;
    int result;
};

#endif
