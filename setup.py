#!/usr/bin/env python
# coding=utf-8
from distutils.core import setup, Extension

sources = [
    'judgelight/src/judgelight.c', 'judgelight/src/limit.c', 'judgelight/src/listen.c', 'judgelight/src/run.c'
]
setup(
    name='JudgeLight',
    version='1.0.5',
    ext_modules=[Extension('JudgeLight/judgelight', sources=sources)],
    packages=['JudgeLight'],
    license='Free',
    author='MeiK',
    author_email='meik2333@gmail.com',
    url='https://github.com/MeiK-h/JudgeLight',
)
