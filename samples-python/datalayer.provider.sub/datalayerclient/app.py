# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import sys
import typing
from datetime import datetime

# C-interfaces
import ctrlxdatalayer
import ctrlxdatalayer.subscription
from ctrlxdatalayer.client import Client
from ctrlxdatalayer.variant import Result, Variant

from helper.ctrlx_datalayer_helper import get_client


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

    for item in items:
        print("  address:", item.get_address())
        print("  type:", item.get_data().get_type())
        print("  value:", item.get_data().get_int32())
        print("  timestamp:", item.get_timestamp())
        print("  datetime:", ctrlxdatalayer.subscription.to_datetime(
            item.get_timestamp()), flush=True)

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

    address = "sdk/py/provider_sub/int"
    result = subscription.subscribe(address)

    return result, subscription

g_client = None
g_subscription = None

def local_client_start(system: ctrlxdatalayer.system.System):
    """only local debug 
    Args:
        system (ctrlxdatalayer.system.System): 
        node_name (str): 
    """

    connection_str = "ipc://"
    datalayer_client, datalayer_client_connection_string = get_client(
        system, ip=connection_str, ssl_port=8443
    )
    if datalayer_client is None:
        print(
            "WARNING Connecting",
            datalayer_client_connection_string,
            "failed.",
            flush=True,
        )
        sys.exit(1)

    subscription_properties = ctrlxdatalayer.subscription.create_properties(
        "python-datalayer-client-sub", publish_interval=1000
    )

    if subscription_properties is None:
        print("ERROR create_properties() returned: None", flush=True)
        sys.exit(1)

    with subscription_properties:
        result, subscription = subscribe_single(
            datalayer_client, subscription_properties
        )
        if result != Result.OK:
            print("ERROR subscribe_single() failed with:", result, flush=True)
            sys.exit(1)

        if subscription is None:
            print("ERROR subscribe_single() returned None", flush=True)
            sys.exit(1)

        global g_client
        g_client = datalayer_client
        global g_subscription
        g_subscription = subscription

def local_client_stop():
    """only local debug
    """
    if g_client is None:
        return

    g_subscription.unsubscribe_all()
    g_subscription.close()

    g_client.close()
