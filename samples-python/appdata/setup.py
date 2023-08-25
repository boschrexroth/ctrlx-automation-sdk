# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT
from setuptools import setup

setup(name = 'sdk-py-appdata',
      version='3.0.0',
      description = 'This sample demonstrates how to persist your application data in ctrlX CORE',
      author = 'SDK Team',
      packages = ['appdata','web'],
      install_requires=['PyJWT', 'ctrlx-datalayer', 'ctrlx-fbs'], 
      scripts = ['main.py'],
      license = 'MIT License'
)
