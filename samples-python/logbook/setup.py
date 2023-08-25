# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT
from setuptools import setup


setup(name = 'sdk-py-logbook',
      version='2.4.0',
      description = 'Logbook sample written in Python for ctrlX',
      author = 'SDK Team',
      packages = ['logbook'],
      install_requires=['cysystemd'], 
      scripts = ['main.py'],
      license = 'MIT License'
)
