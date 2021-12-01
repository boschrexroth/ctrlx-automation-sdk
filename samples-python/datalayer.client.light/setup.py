from setuptools import setup

setup(name = 'sdk-py-client-light',
      version='2.0.0',
      description = 'Lightweight Data Layer client',
      author = 'SDK Team',
      install_requires = ['ctrlx-datalayer', 'ctrlx-fbs'],
      scripts = ['main.py'],
      license = 'Copyright (c) 2021 Bosch Rexroth AG, Licensed under MIT License'
)
