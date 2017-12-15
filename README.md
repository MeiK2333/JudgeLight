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

### 注意
所有的示例都只是个例子，并不代表必须要这样使用。  
实际上，JudgeLight十分灵活，我和她也很希望能有更多的人使用并发掘出更多有趣的用法。
 
### 基础
```python
>>> from judgelight import judgelight
>>> judgelight()
'Hello JudgeLight!'

>>> from Judge import JudgeLight
>>> judge = JudgeLight()
>>> judge.time_limit = 10000  # <del>注意从 judge 创建的时候 realtime 就开始计时，因此如果要限制time的话，应尽量避免在控制台中限制</del> JudgeLight在执行fork后才会开始计时
>>> judge.memory_limit = 65536
>>> judge.fork()  # 在运行之前应该执行此操作
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
>>> judge.fork()
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

### 重定向流
JudgeLight可以重定向将要运行的程序的stdin、stdout、stderr流，前提是她必须认识要重定向的管道（必须在JudgeLight fork之前被创建）
```python
>>> judge = JudgeLight()
>>> f = open('test.txt')
>>> judge.stdin = f.fileno()  # <del>错误，judge并不认识这个管道</del>可以，现在JudgeLight认识所有在fork之前产生的管道与文件描述符
```

```python
>>> f = open('test2.txt', 'w')
>>> judge = JudgeLight()
>>> judge.stdout = f.fileno()  # 重定向程序的输出流到文件
```

### Special Judge Test
```python
compile = JudgeLight()
compile.fork()
compile.run('g++ specialjudgeserver.cpp -o spjs.out')
compile = JudgeLight()
compile.fork()
compile.run('g++ specialjudgecli.cpp -o spjc.out')

# judgelight中的流只能重定向为她认识的管道，也就是说，如果想要改变某个流，那么相应的管道必须在JudgeLight类初始化之前被创建
fout = open('tmp.out', 'w')

judge_cli = JudgeLight()
judge_cli.fork()
judge_cli.time_limit = 1000
judge_cli.memory_limit = 10000
judge_cli.stdout = fout.fileno()
judge_cli.run('./spjc.out')
fout.close()

fin = open('tmp.out')
sout = open('spj.out', 'w')

judge_server = JudgeLight()
judge_server.fork()
judge_server.stdin = fin.fileno()
judge_server.stdout = sout.fileno()
rst = judge_server.run('./spjs.out')
fin.close()
sout.close()

# special judge server的程序可以通过程序运行的返回值来标识用户程序是否正确
print json.dumps(judge_server.result, indent=4)
```

### 交互式评测
交互式评测需要同时开启两个JudgeLight，并对接他们的输入与输出。在示例中，评测姬可以通过交互评测程序的stderr流获得一些反馈信息，并且根据最终的返回值来确定题目的正确与否。  


## 其他问题

- **内存限制是个大坑**
- **内存测量是个大坑**
- **和内存相关的都是大坑**
- **我也是个大坑**
- 在macOS下，内存限制基本无效，只能等待运行结束或者超时然后判断使用内存是否超出<del>（大概率是因为我太菜了而有哪里写错了）</del>（发现在Ubuntu系统下限制正常，但是Ubuntu系统下测量出来的结果与我预料的差异较大，因此内存测量依旧是一个大坑）
- **在校学习、还有这么多事情的情况下，真的是很难做到每天都敲代码......好多的设想也都没有时间来实现。**
- **是不是应该少承担点事情呢......**

## 下一步（设想）
- <del>交互式评测</del>
- <del>修改JudgeLight的子进程创建时间为run的时候（现在在创建类对象的时候就会fork子进程，讲道理使用起来会有点怪）</del>
- <del>提供更加合理的使用接口（现在的时间限制和重定向有些不合常理的用法）</del>
- 整理出成型的OJ评测姬（我的初步设想是为每个题目创建自己的评测程序，以此获得最大限度的自由和灵活。至于效率......再说吧）
- 其他......