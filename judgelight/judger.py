# coding=utf-8
import json
import os
import sys
import uuid
from shutil import rmtree

from config import LANGUAGES, COMPILE_MEMORY_LIMIT, COMPILE_TIME_LIMIT
from plunge import Plunge
from result import Result

result = Result()


def main():
    if len(sys.argv) < 2:
        result.message = 'The parameter {judge_json_file} is required'
        result.exit()
    if not os.path.exists(sys.argv[1]):
        result.message = '{judge_json_file} is not exists'
        result.exit()
    # 读取 json 数据
    with open(sys.argv[1]) as fr:
        data = fr.read()
        try:
            judge_data = json.loads(data)
        except json.decoder.JSONDecodeError:
            result.message = 'JSON parse error'
            result.exit()
    for field in ['code', 'data', 'time_limit', 'memory_limit', 'language']:
        if not judge_data.get(field):
            result.message = 'field \'{field}\' is required'.format(
                field=field)
            result.exit()
    language = judge_data.get('language', 'unknown')
    # 检查语言是否存在
    if language not in LANGUAGES:
        result.message = 'Unknown language \'{}\''.format(language)
        result.exit()
    run_dir = init_file(judge_data)
    compile_it(judge_data, run_dir)
    run_it(judge_data, run_dir)
    clear_run_dir(run_dir)
    result.exit()


def init_file(judge_data):
    """ 检查文件合法性并创建运行目录 """
    for data in judge_data['data']:
        in_file = data.get('in_file')
        ans_file = data.get('ans_file')
        # 判断文件是否存在
        if not in_file or not ans_file:
            result.message = 'in_file and ans_file is required'
            result.exit()
        if not os.path.exists(in_file) or not os.path.exists(ans_file):
            result.message = 'in_file or ans_file is not exists'
            result.exit()
    if not os.path.exists('work'):
        result.message = 'work is not exists'
        result.exit()
    # 创建随机的运行目录
    run_dir_uuid = str(uuid.uuid1())
    run_dir = os.path.join(os.path.abspath('./'), 'work', run_dir_uuid)
    os.mkdir(run_dir)
    # 检查文件夹创建是否成功（以前被这个问题坑过……）
    if not os.path.exists(run_dir):
        result.message = 'mkdir error'
        result.exit()
    language = judge_data['language']
    file_name = LANGUAGES[language]['file_name']
    # 将代码写入文件
    with open(os.path.join(run_dir, file_name), 'w') as fw:
        fw.write(judge_data['code'])
    return run_dir


def in_validator(judge_data):
    """ 输入过滤 """
    pass


def out_validator(judge_data):
    """ 输出过滤 """
    pass


def compile_it(judge_data, run_dir):
    """ 编译程序 """
    this_dir = os.path.abspath('./')
    # 切换工作目录
    os.chdir(run_dir)

    # 限制资源编译
    compile_cmd = LANGUAGES[judge_data['language']]['compile'].split()
    plunge = Plunge(
        run_file_name=compile_cmd[0],
        args=compile_cmd[1:],
        max_cpu_time=COMPILE_TIME_LIMIT,
        max_real_time=COMPILE_TIME_LIMIT,
        max_memory=COMPILE_MEMORY_LIMIT,
        err_file_name='compile.err'
    )
    plunge.run()

    result.compile['time_used'] = plunge.data['cpu_time']
    result.compile['memory_used'] = plunge.data['memory']
    result.compile['exit_code'] = plunge.data['exit_code']
    result.compile['signal'] = plunge.data['signal']
    # 读取编译错误信息
    with open('compile.err') as fr:
        result.compile['message'] = fr.read()
    if plunge.data['exit_code'] != 0:
        result.message = 'Compile Error'
        clear_run_dir(run_dir)
        result.exit()

    # 切换回来工作目录
    os.chdir(this_dir)


def run_it(judge_data, run_dir):
    """ 运行程序 """
    this_dir = os.path.abspath('./')
    # 切换工作目录
    os.chdir(run_dir)

    for data in judge_data['data']:
        # 限制资源运行
        run_cmd = LANGUAGES[judge_data['language']]['run'].split()
        plunge = Plunge(
            run_file_name=run_cmd[0],
            args=run_cmd[1:],
            max_cpu_time=judge_data['time_limit'],
            max_real_time=judge_data['time_limit'] * 2 + 5000,
            max_memory=judge_data['memory_limit'] * 1024,
            in_file_name=data['in_file'],
            out_file_name='run.out',
            err_file_name='run.err'
        )
        plunge.run()

        run_data = {}

        run_data['time_used'] = plunge.data['cpu_time']
        run_data['memory_used'] = plunge.data['memory']
        run_data['exit_code'] = plunge.data['exit_code']
        run_data['signal'] = plunge.data['signal']
        with open('run.err') as fr:
            run_data['message'] = fr.read()
        result.run.append(run_data)
    # 切换回来工作目录
    os.chdir(this_dir)


def clear_run_dir(run_dir):
    """ 清理运行目录 """
    rmtree(run_dir)


if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print(repr(e))
        result.message = 'Unknown error'
        result.exit()
