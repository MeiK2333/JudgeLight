import os
import unittest

from JudgeLight import JudgeLight

from utils import compile_it, clear_it


class TestCore(unittest.TestCase):

    def test_echo(self):
        jl = JudgeLight(
            '/bin/echo', exec_args=['/bin/echo', 'Hello', 'World'], output_file_path='output.txt')
        stats = jl.run()
        self.assertEqual(stats['signum'], 0)
        self.assertEqual(stats['re_flag'], 0)
        with open('output.txt') as fr:
            self.assertEqual(fr.read().strip(), 'Hello World')
        os.remove('output.txt')

    def test_a_add_b(self):
        code = '''
#include <stdio.h>
int main() {
    int a, b;
    scanf("%d %d", &a, &b);
    printf("%d\\n", a + b);
    return 0;
}
'''
        compile_it(code)

        in_data = '1 2'
        with open('input.txt', 'w') as fw:
            fw.write(in_data)

        jl = JudgeLight('./a.out', input_file_path='input.txt',
                        output_file_path='output.txt')
        jl.run()
        with open('output.txt') as fr:
            self.assertEqual(fr.read().strip(), '3')

        clear_it()
        os.remove('input.txt')
        os.remove('output.txt')

    def test_signum(self):
        code = '''
#include <stdio.h>
int main() {
    return 1;
}
'''
        compile_it(code)

        jl = JudgeLight('./a.out')
        stats = jl.run()

        self.assertEqual(stats['signum'], 1)

        clear_it()

    def test_re(self):
        code = '''
#include <stdio.h>
int main() {
    int a = 0 / 0;
}
'''
        compile_it(code)

        jl = JudgeLight('./a.out')
        stats = jl.run()

        self.assertEqual(stats['re_flag'], 1)

        clear_it()


if __name__ == '__main__':
    unittest.main()
