# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT
from setuptools import setup

setup(name='sdk-py-webserver',
      version='2.3.0',
      description='Webserver Sample written in Python for ctrlX',
      author='SDK Team',
      install_requires=['cysystemd','PyJWT', 'ctrlx-datalayer', 'ctrlx-fbs'],
      scripts=['main.py'],
      packages=['app', 'web'],
      license='MIT License'
      )
