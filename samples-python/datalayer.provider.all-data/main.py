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
import time

import datalayer

from datalayerprovider.providerNodeAllData import ProviderNodeAllData

def main(argv):
    arg_num_loops = '--num-loops'
    arg_time_sleep = '--time-sleep'

    time_sleep = 1.0
    num_loops = -1

    print(sys.argv)
    for i, arg in enumerate(sys.argv):
        if i <= 0:
            continue
        if arg.startswith(arg_num_loops):
            num_loops = int(arg[len(arg_num_loops)+1:])
        if arg.startswith(arg_time_sleep):
            time_sleep = float(arg[len(arg_time_sleep)+1:])

    with datalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)


        # This is the connection string for TCP in the format: tcp://USER:PASSWORD@IP_ADDRESS:PORT
        # Please check and change according your environment:
        # - USER:       Enter your user name here - default is boschrexroth
        # - PASSWORD:   Enter your password here - default is boschrexroth
        # - IP_ADDRESS: 127.0.0.1   If you develop in WSL and you want to connect to a ctrlX CORE virtual with port forwarding
        #               10.0.2.2    If you develop in a VM (Virtual Box, QEMU,...) and you want to connect to a ctrlX virtual with port forwarding
        #               192.168.1.1 If you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater

        connectionProvider = "tcp://boschrexroth:boschrexroth@127.0.0.1:2070"

        if 'SNAP' in os.environ:
            connectionProvider = "ipc://"

        print("Connecting", connectionProvider)
        with datalayer_system.factory().create_provider(connectionProvider) as provider:
            result = provider.start()
            if result is not datalayer.variant.Result.OK:
                print("ERROR Starting Data Layer Provider failed with: ", result)
                sys.exit(1)

            providerNodeAllDataStatic = ProviderNodeAllData(
                provider, "all-data/_py/", False)
            providerNodeAllDataStatic.RegisterNodes()

            providerNodeAllDataDynamic = ProviderNodeAllData(
                provider, "all-data/_py/", True)
            providerNodeAllDataDynamic.RegisterNodes()

            loops = 1
            while True:

                print("Loop", loops, end='')
                if num_loops >= 1:
                    print(" of ",  num_loops, end='')
                    if loops > num_loops:
                        print("DONE")
                        break
                print()
                
                loops = loops + 1

                if provider.is_connected() == False:
                    print("ERROR: Data Layer provider is NOT connected")
                    break

                providerNodeAllDataDynamic.CheckConfig()

                time.sleep(time_sleep)

            provider.stop()
            # python context manager calls automatically provider.close()

    datalayer_system.stop(True)


if __name__ == "__main__":
    main(sys.argv)
