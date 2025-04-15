# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT
from setuptools import setup

setup(
    name="sdk-py-client-light",
    version="1.0.0",
    description="Lightweight ctrlX Data Layer client",
    author="SDK Team",
    install_requires=["ctrlx-datalayer<=3.5", "ctrlx-fbs"],
    packages=["helper"],
    scripts=["main.py"],
    license="MIT License",
)
