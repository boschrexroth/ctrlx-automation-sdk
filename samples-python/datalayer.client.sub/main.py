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

import os
import sys

import faulthandler
import time

import datalayer
from datalayer.variant import Result

import datalayerclient.app

def is_snap() -> bool:
    return 'SNAP' in os.environ


if __name__ == '__main__':

    print()
    print("===========================================================================")
    print("Simple ctrlX Data Layer Client Snap in Python using Data Layer subsciptions")
    print("Will be restarted by the snap system.")
    print("===========================================================================")
    print()

    faulthandler.enable()

    with datalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)

        # This is the connection string for TCP in the format: tcp://USER:PASSWORD@IP_ADDRESS:PORT
        # Please check and change according your environment:
        # - USER:       Enter your user name here - default is boschrexroth
        # - PASSWORD:   Enter your password here - default is boschrexroth
        # - IP_ADDRESS: 127.0.0.1   If you develop in WSL and you want to connect to a ctrlX CORE virtual with port forwarding
        #               10.0.2.2    If you develop in a VM (Virtual Box, QEMU,...) and you want to connect to a ctrlX virtual with port forwarding
        #               192.168.1.1 If you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater
        
        connection = "tcp://boschrexroth:boschrexroth@127.0.0.1:2069"

        if is_snap():
            connection = "ipc://"

        with datalayer_system.factory().create_client(connection) as datalayer_client:

            result = datalayer_client.set_timeout(
                datalayer.client.TimeoutSetting.PING, 5000)

            subscription_properties = datalayer.subscription.create_properties(
                "python-datalayer-client-sub", publish_interval=100)

            if subscription_properties is None:
                print("ERROR create_properties() returned: None")
                sys.exit(1)

            with subscription_properties:
                result, subscription= datalayerclient.app.subscribe_single(datalayer_client, subscription_properties)
                if result != Result.OK :
                    print("ERROR subscribe_single() failed with:", result)
                    sys.exit(1)

                if subscription is None:
                    print("ERROR subscribe_single() returned None")
                    sys.exit(1)

                with subscription:
                    time.sleep(10.0)
                    subscription.unsubscribe_all()

                result, subscription= datalayerclient.app.subscribe_multi(datalayer_client, subscription_properties)
                if result != Result.OK:
                    print("ERROR subscribe_multi() failed with:", result)
                    sys.exit(1)

                if subscription is None :
                    print("ERROR subscribe_multi() returned None")
                    sys.exit(1)

                with subscription:

                    # Endless loop
                    while datalayer_client.is_connected():
                        time.sleep(1.0)

                    subscription.unsubscribe_all()

        datalayer_client.close()
    
    datalayer_system.stop(True)

    sys.exit(0)
