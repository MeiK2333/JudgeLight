#ifndef JLM_RESULT_H
#define JLM_RESULT_H

#include <jl_core.h>
#include <jlm_base.h>

class ResultMiddleware : BaseMiddleware {
   public:
    void AllRunBefore();
    void RunResult(int, struct rusage &, int &);
};

#endif
