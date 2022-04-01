from setuptools import setup

setup(name='sdk-py-remote-debug',
      version='2.1.0',
      description='ctrlX Data Layer client in Python, enables remote debugging on a ctrlX',
      author='SDK Team',
      install_requires=['debugpy'],
      packages = ['helper'],
      scripts=['main.py', 'debugging.py'],
      license='Copyright (c) 2021-2022 Bosch Rexroth AG, Licensed under MIT License'
      )
