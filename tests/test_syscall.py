import os
import unittest

from JudgeLight import JudgeLight

from utils import clear_it, compile_it


class TestSyscall(unittest.TestCase):

    def test_hello(self):
        # pass
        code = '''
#include <stdio.h>

int main() {
    printf("Hello World!\\n");
    return 0;
}
'''
        compile_it(code)

        jl = JudgeLight('./a.out', allow_system_calls_rule='default',
                        output_file_path='output.txt')
        stats = jl.run()
        self.assertEqual(stats['re_flag'], 0)
        with open('output.txt') as fr:
            self.assertEqual(fr.read().strip(), 'Hello World!')

        clear_it()
        os.remove('output.txt')

    def test_malloc(self):
        # pass
        code = '''
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *a = (int *) malloc(sizeof(int));
    *a = 1;
    return *a;
}
'''
        compile_it(code)

        jl = JudgeLight('./a.out', allow_system_calls_rule='default')
        stats = jl.run()
        self.assertEqual(stats['re_flag'], 0)

        clear_it()

    def test_new(self):
        # pass
        code = '''
#include <iostream>
using namespace std;

int main() {
    int *a = new int;
    *a = 2;
    return *a;
}
'''
        compile_it(code)

        jl = JudgeLight('./a.out', allow_system_calls_rule='default')
        stats = jl.run()
        self.assertEqual(stats['re_flag'], 0)

        clear_it()

    def test_python(self):
        # pass
        code = 'print("Hello World!")'
        with open('code.py', 'w') as fw:
            fw.write(code)
        jl = JudgeLight('/usr/bin/python3', exec_args=[
                        '/usr/bin/python3', 'code.py'], allow_system_calls_rule='python', output_file_path='output.txt')
        stats = jl.run()

        self.assertEqual(stats['re_flag'], 0)
        with open('output.txt') as fr:
            self.assertEqual(fr.read().strip(), 'Hello World!')
        os.remove('code.py')
        os.remove('output.txt')

    def test_exec(self):
        # nopass
        code = '''
#include <unistd.h>

int main() {
    char *const args[] = {"echo", "Hello", NULL};
    execvp("echo", args);
    return 0;
}
'''
        compile_it(code)

        jl = JudgeLight('./a.out', allow_system_calls_rule='default')
        stats = jl.run()

        self.assertEqual(stats['re_flag'], 1)
        self.assertTrue(stats['re_syscall'] != 0 and stats['re_syscall'] != -1)

        clear_it()

    def test_fork(self):
        # nopass
        code = '''
#include <unistd.h>

int main() {
    int pid = fork();
    return 0;
}
'''
        compile_it(code)

        jl = JudgeLight('./a.out', allow_system_calls_rule='default')
        stats = jl.run()

        self.assertEqual(stats['re_flag'], 1)
        self.assertTrue(stats['re_syscall'] != 0 and stats['re_syscall'] != -1)

        clear_it()

    def test_system(self):
        # nopass
        code = '''
#include <stdlib.h>

int main() {
    system("echo Hello World");
}
'''
        compile_it(code)
        jl = JudgeLight('./a.out', allow_system_calls_rule='default',
                        output_file_path='output.txt')
        stats = jl.run()

        self.assertEqual(stats['re_flag'], 1)
        with open('output.txt') as fr:
            self.assertEqual(fr.read().strip(), '')

        clear_it()
        os.remove('output.txt')

    def test_requests(self):
        # nopass
        code = 'import requests'
        with open('code.py', 'w') as fw:
            fw.write(code)
        jl = JudgeLight('/usr/bin/python3', exec_args=[
                        '/usr/bin/python3', 'code.py'], allow_system_calls_rule='python')
        stats = jl.run()
        self.assertEqual(stats['re_flag'], 1)
        self.assertTrue(stats['re_syscall'] != 0 and stats['re_syscall'] != -1)

        os.remove('code.py')

    def test_openat(self):
        # nopass
        code = '''
#include <fcntl.h>

int main() {
    openat(0, "~/JudgeLight/tests/input.txt", O_CREAT | O_RDWR);
    return 0;
}
'''
        compile_it(code)

        jl = JudgeLight('./a.out', allow_system_calls_rule='default')
        stats = jl.run()

        self.assertEqual(stats['re_flag'], 1)
        self.assertTrue(stats['re_syscall'] != 0 and stats['re_syscall'] != -1)

        clear_it()


if __name__ == '__main__':
    unittest.main()
