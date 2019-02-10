import os

from .JudgeLightRunner import run as jlrun


class JudgeLight(object):
    def __init__(
        self,

        exec_file_path,  # 要执行的程序路径
        exec_args=None,  # 执行时的命令行参数
        envs=None,  # 执行时的 env

        time_limit=None,  # 时间限制
        real_time_limit=None,  # 真实时间限制
        memory_limit=None,  # 内存限制
        output_size_limit=None,  # 最大输出限制
        stack_limit=None,  # 栈限制

        input_file_path=None,  # 输入数据文件（None 则从控制台输入）
        output_file_path=None,  # 输出数据文件（None 输出到控制台，无对应文件则创建）
        error_file_path=None,  # 同输出

        uid=None,  # 执行的用户 id
        gid=None,  # 执行的用户组 id
        trace=True,  # 是否使用 ptrace 进行系统调用限制，如果不使用的话，同时将无法获得准确的内存使用量（偏大）
        allow_system_calls_rule=None,  # 允许执行的系统调用规则（在 C 代码中定义）
    ):
        run_config = {}

        str_list_vars = ['exec_args', 'envs']
        int_vars = ['time_limit', 'memory_limit', 'real_time_limit',
                    'output_size_limit', 'stack_limit', 'uid', 'gid']
        str_vars = ['exec_file_path', 'input_file_path',
                    'output_file_path', 'error_file_path', 'allow_system_calls_rule']

        if exec_args is None:
            exec_args = [exec_file_path]
        if exec_args[0] != exec_file_path:
            exec_args = [exec_file_path] + exec_args

        # 未提供 envs 则使用当前的 envs
        if envs is None:
            envs = [f'{key}={os.environ[key]}' for key in os.environ]

        if not isinstance(trace, bool):
            raise ValueError(f'trace must be a bool')
        run_config['trace'] = trace

        for var in str_list_vars:
            value = vars()[var]
            if value is None:
                continue
            if not isinstance(value, list):
                raise ValueError(f'{var} must be a list')
            for item in value:
                if not isinstance(item, str):
                    raise ValueError(f'{var} item must be a string')
            run_config[var] = value

        for var in int_vars:
            value = vars()[var]
            if value is None:
                continue
            if not isinstance(value, int):
                raise ValueError(f'{var} must be a int')
            run_config[var] = value

        for var in str_vars:
            value = vars()[var]
            if value is None:
                continue
            if not isinstance(value, str):
                raise ValueError(f'{var} must be a string')
            run_config[var] = value

        self.run_config = run_config

    def run(self):
        stats = jlrun(self.run_config)
        return stats
