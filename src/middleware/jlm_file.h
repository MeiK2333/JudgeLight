#ifndef JLM_FILE_H
#define JLM_FILE_H

#include <jlm_base.h>

class FileMiddleware : BaseMiddleware {
   public:
    void ProcessInit();
    void RunChild(int);
    void ProcessExit();
};

void remove_dir(const char *);

#endif
