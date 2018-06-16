# coding=utf-8
import os

ABSPATH = os.path.abspath('./')

JUDGER_CONFIG = {
    'compile_time_limit': 10000,  # 编译时间限制 (ms)
    'compile_memory_limit': 1 * 1024 * 1024 * 1024,  # 编译内存限制 (byte)
    'validator': os.path.join(ABSPATH, 'validator.py'),  # 标准答案检查器
    'validator_cmd': 'python3 {process} {in_file} {out_file} {ans_file}',  # 答案检查器执行命令
    'validator_time_limit': 10000,  # 答案检查器时间限制 (ms)
    'validator_memory_limit': 1 * 1024 * 1024 * 1024,  # 答案检查器内存限制 (byte)
    'data_dir': os.path.join(ABSPATH, 'data'),  # data 数据目录

    # 支持的语言及其评测配置
    'language': {
        'gcc': {
            'file_name': 'main.c',
            'compile_cmd': 'gcc main.c -o main.out --std=gnu11 -O2 -Wall',
            'run_cmd': './main.out'
        },
        'g++': {
            'file_name': 'main.cpp',
            'compile_cmd': 'g++ main.cpp -o main.out --std=gnu++11 -O2 -Wall',
            'run_cmd': './main.out'
        },
        'python': {
            'file_name': 'main.py',
            'compile_cmd': 'python3 -m py_compile main.py',
            'run_cmd': 'python3 main.py'
        },
    }
}

SYSTEM_CONFIG = {
    'plunge_path': os.path.join(ABSPATH, 'plunge'),  # plunge 组件位置
    'work_dir': os.path.join(ABSPATH, 'work'),  # work 工作目录

    'max_judger_count': -1,  # 同时评测的最大数量 (-1 为默认 CPU 核数)

    'flask_host': '0.0.0.0',
    'flask_port': 5000,
    'redis_host': '127.0.0.1',
    'redis_port': '6379',
    'redis_list': 'judger:list',
    'redis_result': 'judger:result',
}
