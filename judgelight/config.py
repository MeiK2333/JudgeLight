# coding=utf-8

import os

class Config(object):
    plunge_path = '../plunge'

if not os.path.exists(Config.plunge_path):
    print("Please make sure the file plunge exists")
    exit(1)