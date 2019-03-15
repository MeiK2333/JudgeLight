import sys
from distutils.core import Extension, setup

sources = [
    'JudgeLight/JudgeLightRunner/judgelightrunner.c',
    'JudgeLight/JudgeLightRunner/jl_runner.c',
    'JudgeLight/JudgeLightRunner/jl_memory.c',
    'JudgeLight/JudgeLightRunner/jl_limit.c',
    'JudgeLight/JudgeLightRunner/jl_convert.c',
]
if sys.platform.lower() != 'darwin':
    sources.append('JudgeLight/JudgeLightRunner/jl_rules.c')

setup(
    name='JudgeLight',
    version='2.0.4',
    ext_modules=[Extension('JudgeLight/JudgeLightRunner', sources=sources)],
    packages=['JudgeLight'],
    license='MIT',
    author='MeiK',
    author_email='meik2333@gmail.com',
    url='https://github.com/MeiK-h/JudgeLight',
)
