#include <jl_core.h>

void Exit(int exit_code) {
    /** Process Exit */
    for (auto middleware : middleware_list) {
        middleware->ProcessExit();
    }
    cout << "exit_code: " << exit_code << endl;
    // 拜拜了您内
    exit(exit_code);
}
