from setuptools import setup

setup(name = 'sdk-py-browse',
      version='2.4.0',
      description = 'This sample demonstrates how to browse a Data Layer tree',
      author = 'SDK Team',
      install_requires = ['ctrlx-datalayer', 'ctrlx-fbs'],
      packages = ['helper'],
      scripts = ['main.py'],
      license = 'Copyright (c) 2021-2022 Bosch Rexroth AG, Licensed under MIT License'
)
