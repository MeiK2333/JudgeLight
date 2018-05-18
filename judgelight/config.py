# coding=utf-8

import os

COMPILE_TIME_LIMIT = 10000  # ms
COMPILE_MEMORY_LIMIT = 1 * 1024 * 1024 * 1024  # byte

BASE_DIR = os.path.abspath('./')
WORK_DIR = os.path.join(BASE_DIR, 'work')

PLUNGE_PATH = '/home/meik/plunge/src/plunge'

STAND_DIFF_PROCESS_PATH = '/home/meik/JudgeLight/judgelight/stand_diff.py'
STAND_DIFF_PROCESS_CMD = 'python3 {process} {out_file} {ans_file}'
VALIDATOR_TIME_LILIT = 10000
VALIDATOR_MEMORY_LIMIT = 1 * 1024 * 1024 * 1024

if not os.path.exists(PLUNGE_PATH):
    print("Please make sure the file plunge exists")
    exit(1)
