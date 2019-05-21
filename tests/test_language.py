import os
import unittest

from JudgeLight import JudgeLight
from utils import clear_it, compile_it


class TestLanguage(unittest.TestCase):

    def test_python(self):
        code = '''
import time
time.sleep(1)
print('Hello World!')
'''
        with open('main.py', 'w') as fw:
            fw.write(code)
        jl = JudgeLight(
            '/usr/local/bin/python', ['/usr/local/bin/python', 'main.py'],
            allow_system_calls_rule='default',
            output_file_path='output.txt',
        )
        stats = jl.run()
        with open('output.txt') as fr:
            data = fr.read()

        self.assertEqual(stats['re_syscall'], -1)
        self.assertEqual(data.strip(), 'Hello World!')
        self.assertEqual(stats['signum'], 0)
        self.assertEqual(stats['re_flag'], 0)

    def test_java(self):
        code = '''
public class Main
{
    public static void main(String[] args)
    {
        System.out.println("Hello World!");
    }
}
'''
        with open('Main.java', 'w') as fw:
            fw.write(code)
        jl = JudgeLight(
            '/usr/bin/javac', ['Main.java']
        )
        jl.run()

        jl = JudgeLight(
            '/usr/bin/java', ['Main'],
            allow_system_calls_rule='default',
            output_file_path='output.txt',
        )
        stats = jl.run()
        with open('output.txt') as fr:
            data = fr.read()

        self.assertEqual(stats['re_syscall'], -1)
        self.assertEqual(data.strip(), 'Hello World!')
        self.assertEqual(stats['signum'], 0)
        self.assertEqual(stats['re_flag'], 0)

if __name__ == '__main__':
    unittest.main()
