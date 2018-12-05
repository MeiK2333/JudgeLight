#include <jl_core.h>
#include <jlm_limit.h>
#include <jlm_parser.h>
#include <judge_light.h>

/**
 * Middleware
 * 扩展的功能需要在这里注册
 * */
static auto middleware_list = []() {
    vector<BaseMiddleware *> _ = {
        (BaseMiddleware *)new ParserMiddleware(),
        (BaseMiddleware *)new LimitMiddleware(),
    };
    return _;
}();

/**
 * 保存配置与状态的变量
 * 全局唯一的变量，也是唯一的全局变量
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

    jl_cycle = new JudgeLightCycle();

    /**
     * Process Init
     * */
    for (auto middleware : middleware_list) {
        middleware->ProcessInit();
    }

    /**
     * Compile
     * */
    Compile();

    /**
     * Run
     * */
    Run();

    return 0;
}

void Compile() {
    /** Compile Before */
    for (auto middleware : middleware_list) {
        middleware->CompileBefore();
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

    /** Run After */
    for (auto middleware : middleware_list) {
        middleware->RunAfter();
    }
}