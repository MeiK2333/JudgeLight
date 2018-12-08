#ifndef JLM_PARSER_H
#define JLM_PARSER_H

#include <jl_errno.h>
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
    FilenameCode,
    CompileArgsCode,
    RunArgsCode,
    DoneCode,
    UnknownCode,
};

ParserStringCode Hashit(string const&);

#endif
