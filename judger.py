# coding=utf-8
from multiprocessing import Pool

from config import SYSTEM_CONFIG
from logger import logger
from models import Judger


def work(judger):
    logger.info('judger {} start'.format(judger.runid))


def main():
    """
    死循环进行评测任务，多进程分发子任务；如果某个评测出现错误，则对应进程更新状态后直接退出
    :return:
    """
    if SYSTEM_CONFIG['max_judger_count'] > 0:
        pool = Pool(SYSTEM_CONFIG['max_judger_count'])
    else:
        pool = Pool()

    while True:
        judger = Judger.get()
        if judger is None:
            continue
        pool.apply_async(work, args=(judger,))
    pool.close()
    pool.join()


if __name__ == '__main__':
    main()
