# coding=utf-8
from flask import Flask

from config import SYSTEM_CONFIG

app = Flask(__name__)

if __name__ == '__main__':
    app.run(debug=True, host=SYSTEM_CONFIG['flask_host'], port=SYSTEM_CONFIG['flask_port'])
