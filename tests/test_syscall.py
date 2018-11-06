import os
import unittest

args = {
    'c': {
        'compile': 'gcc {file} -o {filename}',
        'run': './{filename}',
        'clean': 'rm {filename}'
    },
    'cpp': {
        'compile': 'g++ {file} -o {filename}',
        'run': './{filename}',
        'clean': 'rm {filename}'
    },
    'py': {
        'compile': 'echo > /dev/null',
        'run': 'python3 {file}',
        'clean': 'echo > /dev/null'
    }
}

class TestSyscall(unittest.TestCase):

    def test_allow(self):
        os.chdir('allow')
        allow_file_list = os.listdir('./')

        for file in allow_file_list:
            print('----------------------------------------------------------------------')
            filename, file_type = file.rsplit('.', 1)

            if not args.get(file_type):
                pass

            os.system(args[file_type]['compile'].format(filename=filename, file=file))

            print(file)
            print()
            code = os.system('../test_syscall ' + args[file_type]['run'].format(filename=filename, file=file))
            self.assertEqual(0, code)

            os.system(args[file_type]['clean'].format(filename=filename, file=file))

            print('----------------------------------------------------------------------\n')
        os.chdir('../')

    def test_deny(self):
        os.chdir('deny')
        deny_file_list = os.listdir('./')

        for file in deny_file_list:
            print('----------------------------------------------------------------------')
            if '.' not in file:
                continue
            filename, file_type = file.rsplit('.', 1)

            if not args.get(file_type):
                pass

            os.system(args[file_type]['compile'].format(filename=filename, file=file))

            print(file)
            print()
            code = os.system('../test_syscall ' + args[file_type]['run'].format(filename=filename, file=file))
            self.assertTrue(0 != code)

            os.system(args[file_type]['clean'].format(filename=filename, file=file))

            print('----------------------------------------------------------------------\n')
        os.chdir('../')

if __name__ == '__main__':
    os.system('gcc test_syscall.c -lseccomp -o test_syscall')
    unittest.main()
    os.system('rm test_syscall')
