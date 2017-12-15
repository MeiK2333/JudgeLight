# coding=utf-8
import json
from Judge import JudgeLight

if __name__ == '__main__':
    compile = JudgeLight()
    # compile.memory_limit = 10000
    # compile.time_limit = 1000
    compile.fork()
    print compile.run('g++ test2.cpp -o test2.out')
    print '------------------------------'
    judge = JudgeLight()
    judge.memory_limit = 10000
    judge.time_limit = 1000
    judge.fork()
    print judge.run('./test2.out')
