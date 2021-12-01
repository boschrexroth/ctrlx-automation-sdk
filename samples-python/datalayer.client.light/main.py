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
import time
from datetime import datetime
from typing import List
import flatbuffers

import datalayer
from datalayer.variant import Result, Variant
from comm.datalayer import SubscriptionProperties


def main():

    # This is the connection string for TCP in the format: tcp://USER:PASSWORD@IP_ADDRESS:PORT
    # Please check and change according your environment:
    # - USER:       Enter your user name here - default is boschrexroth
    # - PASSWORD:   Enter your password here - default is boschrexroth
    # - IP_ADDRESS: 127.0.0.1   If you develop in WSL and you want to connect to a ctrlX CORE virtual with port forwarding
    #               10.0.2.2    If you develop in a VM (Virtual Box, QEMU,...) and you want to connect to a ctrlX virtual with port forwarding
    #               192.168.1.1 If you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater

    connectionClient = "tcp://boschrexroth:boschrexroth@10.0.2.2:2069"

    if 'SNAP' in os.environ:
        connectionClient = "ipc://"

    print()
    print("========================================================================")
    print("sdk-py-datalayer-client - A ctrlX Data Layer Client App in Python")

    with datalayer.system.System("") as datalayer_system:
        print("INFO Starting Data Layer system")
        datalayer_system.start(False)

        # Create Data Layer client connection
        print("INFO Creating Data Layer Client connection to ctrlX with", connectionClient)
        with datalayer_system.factory().create_client(connectionClient) as datalayer_client:

            # Check if client is connected
            print("INFO Client connected:", datalayer_client.is_connected())
            if datalayer_client.is_connected() is False:
                return

            # Define the subscription properties by using Flatbuffers class SubscriptionProperties
            builder = flatbuffers.Builder(1024)
            id = builder.CreateString("sdk-py-sub")
            SubscriptionProperties.SubscriptionPropertiesStart(builder)
            SubscriptionProperties.SubscriptionPropertiesAddId(builder, id)
            SubscriptionProperties.SubscriptionPropertiesAddKeepaliveInterval(builder, 10000)
            SubscriptionProperties.SubscriptionPropertiesAddPublishInterval(builder, 1000)
            SubscriptionProperties.SubscriptionPropertiesAddErrorInterval(builder, 10000)
            properties = SubscriptionProperties.SubscriptionPropertiesEnd(builder)
            builder.Finish(properties)
            sub_prop = Variant()
            sub_prop.set_flatbuffers(builder.Output())

            # Create subscription
            print("INFO Creating subscription")
            result, sub = datalayer_client.create_subscription_sync(sub_prop, cb_subscription_sync)
            if result is not Result.OK:
                print("ERROR Creating subscription failed:", result)

            # Add subscription node
            print("INFO Add subscription node")
            sub_adr = "framework/metrics/system/cpu-utilisation-percent"
            result = sub.subscribe(sub_adr)   
            if result is not Result.OK:
                print("ERROR Adding subscription node failed:", result)

            while datalayer_client.is_connected():

                dt_str = datetime.now().strftime("%d/%m/%Y %H:%M:%S.%f")

                addr = "framework/metrics/system/memused-percent"
                result, read_var = datalayer_client.read_sync(addr)
                val = read_var.get_float64()
                print("INFO read_sync: %s, %s: %f" % (dt_str, addr, val))

                '''
                addr = "scheduler/admin/state"
                dt_str = datetime.now().strftime("%d/%m/%Y %H:%M:%S.%f")
                conv = datalayer_system.json_converter()
                result, read_var = datalayer_client.read_json_sync(conv, "scheduler/admin/state", 0)
                state = read_var.get_string()
                print("INFO read_json_sync: %s, %s: %s" % (dt_str, addr, state))

                if 'RUN' in state:
                    state = state.replace('RUN', 'CONFIG')
                else:
                    state = state.replace('CONFIG', 'RUN')

                print("New state: ", state)

                # Result.OK expected
                result, error = datalayer_client.write_json_sync(conv, addr, state)
                print("write_json_sync Result:", result)

                # Result.INVALID_ADDRESS expected
                result, error = datalayer_client.write_json_sync(conv, addr+"x", state)
                print("write_json_sync with invalid address Result:", result, "Error:", error.get_string())
                '''
                
                time.sleep(5.0)

            print("ERROR Data Layer connection")
            print("INFO Close subscription")
            sub.close()

        print("INFO Stopping Data Layer system")
        datalayer_system.stop(True)


# Response notify callback function
def cb_subscription_sync(result: Result, items: List[datalayer.subscription.NotifyItem], userdata):
    if result is not Result.OK:
        print("ERROR notify subscription:", result)
        return
    timestamp = items[0].get_timestamp()
    dt = datetime.fromtimestamp(timestamp/10000000-11644473600) # convert ldap to unix timestamp
    dt_str = dt.strftime("%d/%m/%Y %H:%M:%S.%f")
    address = items[0].get_address()
    val = Variant.get_float64(items[0].get_data())
    print("INFO Subscription notification: %s, %s: %f" % (dt_str, address, val))


if __name__ == '__main__':
    main()
