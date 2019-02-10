#ifndef JL_RUNNER_H
#define JL_RUNNER_H

/**
 * 待运行程序的配置
 * */
struct RunnerConfig {
    char *exec_file_path;
    char **exec_args;
    char **envs;

    int time_limit;
    int real_time_limit;
    int memory_limit;
    int output_size_limit;
    int stack_limit;

    char *input_file_path;
    char *output_file_path;
    char *error_file_path;

    int uid;
    int gid;

    char trace;
    char *allow_system_calls_rule;
    char system_calls[1024];
};

#define UNLIMITED -1

/**
 * 程序运行后的结果与资源占用
 * */
struct RunnerStats {
    int time_used;
    int real_time_used;
    int memory_used;
    int signum;
    int re_flag;
    int re_syscall;
};

int RunIt(struct RunnerConfig *, struct RunnerStats *);

#endif
