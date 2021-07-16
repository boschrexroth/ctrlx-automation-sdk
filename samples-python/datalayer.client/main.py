#!/usr/bin/env python3

# MIT License
#
# Copyright (c) 2020-2021 Bosch Rexroth AG
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

from app import call_datalayer_client
import os
import faulthandler
import time

import datalayer
from datalayer.variant import Result

from app.call_datalayer_client import CallDataLayerClient

if __name__ == '__main__':

    faulthandler.enable()

    # This is the connection string for TCP in the format: tcp://USER:PASSWORD@IP_ADDRESS:PORT
    # Please check and change according your environment:
    # - USER:       Enter your user name here - default is boschrexroth
    # - PASSWORD:   Enter your password here - default is boschrexroth
    # - IP_ADDRESS: 127.0.0.1   If you develop in WSL and you want to connect to a ctrlX CORE virtual with port forwarding
    #               10.0.2.2    If you develop in a VM (Virtual Box, QEMU,...) and you want to connect to a ctrlX virtual with port forwarding
    #               192.168.1.1 If you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater

    connectionClient = "tcp://boschrexroth:boschrexroth@127.0.0.1:2069"

    if 'SNAP' in os.environ:
        connectionClient = "ipc://"

    print()
    print("========================================================================")
    print("rexroth-python-client - A ctrlX Data Layer Client App in Python")
    print()
    print("These functions are used synchronously and asynchronously:")
    print("  ping_..")
    print("  read_..")
    print("  create_..")
    print("  remove_..")
    print("  browse_..")
    print("  write_..")
    print("  metadata_..")
    print()
    print("Precondition:")
    print("Build and install the snap 'alldataprovider' from the SDK folder")
    print("sdk.control/public/samples-cpp/datalayer.provider.all-data")
    print("========================================================================")
    print()

    with datalayer.system.System("") as datalayer_system:
        print("INFO Starting Data Layer system")
        datalayer_system.start(False)

        print("INFO Creating Data Layer Client connection to ctrlX with", connectionClient)
        with datalayer_system.factory().create_client(connectionClient) as datalayer_client:

            print("INFO Pausing while connection will be established")
            time.sleep(1.0)

            print("INFO Testing connection")
            while datalayer_client.is_connected() == False:
                print()
                print("ERROR Data Layer Client is - NOT - connected")
                print("INFO Ensure that ctrlX is running and in Operation Mode")
                print("INFO Sleeping for 5s...")
                print()
                time.sleep(5.0)
                
            print("INFO Setting Data Layer Client timeout")
            result = datalayer_client.set_timeout(datalayer.client.TimeoutSetting.PING, 5000)
            if result != Result.OK:
                print("ERROR Setting Data Layer Client timeout failed with", result)

            print("INFO Creating Python Data Layer Client instance")
            call_datalayer_client = CallDataLayerClient(datalayer_client)

            while datalayer_client.is_connected():
                call_datalayer_client.run()
                print(
                    "Sleeping --------------------------------------------------------------------------")
                time.sleep(1.0)

            print("ERROR Data Layer is NOT connected")

        print("INFO Stopping Data Layer system")
        datalayer_system.stop(True)
