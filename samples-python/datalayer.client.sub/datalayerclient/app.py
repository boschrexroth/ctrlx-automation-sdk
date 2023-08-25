# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import typing
from datetime import datetime

# C-interfaces
import ctrlxdatalayer
import ctrlxdatalayer.subscription
from ctrlxdatalayer.client import Client
from ctrlxdatalayer.variant import Result, Variant


def rncb(result: Result, items: typing.List[ctrlxdatalayer.subscription.NotifyItem], userdata: ctrlxdatalayer.clib.userData_c_void_p):
    """rncb
    """

    now = datetime.now().time()
    print(now, "----------------------------------------------------------")
    print("ResponseNotifyCallback", result, flush=True)
    if result != Result.OK:
        return

    if items is None:
        print("No items", flush=True)
        return

    print("Number of items", len(items), flush=True)
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
            item.get_timestamp()), flush=True)
        n = n + 1


def subscribe_single(client: Client, subscription_properties: Variant):
    """subscribe_single
    """
    print("subscribe_single() +++++++++++++++++++++++++++++++++++++++++++++++", flush=True)

    result, subscription = client.create_subscription_sync(
        subscription_properties, rncb)
    if result != Result.OK:
        print("ERROR create_subscription_sync() failed with:", result, flush=True)
        return result, None

    if subscription is None:
        print("ERROR create_subscription_sync() returned: None", flush=True)
        return Result.CREATION_FAILED, None

    address = "framework/metrics/system/cpu-utilisation-percent"
    result = subscription.subscribe(address)

    return result, subscription


def get_address_list(client: Client, addressBase: str):
    """get_address_list
    """
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
    """subscribe_multi
    """
    print("subscribe_multi() ++++++++++++++++++++++++++++++++++++++++++++++++", flush=True)

    addressBase = "framework/metrics/system/"
    addressList = get_address_list(client, addressBase)
    if addressList is None:
        print("ERROR No sub nodes found:", addressBase, flush=True)
        return Result.FAILED, None

    result, subscription = client.create_subscription_sync(subscription_properties, rncb)
    if result != Result.OK:
        print("ERROR create_subscription_sync() failed with:", result, flush=True)
        return result, None

    if subscription is None:
        print("ERROR create_subscription_sync() returned: None", flush=True)
        return Result.CREATION_FAILED, None

    result = subscription.subscribe_multi(addressList)

    return result, subscription
