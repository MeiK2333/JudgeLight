# coding=utf-8
import os
import json
import time
import judgelight


class Result(object):

    def __init__(self, result):
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


class JudgeLight(object):
    ''' 一个比较狗的类 '''

    def __init__(self):
        '''
        创建子进程
        '''
        self.pid, self.operin, self.operout = judgelight.init()

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
        return Result(self.result)

    def close(self):
        ''' 清理文件描述符和其他的一些操作 '''
        os.close(self.operin)
        os.close(self.operout)

def test(cmd):
    s = JudgeLight()
    s.run(cmd)
    print json.dumps(s.result, indent=2)

if __name__ == '__main__':
    test('g++ test1.cpp')
    test('./a.out')