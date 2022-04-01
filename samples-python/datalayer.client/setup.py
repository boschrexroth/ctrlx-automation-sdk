from setuptools import setup


setup(name = 'sdk-py-client',
      version = '2.1.0',
      description = 'Data Layer client sample written in Python for ctrlX',
      author = 'SDK Team',
      packages = ['app', 'helper'],
      install_requires=['ctrlx-datalayer', 'ctrlx-fbs'], 
      scripts = ['main.py'],
      license = 'Copyright (c) 2020-2022 Bosch Rexroth AG, Licensed under MIT License'
)
