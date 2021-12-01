from setuptools import setup

setup(name = 'sdk-py-browse',
      version='2.0.0',
      description = 'This sample demonstrates how to browse a Data Layer tree',
      author = 'SDK Team',
      install_requires = ['ctrlx-datalayer', 'ctrlx-fbs'],
      scripts = ['main.py'],
      license = 'Copyright (c) 2021 Bosch Rexroth AG, Licensed under MIT License'
)
