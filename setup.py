from distutils.core import setup, Extension

setup(
    name='JudgeLight',
    version='0.0.1',
    ext_modules=[Extension('JudgeLight/JudgeLightRunner', sources=[
        'JudgeLight/JudgeLightRunner/judgelightrunner.c',
        'JudgeLight/JudgeLightRunner/jl_runner.c',
        'JudgeLight/JudgeLightRunner/jl_memory.c',
        'JudgeLight/JudgeLightRunner/jl_limit.c',
        'JudgeLight/JudgeLightRunner/jl_convert.c',
        'JudgeLight/JudgeLightRunner/jl_rules.c',
    ])],
    packages=['JudgeLight']
)
