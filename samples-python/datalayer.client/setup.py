from setuptools import setup


setup(name = 'sdk-py-client',
      version = '2.0.0',
      description = 'Data Layer client sample written in Python for ctrlX',
      author = 'SDK Team',
      packages = ['app'],
      install_requires=['ctrlx-datalayer', 'ctrlx-fbs'], 
      scripts = ['main.py'],
      license = 'Copyright (c) 2020-2021 Bosch Rexroth AG, Licensed under MIT License'
)
