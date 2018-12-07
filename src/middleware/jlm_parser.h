#ifndef JLM_PARSER_H
#define JLM_PARSER_H

#include <jlm_base.h>

class ParserMiddleware : BaseMiddleware {
    void ProcessInit();
    void ProcessExit();
};

enum ParserStringCode {
    TimeLimitCode,
    MemoryLimitCode,
    DataCode,
    CodeCode,
    CodeLengthCode,
    FilenameCode,
    CompileArgsCode,
    RunArgsCode,
    DoneCode,
    PassCode,
};

ParserStringCode Hashit(string const&);

#endif
