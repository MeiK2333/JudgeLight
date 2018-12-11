#ifndef JLM_FILE_H
#define JLM_FILE_H

#include <jlm_base.h>

class FileMiddleware : BaseMiddleware {
   public:
    void ProcessInit();
    void ProcessExit();
};

int remove_dir(const char *);

#endif
