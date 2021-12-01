#!/usr/bin/env python3

# MIT License
#
# Copyright (c) 2021 Bosch Rexroth AG
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import os
import sys

import faulthandler
import time

import datalayer
from datalayer.variant import Result

from calculations.basic_arithmetic_operations import BasicArithmeticOperations

# Do NOT change these values
connection_ipc = "ipc://"
port_client = ":2069"
port_provider = ":2070"

addr_root = "sdk-py-calc"

# This is the connection string for TCP in the format: tcp://USER:PASSWORD@IP_ADDRESS
# Please check and change according your environment:
# - USER:       Enter your user name here - default is boschrexroth
# - PASSWORD:   Enter your password here - default is boschrexroth
# - IP_ADDRESS: 127.0.0.1   If you develop in WSL and you want to connect to a ctrlX CORE virtual with port forwarding
#               10.0.2.2    If you develop in a VM (Virtual Box, QEMU,...) and you want to connect to a ctrlX virtual with port forwarding
#               192.168.1.1 If you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater

connection_tcp = "tcp://boschrexroth:boschrexroth@10.0.2.2"


def is_snap() -> bool:
    return 'SNAP' in os.environ


def start_new_basic_arithmetic_operation(
        provider: datalayer.provider.Provider,
        client: datalayer.client.Client,
        id: str,
        mode: str):

    basicArithmeticOperation = BasicArithmeticOperations(
        provider, client, addr_root, id, mode)
    basicArithmeticOperation.register_nodes()

    while basicArithmeticOperation.subscribe() != Result.OK:
        basicArithmeticOperation.unsubscribe()
        print("WARN Starting Data Layer subsciptions for",
              addr_root + "/" + id, "failed with: " + str(result))
        print("INFO Retry in 5s")
        time.sleep(5.0)
    pass


if __name__ == '__main__':

    print()
    print("===========================================================================")
    print("ctrlX Application in Python:")
    print("- Reads Data Layer values per subscription.")
    print("- Runs a algorithmn")
    print("- Provides result as Data Layer Node")
    print()
    print("Will be restarted by the snap system on error.")
    print("===========================================================================")
    print()

    faulthandler.enable()

    system = datalayer.system.System("")
    system.start(False)

    if is_snap():
        connection_client = connection_ipc
        connection_provider = connection_ipc
    else:
        connection_client = connection_tcp + port_client
        connection_provider = connection_tcp + port_provider

    client = system.factory().create_client(connection_client)
    client.set_timeout(datalayer.client.TimeoutSetting.PING, 5000)

    provider = system.factory().create_provider(connection_provider)
    result = provider.start()
    if result != Result.OK:
        print("ERROR Starting Data Layer provider failed with: " + str(result))
        sys.exit(1)

    bao_plus = start_new_basic_arithmetic_operation(provider, client, "plus", "+")

    # Endless loop
    while client.is_connected():
        time.sleep(10.0)

    print("ERROR Data Layer client is disconnected - exiting application. Will be restarted automatically.")

    bao_plus.unsubscribe()

    client.close()

    system.stop(True)

    sys.exit(3)
