# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT
from setuptools import setup

setup(name = 'sdk-py-client-bulk',
      version='1.0.0',
      description = 'ctrlX Data Layer client bulk operations',
      author = 'SDK Team',
      install_requires = ['cysystemd', 'ctrlx-fbs', 'ctrlx_datalayer' ],
      packages = ['helper'],
      scripts = ['main.py'],
      license = 'MIT License'
)
