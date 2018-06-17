# coding=utf-8
import logging
import sys


def set_logging():
    _logger = logging.getLogger("Judger")
    formatter = logging.Formatter('%(asctime)s %(levelname)-8s: %(message)s')
    file_handler = logging.FileHandler("judger.log")
    file_handler.setFormatter(formatter)
    console_handler = logging.StreamHandler(sys.stdout)
    console_handler.formatter = formatter
    _logger.addHandler(file_handler)
    _logger.addHandler(console_handler)
    _logger.setLevel(logging.INFO)
    _logger.setLevel(logging.DEBUG)
    return _logger


logger = set_logging()
