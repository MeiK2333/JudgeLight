#include <jl_core.h>
#include <jlm_limit.h>
#include <judge_light.h>

/**
 * Middleware
 * 扩展的功能需要在这里注册
 * */
static auto middleware_list = []() {
    vector<BaseMiddleware *> _ = {
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

int main(int argc, char *argv[]) {
    cout << "Hello " JUDGE_LIGHT_VER << endl;
    return 0;
}
