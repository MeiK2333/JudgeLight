#ifndef JLM_LIMIT_H
#define JLM_LIMIT_H

#include <jl_core.h>
#include <jlm_base.h>

class LimitMiddleware : BaseMiddleware {
   public:
    void RunChild(int);
};

#endif
