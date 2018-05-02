# coding=utf-8
import json


class Result(object):

    def __init__(self):
        self.message = ''

        self.run = []

        self.compile = {
            'time_used': -1,
            'memory_used': -1,
            'exit_code': -1,
            'signal': -1,
            'message': '',
        }

        self.in_validator = {
            'time_used': -1,
            'memory_used': -1,
            'exit_code': -1,
            'signal': -1,
            'message': '',
        }

        self.out_validator = {
            'time_used': -1,
            'memory_used': -1,
            'exit_code': -1,
            'signal': -1,
            'message': '',
        }

    def exit(self):
        print(self.__str__())
        exit(0)

    def __str__(self):
        data = {
            'message': self.message,
            'run': self.run,
            'compile': self.compile,
            'in_validator': self.in_validator,
            'out_validator': self.out_validator,
        }
        return json.dumps(data, ensure_ascii=False, indent=4)
