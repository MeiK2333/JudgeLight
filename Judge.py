# coding=utf-8
import os
import time
import judgelight


class Result(object):
    ''' 另一个比较狗的类 '''

    def __init__(self, cmd, time_limit, memory_limit, result):
        self.cmd = cmd
        self.time_limit = time_limit
        self.memory_limit = memory_limit

        self.ru_utime = result['ru_utime']
        self.ru_stime = result['ru_stime']
        self.ru_maxrss = result['ru_maxrss']
        self.ru_ixrss = result['ru_ixrss']
        self.ru_idrss = result['ru_idrss']
        self.ru_isrss = result['ru_isrss']
        self.ru_minflt = result['ru_minflt']
        self.ru_majflt = result['ru_majflt']
        self.ru_nswap = result['ru_nswap']
        self.ru_inblock = result['ru_inblock']
        self.ru_oublock = result['ru_oublock']
        self.ru_msgsnd = result['ru_msgsnd']
        self.ru_msgrcv = result['ru_msgrcv']
        self.ru_nsignals = result['ru_nsignals']
        self.ru_nvcsw = result['ru_nvcsw']
        self.ru_nivcsw = result['ru_nivcsw']
        self.time_used = result['time_used']
        self.memory_used = result['memory_used']
        self.status = result['status']
        self.error = result['error']
        self.reason = result['reason']
        self.reason_str = 'None'

        if self.error:  # 整理错误类型
            if self.reason == 'SIGSEGV':
                if self.memory_limit != -1 and self.memory_used > self.memory_limit:
                    self.reason_str = 'Memory Limit Exceeded'
                else:
                    self.reason_str = 'Memory Limit Exceeded or Runtime Error'
            elif self.reason == 'SIGALRM' or self.reason == 'SIGXCPU':
                self.reason_str = 'Time Limit Exceeded'
            else:
                self.reason_str = 'Runtime Error'
        elif self.time_limit != -1 and self.time_used > self.time_limit:  # 如果同时mle和tle 那么显示为mle 没有为啥 就是随便一写
            self.reason_str = 'Time Limit Exceeded'
        elif self.memory_limit != -1 and self.memory_used > self.memory_limit:
            self.reason_str = 'Memory Limit Exceeded'

    def __str__(self):
        return '''
cmd:\t\t{}
time_limit:\t{}
memory_limit:\t{}
time_used:\t{}
memory_used:\t{}
error:\t\t{}
reason:\t\t{}
    '''.format(self.cmd, self.time_limit, self.memory_limit, self.time_used, self.memory_used, self.error, self.reason_str)


class JudgeLight(object):
    ''' 一个比较狗的类 '''

    def __init__(self):
        '''
        创建子进程
        '''
        self.pid, self.operin, self.operout = judgelight.init()
        self._time_limit = -1
        self._memory_limit = -1

    def _send(self, msg):
        os.write(self.operin, msg + '\0')
        os.read(self.operout, 10)

    @property
    def stdin(self):
        return self._stdin

    @stdin.setter
    def stdin(self, fd):
        ''' 设置输入流 '''
        assert type(fd) == int
        self._send('fd')
        self._send('in')
        self._send(str(fd))
        self._stdin = fd

    @property
    def stdout(self):
        return self._stdout

    @stdout.setter
    def stdout(self, fd):
        ''' 设置输出流 '''
        assert type(fd) == int
        self._send('fd')
        self._send('out')
        self._send(str(fd))
        self._stdout = fd

    @property
    def stderr(self):
        return self._stderr

    @stderr.setter
    def stderr(self, fd):
        ''' 设置错误流 '''
        assert type(fd) == int
        self._send('fd')
        self._send('err')
        self._send(str(fd))
        self._stderr = fd

    def set_real_time(self, time):
        ''' 为子进程设置真实时间 '''
        self._send('limit')
        self._send('time')
        self._send(str(time))

    def set_cpu_time(self, time):
        ''' 为子进程设置CPU时间 '''
        self._send('limit')
        self._send('cpu')
        self._send(str(time))

    @property
    def time_limit(self):
        return self._time_limit

    @time_limit.setter
    def time_limit(self, value):
        ''' 设置时间限制（单位：ms） '''
        assert type(value) == int
        self.set_cpu_time(value)
        self.set_real_time(value + 1000)
        self._time_limit = value

    def set_data(self, value):
        ''' 设置数据段长度限制 '''
        self._send('limit')
        self._send('data')
        self._send(str(value))

    def set_as(self, value):
        ''' 设置进程可用储存空间最大长度 '''
        self._send('limit')
        self._send('as')
        self._send(str(value))

    def fd_close(self, fd):
        ''' 关闭指定的文件描述符 '''
        self._send('fd')
        self._send('close')
        self._send(str(fd))

    @property
    def memory_limit(self):
        return self._memory_limit

    @memory_limit.setter
    def memory_limit(self, value):
        ''' 设置内存限制（单位：KB） '''
        assert type(value) == int
        self.set_data(value)
        self.set_as(value * 2)
        self._memory_limit = value

    def run(self, cmd):
        ''' runit '''
        self._send('run')
        args = cmd.split()
        self._send(str(len(args)))
        for arg in args:
            self._send(str(len(arg)))
            self._send(arg)
        self._send('go')
        time.sleep(0.01)
        self.result = judgelight.run(self.pid)
        try:
            self.close()
        except:
            self.close()
        return Result(cmd, self.time_limit, self.memory_limit, self.result)

    def close(self):
        ''' 清理文件描述符和其他的一些操作 '''
        os.close(self.operin)
        os.close(self.operout)
