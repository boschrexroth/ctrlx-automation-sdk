# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

from setuptools import setup

setup(name = 'sdk-py-remote-debug',
      version = '3.0.0',
      description = 'ctrlX Data Layer client in Python, enables remote debugging on a ctrlX',
      author = 'SDK Team',
      install_requires=['debugpy', 'ctrlx-datalayer', 'ctrlx-fbs'], 
      packages = ['helper'],
      scripts = ['main.py', 'debug.py'],
      license = 'MIT License'
)
