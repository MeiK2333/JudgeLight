from distutils.core import setup, Extension

setup(
    name='JudgeLight',
    version='0.0.1',
    ext_modules=[Extension('JudgeLight/JudgeLightRunner', sources=[
        'JudgeLight/JudgeLightRunner/judgelightrunner.c'
    ])],
    packages=['JudgeLight']
)
