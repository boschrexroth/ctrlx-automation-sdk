# MIT License
#
# Copyright (c) 2020-2022 Bosch Rexroth AG
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

import typing
from datetime import datetime

# C-interfaces
import ctrlxdatalayer
import ctrlxdatalayer.subscription
from ctrlxdatalayer.client import Client
from ctrlxdatalayer.variant import Result, Variant


def rncb(result: Result, items: typing.List[ctrlxdatalayer.subscription.NotifyItem], userdata: ctrlxdatalayer.clib.userData_c_void_p):

    now = datetime.now().time()
    print(now, "----------------------------------------------------------")
    print("ResponseNotifyCallback", result)
    if result != Result.OK:
        return

    if items is None:
        print("No items")
        return

    print("Number of items", len(items))
    if len(items) <= 0:
        return

    n = 1
    for item in items:
        print("#", n)
        print("  address:", item.get_address())
        print("  type:", item.get_data().get_type())
        print("  value:", item.get_data().get_float32())
        print("  timestamp:", item.get_timestamp())
        print("  datetime:", ctrlxdatalayer.subscription.to_datetime(
            item.get_timestamp()))
        n = n + 1


def subscribe_single(client: Client, subscription_properties: Variant):

    print("subscribe_single() +++++++++++++++++++++++++++++++++++++++++++++++")

    result, subscription = client.create_subscription_sync(
        subscription_properties, rncb)
    if result != Result.OK:
        print("ERROR create_subscription_sync() failed with:", result)
        return result, None

    if subscription is None:
        print("ERROR create_subscription_sync() returned: None")
        return Result.CREATION_FAILED, None

    address = "framework/metrics/system/cpu-utilisation-percent"
    result = subscription.subscribe(address)

    return result, subscription


def get_address_list(client: Client, addressBase: str):
    result, data = client.browse_sync(addressBase)
    with data:
        if result != Result.OK:
            return None  # Error

        if data is None:
            return None  # Error

        addressList = []
        for node in data.get_array_string():
            addressList.append(addressBase + node)

        return addressList


def subscribe_multi(client: Client, subscription_properties: Variant):

    print("subscribe_multi() ++++++++++++++++++++++++++++++++++++++++++++++++")

    addressBase = "framework/metrics/system/"
    addressList = get_address_list(client, addressBase)
    if addressList is None:
        print("ERROR No sub nodes found:", addressBase)
        return Result.FAILED, None

    result, subscription = client.create_subscription_sync(subscription_properties, rncb)
    if result != Result.OK:
        print("ERROR create_subscription_sync() failed with:", result)
        return result, None

    if subscription is None:
        print("ERROR create_subscription_sync() returned: None")
        return Result.CREATION_FAILED, None

    result = subscription.subscribe_multi(addressList)

    return result, subscription
