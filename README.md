# JudgeLight2

## 编译指令
```shell
gcc -fPIC -shared judgelight.c limit.c listen.c run.c -o judgelight.so -I/usr/include/python2.7/ -lpython2.7
```

## 使用
```python
>>> from judgelight import judgelight
>>> judgelight()
'Hello JudgeLight!'

>>> from Judge import JudgeLight
>>> judge = JudgeLight()
```