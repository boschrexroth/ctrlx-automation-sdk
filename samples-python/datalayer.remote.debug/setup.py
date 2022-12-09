from setuptools import setup

setup(name = 'sdk-py-remote-debug',
      version = '2.4.0',
      description = 'Data Layer client in Python, enables remote debugging on a ctrlX',
      author = 'SDK Team',
      install_requires=['debugpy', 'ctrlx-datalayer', 'ctrlx-fbs'], 
      packages = ['helper'],
      scripts = ['main.py', 'debugging.py'],
      license = 'Copyright (c) 2021-2022 Bosch Rexroth AG, Licensed under MIT License'
)
