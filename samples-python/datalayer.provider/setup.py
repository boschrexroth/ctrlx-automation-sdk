# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT
from setuptools import setup

setup(
    name='sdk-py-datalayer-provider',
    version='2.4.0',
    description='This sample shows how to provide data to ctrlX Data Layer',
    author='SDK Team',
    install_requires = ['ctrlx-datalayer', 'ctrlx_fbs'],    
    packages=['app', 'helper', 'sample.schema'],
    # https://stackoverflow.com/questions/1612733/including-non-python-files-with-setup-py
    package_data={'./': ['sampleSchema.bfbs']},
    scripts=['main.py'],
    license='MIT License'
)
