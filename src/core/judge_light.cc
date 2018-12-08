#include <jl_core.h>
#include <jlm_limit.h>
#include <jlm_parser.h>
#include <judge_light.h>
#include <unistd.h>

vector<BaseMiddleware *> middleware_list;

/**
 * 保存配置与状态的变量
 * */
JudgeLightCycle *jl_cycle;

/**
 * 完成编译相关操作的函数
 * */
void Compile();

/**
 * 完成运行相关操作的函数
 * */
void Run();
void RunOne(int);

int main() {
    cout << "Hello " JUDGE_LIGHT_VER << endl;

    /**
     * 将扩展的功能添加到此处
     * */
    middleware_list = {
        (BaseMiddleware *)new ParserMiddleware(),
        (BaseMiddleware *)new LimitMiddleware(),
    };

    jl_cycle = new JudgeLightCycle();

    /** Process Init */
    for (auto middleware : middleware_list) {
        middleware->ProcessInit();
    }

    /** Compile */
    Compile();

    /** Run */
    Run();

    /** Process Exit */
    Exit(SUCCESS);
}

void Compile() {
    /** Compile Before */
    for (auto middleware : middleware_list) {
        middleware->CompileBefore();
    }

    pid_t pid;

    if ((pid = fork()) < 0) {
        Exit(COMPILE_FORK_ERROR);
    } else if (pid == 0) {  // child
        /** Compile Child */
        for (auto middleware : middleware_list) {
            middleware->CompileChild();
        }

        // TODO
        // exec...
        exit(0);  // You will never arrive here

    } else {  // parent
        /** Compile Parent */
        for (auto middleware : middleware_list) {
            middleware->CompileParent();
        }

        // TODO
        // waitpid...
    }

    /** Compile After */
    for (auto middleware : middleware_list) {
        middleware->CompileAfter();
    }
}

void Run() {
    /** All Run Before */
    for (auto middleware : middleware_list) {
        middleware->AllRunBefore();
    }

    for (int i = 0; i < jl_cycle->run_count; i++) {
        RunOne(i);
    }

    /** All Run After */
    for (auto middleware : middleware_list) {
        middleware->AllRunAfter();
    }
}

void RunOne(int cnt) {
    /** Run Before */
    for (auto middleware : middleware_list) {
        middleware->RunBefore();
    }

    pid_t pid;

    if ((pid = fork()) < 0) {
        Exit(RUN_FORK_ERROR);
    } else if (pid == 0) {  // child
        /** Run Child */
        for (auto middleware : middleware_list) {
            middleware->RunChild();
        }

        // TODO
        // exec...
        exit(0);  // You will never arrive here

    } else {  // parent
        /** Run Parent */
        for (auto middleware : middleware_list) {
            middleware->RunParent();
        }

        // TODO
        // waitpid...
    }

    /** Run After */
    for (auto middleware : middleware_list) {
        middleware->RunAfter();
    }
}
