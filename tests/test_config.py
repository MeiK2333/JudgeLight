# coding=utf-8
import sys
import unittest

from config import SYSTEM_CONFIG, JUDGER_CONFIG

sys.path.append('../')


class TestConfig(unittest.TestCase):
    def test_SYSTEM_CONFIG_type(self):
        self.assertTrue(isinstance(SYSTEM_CONFIG, dict))
        self.assertTrue(isinstance(SYSTEM_CONFIG.get('plunge_path'), str))
        self.assertTrue(isinstance(SYSTEM_CONFIG.get('work_dir'), str))
        self.assertTrue(isinstance(SYSTEM_CONFIG.get('max_judger_count'), int))
        self.assertTrue(isinstance(SYSTEM_CONFIG.get('flask_host'), str))
        self.assertTrue(isinstance(SYSTEM_CONFIG.get('flask_port'), int))
        self.assertTrue(isinstance(SYSTEM_CONFIG.get('redis_host'), str))
        self.assertTrue(isinstance(SYSTEM_CONFIG.get('redis_port'), str))
        self.assertTrue(isinstance(SYSTEM_CONFIG.get('redis_list'), str))
        self.assertTrue(isinstance(SYSTEM_CONFIG.get('redis_result'), str))

    def test_JUDGER_CONFIG_type(self):
        self.assertTrue(isinstance(JUDGER_CONFIG.get('compile_time_limit'), int))
        self.assertTrue(isinstance(JUDGER_CONFIG.get('compile_memory_limit'), int))
        self.assertTrue(isinstance(JUDGER_CONFIG.get('validator'), str))
        self.assertTrue(isinstance(JUDGER_CONFIG.get('validator_cmd'), str))
        self.assertTrue(isinstance(JUDGER_CONFIG.get('validator_time_limit'), int))
        self.assertTrue(isinstance(JUDGER_CONFIG.get('validator_memory_limit'), int))
        self.assertTrue(isinstance(JUDGER_CONFIG.get('data_dir'), str))
        self.assertTrue(isinstance(JUDGER_CONFIG.get('language'), dict))


if __name__ == '__main__':
    unittest.main()
