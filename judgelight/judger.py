# coding=utf-8
import json
import os
import sys
import uuid
from shutil import rmtree

from config import (COMPILE_MEMORY_LIMIT, COMPILE_TIME_LIMIT,
                    STAND_DIFF_PROCESS_CMD, STAND_DIFF_PROCESS_PATH,
                    VALIDATOR_MEMORY_LIMIT, VALIDATOR_TIME_LILIT)
from plunge import Plunge
from result import Result

result = Result()


def stderr_log(site, msg):
    """ 将 log 输出到 stderr """
    sys.stderr.write('{site}: {msg}\n'.format(site=site, msg=msg))
    sys.stderr.flush()


def set_fifo_stream(fifo_name, stream):
    """ 将流重定向到指定的 fifo 中 """
    if fifo_name is None:
        return
    if os.path.exists(fifo_name):
        os.remove(fifo_name)
    fifo = os.open(fifo_name, os.O_SYNC | os.O_CREAT | os.O_RDWR)
    fifo = open(fifo, 'w')
    if stream == 'stderr':
        sys.stderr = fifo
    elif stream == 'stdout':
        sys.stdout = fifo


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
    set_fifo_stream(judge_data.get('fifo.out'), 'stdout')
    set_fifo_stream(judge_data.get('fifo.err'), 'stderr')
    run_dir = init_file(judge_data)
    stderr_log('init', 'success')
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
    file_name = judge_data['file_name']
    # 将代码写入文件
    with open(os.path.join(run_dir, file_name), 'w') as fw:
        fw.write(judge_data['code'])
    return run_dir


def in_validator(judge_data, in_file, ans_file):
    """ 输入过滤 """
    data = {
        'time_used': -1,
        'memory_used': -1,
        'exit_code': -1,
        'signal': -1,
        'message': '',
    }
    return data


def out_validator(judge_data, in_file, ans_file, out_file):
    """ 输出过滤 """
    data = {
        'time_used': -1,
        'memory_used': -1,
        'exit_code': -1,
        'signal': -1,
        'message': '',
    }

    process = judge_data.get('out_validator')
    cmd = judge_data.get('out_validator')
    if not process:
        process = STAND_DIFF_PROCESS_PATH
        cmd = STAND_DIFF_PROCESS_CMD
    cmd = cmd.format(in_file=in_file, out_file=out_file,
                     ans_file=ans_file, process=process)
    cmd = cmd.split()
    plunge = Plunge(
        cmd[0],
        args=cmd[1:],
        max_cpu_time=VALIDATOR_TIME_LILIT,
        max_real_time=VALIDATOR_TIME_LILIT,
        max_memory=VALIDATOR_MEMORY_LIMIT,
        out_file_name='out_validator.out'
    )
    plunge.run()

    data['time_used'] = plunge.data['cpu_time']
    data['memory_used'] = plunge.data['memory']
    data['exit_code'] = plunge.data['exit_code']
    data['signal'] = plunge.data['signal']
    with open('out_validator.out') as fr:
        data['message'] = fr.read()
    return data


def compile_it(judge_data, run_dir):
    """ 编译程序 """
    this_dir = os.path.abspath('./')
    # 切换工作目录
    os.chdir(run_dir)

    # 限制资源编译
    compile_cmd = judge_data['compile'].split()
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
    stderr_log('compile', 'success')


def run_it(judge_data, run_dir):
    """ 运行程序 """
    this_dir = os.path.abspath('./')
    # 切换工作目录
    os.chdir(run_dir)

    data_cnt = 0
    judge_all = judge_data.get('judge_all')
    for data in judge_data['data']:
        data_cnt += 1
        run_it_data = {
            'in_validator': {},
            'runner': {},
            'out_validator': {}
        }

        # in_validator
        if judge_data.get('in_validator'):
            run_it_data['in_validator'] = in_validator(
                judge_data, data['in_file'], data['ans_file'])
            if run_it_data['in_validator']['exit_code'] == 0:
                stderr_log('in_validator {cnt}'.format(
                    cnt=data_cnt), 'success')
            else:
                stderr_log('in_validator {cnt}'.format(
                    cnt=data_cnt), 'failure')
                if not judge_all:
                    result.exit()

        # 限制资源运行
        run_cmd = judge_data['run'].split()
        plunge = Plunge(
            run_file_name=run_cmd[0],
            args=run_cmd[1:],
            max_cpu_time=judge_data['time_limit'] + 500,
            max_real_time=judge_data['time_limit'] * 2 + 5000,
            max_memory=judge_data['memory_limit'] * 1024,
            in_file_name=data['in_file'],
            out_file_name='run.out',
            err_file_name='run.err'
        )
        plunge.run()

        run_data = {}

        # 收集运行数据
        run_data['time_used'] = plunge.data['cpu_time']
        run_data['memory_used'] = plunge.data['memory']
        run_data['exit_code'] = plunge.data['exit_code']
        run_data['signal'] = plunge.data['signal']
        with open('run.err') as fr:
            run_data['message'] = fr.read()
        run_it_data['runner'] = run_data
        # 标记此组 run 完成
        stderr_log('run {cnt}'.format(cnt=data_cnt), 'success')

        run_it_data['runner']['signal'] = test_run_error(judge_data, run_data)
        # 验证 out_validator
        if run_it_data['runner']['signal'] == 0:
            run_it_data['out_validator'] = out_validator(
                judge_data, data['in_file'], data['ans_file'], 'run.out')
            if run_it_data['out_validator']['exit_code'] == 0:
                stderr_log('out_validator {cnt}'.format(
                    cnt=data_cnt), 'success')
            else:
                stderr_log('out_validator {cnt}'.format(
                    cnt=data_cnt), 'failure')
                if not judge_all:
                    result.exit()
        elif not judge_all:
            result.run.append(run_it_data)
            os.chdir(this_dir)
            return
        result.run.append(run_it_data)

    # 切换回来工作目录
    os.chdir(this_dir)


def test_run_error(judge_data, run_data):
    """ 返回此组测试是否出错 """
    if run_data['signal'] != 0:
        return run_data['signal']
    if run_data['time_used'] > judge_data['time_limit']:
        return 24
    if run_data['memory_used'] > judge_data['memory_limit']:
        return 11
    return 0


def clear_run_dir(run_dir):
    """ 清理运行目录 """
    rmtree(run_dir)


if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        result.message = 'Unknown error'
        result.exit()
