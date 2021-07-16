from distutils.core import setup

setup(name = "sdk-py-webserver",
      version = "1.0.2",
      description = "Webserver Sample written in Python for ctrlX",
      author = "SDK Team",
      scripts = ['main.py'],
      packages = ['app','web'],
      license = "Copyright (c) 2020-2021 Bosch Rexroth AG, Licensed under MIT License"
)
