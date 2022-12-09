from setuptools import setup

setup(name = 'sdk-py-client-bulk',
      version='1.0.0',
      description = 'Data Layer client bulk operations',
      author = 'SDK Team',
      install_requires = ['systemd', 'ctrlx-fbs', 'ctrlx_datalayer' ],
      packages = ['helper'],
      scripts = ['main.py'],
      license = 'Copyright (c) 2022 Bosch Rexroth AG, Licensed under MIT License'
)
