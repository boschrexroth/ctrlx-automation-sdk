from setuptools import setup

setup(name = 'sdk-py-provider-alldata',
      version='2.4.0',
      description = 'From setup.py: ctrlX Data Layer provider in Python publishing nodes of all data types',
      author = 'SDK Team',
      packages = ['alldataprovider', 'helper'],
      install_requires = ['ctrlx-datalayer', 'ctrlx-fbs'],
      scripts = ['main.py'],
      license = 'Copyright (c) 2020-2022 Bosch Rexroth AG, Licensed under MIT License'
)
