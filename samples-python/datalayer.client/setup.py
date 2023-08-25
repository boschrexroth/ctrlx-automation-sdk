# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT
from setuptools import setup


setup(name = 'sdk-py-client',
      version = '2.4.0',
      description = 'ctrlX Data Layer client sample written in Python for ctrlX',
      author = 'SDK Team',
      packages = ['app', 'helper'],
      install_requires=['ctrlx-datalayer', 'ctrlx-fbs'], 
      scripts = ['main.py'],
      license = 'MIT License'
)
