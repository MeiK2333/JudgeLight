# coding=utf-8
import json
from Judge import JudgeLight

if __name__ == '__main__':
    compile = JudgeLight()
    compile.run('g++ specialjudgeserver.cpp -o spjs.out')
    compile = JudgeLight()
    compile.run('g++ specialjudgecli.cpp -o spjc.out')

    # judgelight中的流只能重定向为她认识的管道，也就是说，如果想要改变某个流，那么相应的管道必须在JudgeLight类初始化之前被创建
    fout = open('tmp.out', 'w')

    judge_cli = JudgeLight()
    judge_cli.time_limit = 1000
    judge_cli.memory_limit = 10000
    judge_cli.stdout = fout.fileno()
    judge_cli.run('./spjc.out')
    fout.close()

    fin = open('tmp.out')
    sout = open('spj.out', 'w')

    judge_server = JudgeLight()
    judge_server.stdin = fin.fileno()
    judge_server.stdout = sout.fileno()
    rst = judge_server.run('./spjs.out')
    fin.close()
    sout.close()
    print json.dumps(judge_server.result, indent=4)
