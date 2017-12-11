# coding=utf-8
from Judge import JudgeLight


def test(cmd):
    s = JudgeLight()
    print s.run(cmd)


if __name__ == '__main__':
    test('g++ test1.cpp')
    test('./a.out')
