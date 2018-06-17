# coding=utf-8
import json

import redis

from config import JUDGER_CONFIG, SYSTEM_CONFIG
from logger import logger

rdp = redis.ConnectionPool(host=SYSTEM_CONFIG['redis_host'], port=SYSTEM_CONFIG['redis_port'])
rdc = redis.StrictRedis(connection_pool=rdp)


class Judger(object):
    def __init__(self, pid, runid, code, language, time_limit, memory_limit, config=None, extend=None):
        if extend is None:
            extend = {}
        if config is None:
            config = {}

        self.config = JUDGER_CONFIG.copy().update(config)
        self.pid = pid
        self.runid = runid
        self.code = code
        self.language = language
        self.time_limit = time_limit
        self.memory_limit = memory_limit
        self.extend = extend

    def update(self):
        """
        更新评测状态
        :return:
        """
        pass

    @staticmethod
    def get():
        """
        获取一个待评测的 Judger
        :return: Judger 实例
        """
        task = rdc.blpop(SYSTEM_CONFIG['redis_list'])
        data = bytes.decode(task[1])

        # 将 json 数据解析
        try:
            data = json.loads(data)
        except json.JSONDecodeError:
            logger.warning('json decode error: "{}"'.format(data))
            return None
        except Exception as e:
            logger.warning('unknown error: "{}"'.format(repr(e)))
            return None

        # 拿出所有数据
        try:
            pid = data['pid']
            runid = data['runid']
            code = data['code']
            language = data['language']
            time_limit = int(data['time_limit'])
            memory_limit = int(data['memory_limit'])
            config = data.get('config')
            extend = data.get('extend')
        except KeyError or ValueError or TypeError:
            logger.warning('args parse error: "{}"'.format(data))
            return None
        except Exception as e:
            logger.warning('unknown error: "{}"'.format(repr(e)))
            return None

        judger = Judger(pid=pid, runid=runid, code=code, language=language, time_limit=time_limit,
                        memory_limit=memory_limit, config=config, extend=extend)
        return judger


class Result(object):
    pass


class TestCase(object):
    pass
