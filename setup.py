import sys
from distutils.core import Extension, setup

sources = [
    'JudgeLight/JudgeLightRunner/judgelightrunner.c',
    'JudgeLight/JudgeLightRunner/jl_runner.c',
    'JudgeLight/JudgeLightRunner/jl_memory.c',
    'JudgeLight/JudgeLightRunner/jl_limit.c',
    'JudgeLight/JudgeLightRunner/jl_convert.c',
    'JudgeLight/JudgeLightRunner/jl_rules.c',
]

setup(
    name='JudgeLight',
    version='2.0.5',
    ext_modules=[Extension('JudgeLight/JudgeLightRunner', sources=sources)],
    packages=['JudgeLight'],
    license='MIT',
    author='MeiK',
    author_email='meik2333@gmail.com',
    url='https://github.com/MeiK2333/JudgeLight',
)
