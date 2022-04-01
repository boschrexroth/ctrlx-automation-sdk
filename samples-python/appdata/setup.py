from setuptools import setup

setup(name='sdk-py-appdata',
      version='2.0.0',
      description='This sample demonstrates how to persist your application data in ctrlX environment',
      author='SDK Team',
      packages=['appdata', 'web'],
      install_requires=['PyJWT'],
      scripts=['main.py'],
      license='Copyright (c) 2021-2022 Bosch Rexroth AG, Licensed under MIT License'
      )
