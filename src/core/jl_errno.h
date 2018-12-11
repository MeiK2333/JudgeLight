#ifndef JL_ERRNO_H
#define JL_ERRNO_H

enum JudgeLightErrno {
    SUCCESS = 0,
    INIT_ERROR,

    FILE_NOT_FOUND_ERROR = 100,  // 文件未找到
    FILE_NOT_PERMISSION_ERROR,   // 文件无权限
    FILE_OPEN_ERROR,             // open 打开文件错误
    FILE_CREATE_DIR_ERROR,       // 创建文件夹失败
    FILE_CREATE_TEMP_ERROR,      // 创建临时文件失败
    FILE_CHDIR_ERROR,            // 切换工作目录失败
    FILE_CREATE_ERROR,           // 创建文件失败
    FILE_DUP2_ERROR,             // dup2 失败（我反正没见过。。。）

    COMPILE_INIT_ERROR = 200,
    COMPILE_FORK_ERROR,
    COMPILE_EXEC_ERROR,
    COMPILE_EXEC_ARGS_ERROR,
    COMPILE_WAIT_ERROR,

    RUN_INIT_ERROR = 300,
    RUN_FORK_ERROR,
    RUN_EXEC_ERROR,
    RUN_EXEC_ARGS_ERROR,
    RUN_WAIT_ERROR,

    PARSER_ERROR = 400,
    PARSER_UNKNOWN_ARGS,

    SYSTEM_ERROR = 900,
};

#endif
