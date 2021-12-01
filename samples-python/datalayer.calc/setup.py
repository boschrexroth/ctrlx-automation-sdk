from setuptools import setup

import calculations

setup(name = 'sdk-py-calc',
      version = calculations.__version__,
      description = calculations.__description__,
      author = calculations.__author__,
      # author_email = calculations.__author_email__,
      # url = calculations.__homepage__,
      packages = ['calculations'],
      install_requires=['ctrlx-datalayer', 'ctrlx-fbs'], 
      scripts = ['main.py'],
      license = calculations.__copyright__ + \
                ', Licensed under ' + calculations.__licence__
)
