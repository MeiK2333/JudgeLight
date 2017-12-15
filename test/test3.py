# coding=utf-8
import os
import json
import threading

from Judge import JudgeLight


def comp(cmd):
    j = JudgeLight()
    j.fork()
    j.run(cmd)


def runit(judgelight, cmd):
    judgelight.run(cmd)


if __name__ == '__main__':
    comp('g++ test3cli.cpp -o test3cli.out')
    comp('g++ test3server.cpp -o test3server.out')

    pub_1 = os.pipe()  # 创建公用管道
    pub_2 = os.pipe()

    s1 = JudgeLight()
    s2 = JudgeLight()

    f = open('test3.txt', 'w')
    s1.stdin = pub_1[0]  # 管道连接两个程序
    s2.stdout = pub_1[1]
    s2.stdin = pub_2[0]
    s1.stdout = pub_2[1]

    s1.time_limit = 3000
    s2.time_limit = 3000

    s2.stderr = f.fileno()

    s1.fork()
    s2.fork()

    s1.fd_close(pub_1[1])
    s2.fd_close(pub_1[0])
    s2.fd_close(pub_2[1])
    s1.fd_close(pub_2[0])

    t1 = threading.Thread(target=runit, args=(s1, './test3cli.out', ))
    t1.start()

    s2.run('./test3server.out')
    print json.dumps(s2.result, indent=2)

    os.close(pub_1[0])
    os.close(pub_1[1])
    os.close(pub_2[0])
    os.close(pub_2[1])
