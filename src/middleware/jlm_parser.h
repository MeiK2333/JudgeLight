#ifndef JLM_PARSER_H
#define JLM_PARSER_H

#include <jlm_base.h>

class ParserMiddleware : BaseMiddleware {
    void ProcessInit();
};

enum ParserStringCode {
    TimeLimitCode,
    MemoryLimitCode,
    DataCode,
    CodeCode,
    CodeLengthCode,
    DoneCode,
    PassCode,
};

ParserStringCode Hashit(string const&);

#endif
