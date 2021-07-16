from distutils.core import setup

setup(
      name="sdk-py-datalayer-provider",
      version="1.0.2",
      description="This sample shows how to provide data to ctrlX Data Layer",
      author="SDK Team",
      packages=['app', 'sample.schema'],
      # https://stackoverflow.com/questions/1612733/including-non-python-files-with-setup-py
      package_data={'./': ['sampleSchema.bfbs']},
      scripts=['main.py'],
      license="Copyright (c) 2020-2021 Bosch Rexroth AG, Licensed under MIT License"
 )
