import os

from JudgeLight import JudgeLight


def compile_it(code, output_name='a.out'):
    with open('code.cpp', 'w') as fw:
        fw.write(code)
    jl = JudgeLight(
        '/usr/bin/g++', exec_args=['/usr/bin/g++', 'code.cpp', '-o', output_name], trace=False, error_file_path='error.txt')
    jl.run()
    os.remove('error.txt')


def clear_it(name='a.out'):
    if os.path.exists('code.cpp'):
        os.remove('code.cpp')
    if os.path.exists(name):
        os.remove(name)
