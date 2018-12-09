#ifndef JLM_BASE_H
#define JLM_BASE_H

#include <sys/wait.h>

class BaseMiddleware {
   public:
    virtual void ProcessInit(){};  // 程序运行一开始会执行的函数

    virtual void CompileBefore(){};  // 编译之前执行
    virtual void CompileChild(){};   // 编译时子进程执行
    virtual void CompileParent(){};  // 编译时父进程执行
    virtual void CompileResult(
        struct rusage &,
        int &){};  // 父进程等待子进程结束（编译完成）后执行，可以获取资源占用
    virtual void CompileAfter(){};  // 编译结束后执行

    virtual void AllRunBefore(){};  // 运行之前执行
    virtual void RunBefore(){};     // 每组运行前执行
    virtual void RunChild(){};
    virtual void RunParent(){};
    virtual void RunResult(struct rusage &, int &){};
    virtual void RunAfter(){};
    virtual void AllRunAfter(){};  // 所有组运行结束后执行

    virtual void ProcessExit(){};  // 程序退出（Exit）时执行
};

#endif