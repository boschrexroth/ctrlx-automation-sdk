# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT
from setuptools import setup

import calculations

setup(name = 'sdk-py-calc',
      version = calculations.__version__,
      description = calculations.__description__,
      author = calculations.__author__,
      packages = ['calculations', 'helper'],
      install_requires=['ctrlx-datalayer<=3.5', 'ctrlx-fbs'],
      scripts = ['main.py'],
      license = calculations.__copyright__ + \
                ', Licensed under ' + calculations.__licence__
)
