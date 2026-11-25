# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

from __future__ import annotations

from datetime import datetime, timezone

# C-interfaces
import ctrlxdatalayer
import ctrlxdatalayer.subscription
from comm.datalayer import LosslessRateLimit, Queueing, Sampling
from ctrlxdatalayer.client import Client
from ctrlxdatalayer.subscription_properties_builder import SubscriptionPropertiesBuilder
from ctrlxdatalayer.variant import Result, Variant, VariantType


def rncb(
    result: Result,
    items: list[ctrlxdatalayer.subscription.NotifyItem],
    userdata: ctrlxdatalayer.clib.userData_c_void_p,
):
    """rncb"""

    now = datetime.now(tz=timezone.utc).time()
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
        if item.get_data().get_type() == VariantType.FLOAT32:
            print("  value:", item.get_data().get_float32())
        else:
            print("  value:", item.get_data().get_uint64())
        print("  timestamp:", item.get_timestamp())
        print(
            "  datetime:",
            ctrlxdatalayer.subscription.to_datetime(item.get_timestamp()),
            flush=True,
        )
        n = n + 1


def subscribe_single(client: Client, subscription_properties: Variant):
    """subscribe_single"""
    print(
        "subscribe_single() +++++++++++++++++++++++++++++++++++++++++++++++", flush=True
    )

    result, subscription = client.create_subscription_sync(
        subscription_properties, rncb
    )
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
    """get_address_list"""
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
    """subscribe_multi"""
    print(
        "subscribe_multi() ++++++++++++++++++++++++++++++++++++++++++++++++", flush=True
    )

    addressBase = "framework/metrics/system/"
    addressList = get_address_list(client, addressBase)
    if addressList is None:
        print("ERROR No sub nodes found:", addressBase, flush=True)
        return Result.FAILED, None

    result, subscription = client.create_subscription_sync(
        subscription_properties, rncb
    )
    if result != Result.OK:
        print("ERROR create_subscription_sync() failed with:", result, flush=True)
        return result, None

    if subscription is None:
        print("ERROR create_subscription_sync() returned: None", flush=True)
        return Result.CREATION_FAILED, None

    result = subscription.subscribe_multi(addressList)

    return result, subscription


def subscribe_single_lossless(client: Client):
    """subscribe_single_lossless
    Only nodes that have "qos.lossless: true" in the metadata extensions can be subscribed without loss
    """
    print(
        "subscribe_single_lossless() +++++++++++++++++++++++++++++++++++++++++++++++",
        flush=True,
    )

    builder = SubscriptionPropertiesBuilder("id_lossless")

    s = Sampling.SamplingT()
    s.samplingInterval = 0
    builder.add_rule_sampling(s)

    q = Queueing.QueueingT()
    q.maxQueueSize = 50
    builder.add_rule_queueing(q)

    lr = LosslessRateLimit.LosslessRateLimitT()
    lr.maxRate = 20
    builder.add_rule_losslessratelimit(lr)

    subscription_properties = builder.build()

    with subscription_properties:
        result, subscription = client.create_subscription_sync(
            subscription_properties, rncb
        )
        if result != Result.OK:
            print("ERROR create_subscription_sync() failed with:", result, flush=True)
            return result, None

        if subscription is None:
            print("ERROR create_subscription_sync() returned: None", flush=True)
            return Result.CREATION_FAILED, None

        address = "scheduler/admin/info/common-data-rt/data/counter"
        result = subscription.subscribe(address)

    return result, subscription
