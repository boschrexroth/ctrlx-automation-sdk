# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT
from setuptools import setup

setup(name = 'sdk-py-client-simple',
      version='1.0.0',
      description = 'Simple ctrlX Data Layer client',
      author = 'SDK Team',
      install_requires = ['ctrlx-datalayer', 'ctrlx-fbs'],
      packages = ['helper'],
      scripts = ['main.py'],
      license = 'MIT License'
)
