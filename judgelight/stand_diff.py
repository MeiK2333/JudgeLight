# coding=utf-8
import sys


def diff(out, ans):
    out = out.strip()
    ans = ans.strip()
    if out == ans:
        return 0
    if out.split() == ans.split():
        return 1
    return 1


if __name__ == '__main__':
    out_file = sys.argv[1]
    ans_file = sys.argv[2]
    with open(out_file) as fr:
        out_data = fr.read()
    with open(ans_file) as fr:
        ans_data = fr.read()
    exit(diff(out_data, ans_data))
