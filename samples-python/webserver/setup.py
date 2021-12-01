from setuptools import setup

setup(name = 'sdk-py-webserver',
      version='2.0.0',
      description = 'Webserver Sample written in Python for ctrlX',
      author = 'SDK Team',
      install_requires=['PyJWT','ctrlx-datalayer', 'ctrlx-fbs'], 
      scripts = ['main.py'],
      packages = ['app','web'],
      license = 'Copyright (c) 2020-2021 Bosch Rexroth AG, Licensed under MIT License'
)
