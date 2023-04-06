from setuptools import setup


setup(name = 'sdk-py-logbook',
      version='2.3.0',
      description = 'Logbook sample written in Python for ctrlX',
      author = 'SDK Team',
      packages = ['logbook'],
      install_requires=['systemd'], 
      scripts = ['main.py'],
      license = 'Copyright (c) 2021 Bosch Rexroth AG, Licensed under MIT License'
)
