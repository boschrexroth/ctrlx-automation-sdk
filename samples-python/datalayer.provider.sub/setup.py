# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT
from setuptools import setup

setup(
    name='sdk-py-datalayer-provider-sub',
    version='1.0.0',
    description='This sample shows how to provide data and subscription interface to ctrlX Data Layer',
    author='SDK Team',
    install_requires = ['ctrlx-datalayer>=3.3', 'ctrlx_fbs'],
    packages=['datalayerprovider', 'datalayerclient', 'helper',],
    # https://stackoverflow.com/questions/1612733/including-non-python-files-with-setup-py
    scripts=['provider.py', 'client.py'],
    license='MIT License'
)
