# coding=utf-8
import re
import subprocess

from config import SYSTEM_CONFIG


def get_int_form_str(string):
    try:
        integer = int(re.sub("\D", "", string))
    except ValueError:
        integer = 0
    return integer


class Plunge(object):

    def __init__(self, run_file_name, in_file_name=None, out_file_name=None, err_file_name='/dev/null',
                 max_cpu_time=None, max_real_time=None, max_memory=None, uid=None, gid=None, args=None,
                 max_stack=None, max_output_size=None):
        cmd = [SYSTEM_CONFIG['plunge_path'],
               '--run_file_name={run_file_name}'.format(run_file_name=run_file_name)]
        if args:
            assert isinstance(args, list)
            for arg in args:
                cmd.append('--args={arg}'.format(arg=arg))
        if in_file_name:
            cmd.append(
                '--in_file_name={in_file_name}'.format(in_file_name=in_file_name.strip()))
        if out_file_name:
            cmd.append(
                '--out_file_name={out_file_name}'.format(out_file_name=out_file_name.strip()))
        if err_file_name:
            cmd.append(
                '--err_file_name={err_file_name}'.format(err_file_name=err_file_name.strip()))
        if max_cpu_time:
            cmd.append(
                '--max_cpu_time={max_cpu_time}'.format(max_cpu_time=max_cpu_time))
        if max_real_time:
            cmd.append(
                '--max_real_time={max_real_time}'.format(max_real_time=max_real_time))
        if max_memory:
            cmd.append(
                '--max_memory={max_memory}'.format(max_memory=max_memory))
        if uid:
            cmd.append('--uid={uid}'.format(uid=uid))
        if gid:
            cmd.append('--gid={gid}'.format(gid=gid))
        if max_stack:
            cmd.append('--max_stack={max_stack}'.format(max_stack=max_stack))
        if max_output_size:
            cmd.append(
                '--max_output_size={max_output_size}'.format(max_output_size=max_output_size))
        # cmd.append('--show')

        self.cmd = cmd
        self.result = {}
        self.out = ''
        self.err = ''

    def run(self):
        pro = subprocess.Popen(
            self.cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = pro.communicate()
        self.out = bytes.decode(out)
        self.err = bytes.decode(err)
        self.parser()

    def parser(self):
        int_field = ['cpu_time', 'real_time', 'memory', 'exit_code', 'signal', 'status', 'max_cpu_time',
                     'max_real_time',
                     'max_memory', 'max_stack', 'max_output_size', 'gid', 'uid']
        str_field = ['run_file_name', 'in_file_name',
                     'out_file_name', 'args', 'err_file_name']
        data = self.err.split('\n')
        for line in data:
            field = line.split(':')[0]
            if field in int_field:
                self.result[field] = get_int_form_str(line)
            elif field in str_field:
                string = line[len(field) + 1:].strip()
                self.result[field] = string
