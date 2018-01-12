# JudgeLight2

## 介绍
没有什么好介绍的，一个精简版的、可用作OJ评测姬的（但是没有什么安全措施）、可以限制与获取程序运行占用资源的小玩意。  
作为一个OJ的评测姬来说，她支持普通评测、special judge、交互题等评测方式，因为她很灵活，使用起来也很简单。如果配合docker来解决安全性问题的话，会是一个可用性很高的评测姬。  
作为一个本地服务，她也可以用于获得程序运行占用资源，可以直观的看出自己程序的性能（大体上...），或者用于测试数据大小是否合理。  
<del>还有就是，以上功能，我其实都没有实现。</del>


## 安装

### 作为Python模块

现在可以直接 pip 安装
```bash
pip install judgelight
```

或者其他的方式
```bash
$ python setup.py install
$ python
```
```python
>>> from judgelight import JudgeLight
>>> judge = JudgeLight()
>>> judge.fork()
>>> print judge.run('ls')
'''
LICENSE		README.md	install.sh	judgelight	setup.py

cmd:		ls
time_limit:	-1 ms
memory_limit:	-1 kb
time_used:	0 ms
memory_used:	1236 kb
error:		0
reason:		None
status:		0
'''
```

### 作为程序安装
```bash
$ sudo bash install.sh
$ judgelight ls
LICENSE		README.md	install.sh	judgelight	setup.py

cmd:		ls
time_limit:	-1 ms
memory_limit:	-1 kb
time_used:	0 ms
memory_used:	1236 kb
error:		0
reason:		None
status:		0

$ judgelight ls > out.txt  # judgelight中stdin和stdout的重定向依然有效，程序运行的结果通过stderr流传出
cmd:		ls
time_limit:	-1 ms
memory_limit:	-1 kb
time_used:	0 ms
memory_used:	1236 kb
error:		0
reason:		None
status:		0
```
注意，如果您的Python未安装在默认位置，则可能需要修改install.sh中的Python路径


## 使用

### 注意
所有的示例都只是个例子，并不代表必须要这样使用。  
实际上，JudgeLight十分灵活，我和她也很希望能有更多的人使用并发掘出更多有趣的用法。
 
### 基础
```python
>>> from judgelight import JudgeLight
>>> judge = JudgeLight()
>>> judge.time_limit = 1000
>>> judge.memory_limit = 10000  # 此操作在macOS下基本无效，在Ubuntu下有效
>>> judge.fork()  # 从此处开始初始化子进程
>>> print judge.run('ls')
'''
LICENSE		README.md	install.sh	judgelight	setup.py

cmd:		ls
time_limit:	1000 ms
memory_limit:	10000 kb
time_used:	0 ms
memory_used:	1236 kb
error:		0
reason:		None
status:		0
'''
```

### 重定向流
JudgeLight可以重定向将要运行的程序的stdin、stdout、stderr流，前提是她必须认识要重定向的管道（必须在JudgeLight fork之前被创建）
```python
>>> judge = JudgeLight()
>>> f = open('test.txt')
>>> judge.stdin = f.fileno()  # JudgeLight认识所有在fork之前产生的管道与文件描述符
```

```python
>>> f = open('test2.txt', 'w')
>>> judge = JudgeLight()
>>> judge.stdout = f.fileno()  # 重定向程序的输出流到文件
```

### 交互式评测
交互式评测可以通过同时开启两个JudgeLight，并对接他们的输入与输出来实现。评测姬可以通过交互评测程序的stderr流获得一些反馈信息，并且根据最终的返回值来确定题目的正确与否。  


## 其他问题

- **内存限制是个大坑**
- **内存测量是个大坑**
- **和内存相关的都是大坑**
- **我也是个大坑**
- 在macOS下，内存限制基本无效，只能等待运行结束或者超时然后判断使用内存是否超出<del>（大概率是因为我太菜了而有哪里写错了）</del><del>（发现在Ubuntu系统下限制正常，但是Ubuntu系统下测量出来的结果与我预料的差异较大，因此内存测量依旧是一个大坑）</del>（是我的程序写炸了，其实还算正常）
- **在校学习、还有这么多事情的情况下，真的是很难做到每天都敲代码......好多的设想也都没有时间来实现。**
- **是不是应该少承担点事情呢......**

## 下一步（设想）
- <del>交互式评测</del>
- <del>修改JudgeLight的子进程创建时间为run的时候（现在在创建类对象的时候就会fork子进程，讲道理使用起来会有点怪）</del>
- <del>提供更加合理的使用接口（现在的时间限制和重定向有些不合常理的用法）</del>
- 整理出成型的OJ评测姬（我的初步设想是为每个题目创建自己的评测程序，以此获得最大限度的自由和灵活。至于效率......再说吧）
- 其他......