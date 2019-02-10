import os
import unittest

from JudgeLight import JudgeLight

from utils import clear_it, compile_it


class TestLimit(unittest.TestCase):

    def test_cpu_time_limit(self):
        code = '''
int main() {
    while (1);
}
'''
        compile_it(code)

        jl = JudgeLight('./a.out', time_limit=1000)
        stats = jl.run()

        self.assertEqual(stats['re_flag'], 1)
        # CPU 时间限制只能精确到秒，因此这里放宽限制
        self.assertTrue(1000 <= stats['time_used'] <= 3000)

        clear_it()

    def test_real_time_limit(self):
        code = '''
#include <unistd.h>

int main() {
    sleep(10);
}
'''
        compile_it(code)

        jl = JudgeLight('./a.out', real_time_limit=1000)
        stats = jl.run()

        self.assertEqual(stats['re_flag'], 1)
        self.assertTrue(1000 <= stats['real_time_used'] <= 1500)

        clear_it()

    def test_output_size_limit(self):
        code = '''
#include <stdio.h>

int main() {
    for (int i = 0; i < 1000000; i++) {
        printf("Hello World!\\n");
    }
    return 0;
}
'''
        compile_it(code)

        jl = JudgeLight('./a.out', output_size_limit=10240,
                        output_file_path='output.txt')
        stats = jl.run()

        self.assertEqual(stats['re_flag'], 1)
        with open('output.txt') as fr:
            l = len(fr.read())
            self.assertTrue(l == 10240)

        clear_it()
        os.remove('output.txt')

    def test_memory_limit(self):
        code = '''
#include <stdio.h>
#include <stdlib.h>

#define MAX 2000000

int *a[MAX];

int main(){
    for (int i = 0; i < MAX; i++) {
        a[i] = (int *) malloc(sizeof(int));
    }
    for (int i = 0; i < MAX; i++) {
        *a[i] = i;
    }
    int ans = 0;
    for (int i = 0; i < MAX; i++) {
        ans ^= *a[i];
    }
    printf("%d\\n", ans);
    return 0;
}
'''
        compile_it(code)

        jl = JudgeLight('./a.out', output_file_path='output.txt',
                        memory_limit=65535, trace=False)
        stats = jl.run()

        self.assertEqual(stats['re_flag'], 1)

        clear_it()
        os.remove('output.txt')


if __name__ == '__main__':
    unittest.main()
