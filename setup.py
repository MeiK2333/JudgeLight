#!/usr/bin/env python
# coding=utf-8
from distutils.core import setup, Extension

sources = [
    'judgelight/src/judgelight.c', 'judgelight/src/limit.c', 'judgelight/src/listen.c', 'judgelight/src/run.c'
]
setup(
    name='judgelight',
    version='1.0.2',
    ext_modules=[Extension('judgelight/judgelight', sources=sources)],
    packages=['judgelight'],
    python_requires='<=2.7',
    license='Free',
    author='MeiK',
    author_email='meik2333@gmail.com',
    url='https://github.com/MeiK-h/JudgeLight2',
)
