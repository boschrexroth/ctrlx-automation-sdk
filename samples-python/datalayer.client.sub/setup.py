# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT
from setuptools import setup

setup(name = 'sdk-py-client-sub',
      version='2.4.0',
      description = 'This sample demonstrates how to use ctrlX Data Layer subscriptions with Python',
      author = 'SDK Team',
      install_requires = ['ctrlx-datalayer', 'ctrlx-fbs'],
      packages = ['datalayerclient', 'helper'],
      scripts = ['main.py'],
      license = 'MIT License'
)
