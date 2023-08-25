#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import sys

import time
from datetime import datetime
from typing import List
import flatbuffers

import ctrlxdatalayer
from ctrlxdatalayer.variant import Result, Variant
from comm.datalayer import SubscriptionProperties


import debug

from helper.ctrlx_datalayer_helper import get_client

def remote_debugging_wait_for_client(port: int):
    """Waits until a debug client connects.
    Detailed functions:
    - Set the listen parameters: '0.0.0.0' means listen on all network interfaces
    - Start the debug server
    - Wait until debug client connects.

    Parameters:
    port (int): Port number for the debug connection

    Returns:
    No value, the function blocks and returns if a debug client has been connected.

    """

    print('Accepting remote debug client attaches to port', port, flush=True)
    address = ('0.0.0.0', port) # Accept incomming calls from all network interfaces
    debugpy.listen(address)

    debugpy.wait_for_client()
    print('Debugger is attached!', flush=True)

    debugpy.breakpoint()



def main():
    """main
    """

    print()
    print("========================================================================")
    print("sdk-py-datalayer-client - A ctrlX Data Layer Client App in Python", flush=True)

    with ctrlxdatalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)

        client, connection_string = get_client(datalayer_system)
        if client is None:
            print("ERROR Connecting", connection_string, "failed.", flush=True)
            sys.exit(1)

        with client:

            # Define the subscription properties by using Flatbuffers class SubscriptionProperties
            sub_prop = __set_sub_prop()

            # Create subscription
            print("INFO Creating subscription", flush=True)
            result, sub = client.create_subscription_sync(
                sub_prop, cb_subscription_sync)
            sub_prop.close()
            if result is not Result.OK:
                print("ERROR Creating subscription failed:", result, flush=True)

            # Add subscription node
            print("INFO Add subscription node", flush=True)
            sub_adr = "framework/metrics/system/cpu-utilisation-percent"
            result = sub.subscribe(sub_adr)
            if result is not Result.OK:
                print("ERROR Adding subscription node failed:", result, flush=True)

            while client.is_connected():

                dt_str = datetime.now().strftime("%d/%m/%Y %H:%M:%S.%f")

                addr = "framework/metrics/system/memused-percent"
                result, read_var = client.read_sync(addr)
                val = read_var.get_float64()
                print("INFO read_sync: %s, %s: %f" % (dt_str, addr, val), flush=True)

                time.sleep(5.0)

            print("ERROR ctrlX Data Layer connection", flush=True)
            print("INFO Close subscription", flush=True)
            sub.close()

        print("INFO Stopping ctrlX Data Layer system", flush=True)
        datalayer_system.stop(True)

def __set_sub_prop():
    """__set_sub_prop
    """
    builder = flatbuffers.Builder(1024)
    id = builder.CreateString("sdk-py-remote-debug")
    SubscriptionProperties.SubscriptionPropertiesStart(builder)
    SubscriptionProperties.SubscriptionPropertiesAddId(builder, id)
    SubscriptionProperties.SubscriptionPropertiesAddKeepaliveInterval(
                builder, 10000)
    SubscriptionProperties.SubscriptionPropertiesAddPublishInterval(
                builder, 1000)
    SubscriptionProperties.SubscriptionPropertiesAddErrorInterval(
                builder, 10000)
    properties = SubscriptionProperties.SubscriptionPropertiesEnd(
                builder)
    builder.Finish(properties)
    sub_prop = Variant()
    sub_prop.set_flatbuffers(builder.Output())
    return sub_prop


# Response notify callback function
def cb_subscription_sync(result: Result, items: List[ctrlxdatalayer.subscription.NotifyItem], userdata):
    """cb_subscription_sync
    """
    if result is not Result.OK:
        print("ERROR notify subscription:", result, flush=True)
        return
    timestamp = items[0].get_timestamp()
    # convert ldap to unix timestamp
    dt = datetime.fromtimestamp(timestamp/10000000-11644473600)
    dt_str = dt.strftime("%d/%m/%Y %H:%M:%S.%f")
    address = items[0].get_address()
    val = Variant.get_float64(items[0].get_data())
    print("INFO Subscription notification: %s, %s: %f" %
          (dt_str, address, val), flush=True)


if __name__ == '__main__':
    # Wait for remote debug client
    debug.remote_debugging_wait_for_client(port=15678)

    # Run function
    main()
