#ifndef JLM_BASE_H
#define JLM_BASE_H

class BaseMiddleware {
   public:
    virtual void ProcessInit(){};

    virtual void CompileBefore(){};
    virtual void CompileChild(){};
    virtual void CompileParent(){};
    virtual void CompileAfter(){};

    virtual void AllRunBefore(){};
    virtual void RunBefore(){};
    virtual void RunChild(){};
    virtual void RunParent(){};
    virtual void RunAfter(){};
    virtual void AllRunAfter(){};

    virtual void ProcessExit(){};
};

#endif