from setuptools import setup

setup(name = 'sdk-py-client-light',
      version='2.4.0',
      description = 'Lightweight Data Layer client',
      author = 'SDK Team',
      install_requires = ['ctrlx-datalayer', 'ctrlx-fbs'],
      packages = ['helper'],
      scripts = ['main.py'],
      license = 'Copyright (c) 2021-2022 Bosch Rexroth AG, Licensed under MIT License'
)
