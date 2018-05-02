# coding=utf-8

import os

LANGUAGES = {
    'gcc': {
        'file_name': 'main.c',
        'compile': 'gcc main.c -o main.out --std=gnu11',
        'run': './main.out'
    },
    'g++': {
        'file_name': 'main.cpp',
        'compile': 'g++ main.cpp -o main.out --std=gnu++11',
        'run': './main.out'
    },
    'python': {
        'file_name': 'main.py',
        'compile': 'python3 -m py_compile main.py',
        'run': 'python3 main.py'
    },
    'java': {
        'file_name': 'Main.java',
        'compile': 'javac Main.java',
        'run': 'java Main'
    }
}

COMPILE_TIME_LIMIT = 10000  # ms
COMPILE_MEMORY_LIMIT = 1 * 1024 * 1024 * 1024  # byte

BASE_DIR = os.path.abspath('./')
WORK_DIR = os.path.join(BASE_DIR, 'work')

PLUNGE_PATH = '/home/meik/plunge/src/plunge'

if not os.path.exists(PLUNGE_PATH):
    print("Please make sure the file plunge exists")
    exit(1)
