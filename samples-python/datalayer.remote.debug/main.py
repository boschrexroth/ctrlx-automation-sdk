#!/usr/bin/env python3

# MIT License
#
# Copyright (c) 2021-2022 Bosch Rexroth AG
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

import sys

import time
from datetime import datetime
from typing import List
import flatbuffers

import ctrlxdatalayer
from ctrlxdatalayer.variant import Result, Variant
from comm.datalayer import SubscriptionProperties

import debugging

from helper.ctrlx_datalayer_helper import get_client

def main():

    debugging.breakpoint()

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

            # Create subscription
            print("INFO Creating subscription", flush=True)
            result, sub = client.create_subscription_sync(
                sub_prop, cb_subscription_sync)
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

            print("ERROR Data Layer connection", flush=True)
            print("INFO Close subscription", flush=True)
            sub.close()

        print("INFO Stopping Data Layer system", flush=True)
        datalayer_system.stop(True)


# Response notify callback function
def cb_subscription_sync(result: Result, items: List[ctrlxdatalayer.subscription.NotifyItem], userdata):
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
    print('Number of arguments:', len(sys.argv), 'arguments.')
    print('Argument List:', str(sys.argv), flush=True)
    debugging.init()
    main()
