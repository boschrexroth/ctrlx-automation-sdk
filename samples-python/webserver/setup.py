from setuptools import setup

setup(name='sdk-py-webserver',
      version='2.3.0',
      description='Webserver Sample written in Python for ctrlX',
      author='SDK Team',
      install_requires=['systemd','PyJWT', 'ctrlx-datalayer', 'ctrlx-fbs'],
      scripts=['main.py'],
      packages=['app', 'web'],
      license='Copyright (c) 2020-2022 Bosch Rexroth AG, Licensed under MIT License'
      )
