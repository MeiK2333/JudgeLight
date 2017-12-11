# JudgeLight2

## 介绍
没有什么好介绍的，一个精简版的、可用作OJ评测姬的（但是没有什么安全措施）、可以限制与获取程序运行占用资源的小玩意。  
作为一个OJ的评测姬来说，她支持普通评测、special judge、交互题等评测方式，因为她很灵活，使用起来也很简单。如果配合docker来解决安全性问题的话，会是一个可用性很高的评测姬。  
作为一个本地服务，她也可以用于获得程序运行占用资源，可以直观的看出自己程序的性能（大体上...），或者用于测试数据大小是否合理。  
还有就是，以上功能，我其实都没有实现。

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
>>> judge.time_limit = 10000  # 注意从 judge 创建的时候 realtime 就开始计时，因此如果要限制time的话，应尽量避免在控制台中限制
>>> judge.memory_limit = 65536
>>> print judge.run('g++ test2.cpp -o test2.out')  # run 之后无法对 judge 再做操作
'''
cmd:		g++ test2.cpp -o test2.out
time_limit:	-1
memory_limit:	65536
time_used:	332
memory_used:	61968
error:		0
reason:		None
'''



>>> judge = JudgeLight()
>>> judge.time_limit = 1000
>>> judge.memory_limit = 65536
>>> print judge.run('./test2.out')
'''
cmd:		./test2.out
time_limit:	1000
memory_limit:	10000
time_used:	2002  # 程序强制中断限制比time_limit大1000ms
memory_used:	2456  # 内存测量是个大坑，我确实不大会
error:		1
reason:		Time Limit Exceeded
'''
```

## 其他问题

- **内存限制是个大坑**
- **内存测量是个大坑**
- **和内存相关的都是大坑**
- **我也是个大坑**
- 在macOS下，内存限制基本无效，只能等待运行结束或者超时然后判断使用内存是否超出<del>（大概率是因为我太菜了而有哪里写错了）</del>（发现在Ubuntu系统下限制正常，但是Ubuntu系统下测量出来的结果与我预料的差异较大，因此内存测量依旧是一个大坑）