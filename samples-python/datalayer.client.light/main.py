#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import signal
import sys
import time
from datetime import datetime
from typing import List

import ctrlxdatalayer
import flatbuffers
from comm.datalayer import Metadata, SubscriptionProperties
from ctrlxdatalayer.variant import Result, Variant

from helper.ctrlx_datalayer_helper import get_client

__close_app = False


def handler(signum, frame):
    """handler"""
    global __close_app
    __close_app = True
    # print('Here you go signum: ', signum, __close_app, flush=True)


def main():
    """main"""
    print()
    print("=================================================================")
    print("sdk-py-datalayer-client - A ctrlX Data Layer Client App in Python")
    print(
        "=================================================================", flush=True
    )

    signal.signal(signal.SIGINT, handler)
    signal.signal(signal.SIGTERM, handler)
    signal.signal(signal.SIGABRT, handler)

    with ctrlxdatalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)

        datalayer_client, datalayer_client_connection_string = get_client(
            datalayer_system, ip="10.0.2.2", ssl_port=8443
        )
        if datalayer_client is None:
            print(
                "ERROR Connecting",
                datalayer_client_connection_string,
                "failed.",
                flush=True,
            )
            sys.exit(1)

        with (
            datalayer_client
        ):  # datalayer_client is closed automatically when leaving with block
            if datalayer_client.is_connected() is False:
                print(
                    "ERROR ctrlX Data Layer is NOT connected:",
                    datalayer_client_connection_string,
                    flush=True,
                )
                sys.exit(1)

            # Define the subscription properties by using Flatbuffers class SubscriptionProperties
            builder = flatbuffers.Builder(1024)
            id = builder.CreateString("sdk-py-sub")
            SubscriptionProperties.SubscriptionPropertiesStart(builder)
            SubscriptionProperties.SubscriptionPropertiesAddId(builder, id)
            SubscriptionProperties.SubscriptionPropertiesAddKeepaliveInterval(
                builder, 10000
            )
            SubscriptionProperties.SubscriptionPropertiesAddPublishInterval(
                builder, 2000
            )
            SubscriptionProperties.SubscriptionPropertiesAddErrorInterval(
                builder, 10000
            )
            properties = SubscriptionProperties.SubscriptionPropertiesEnd(builder)
            builder.Finish(properties)
            sub_prop = Variant()
            sub_prop.set_flatbuffers(builder.Output())

            # Create subscription
            print("INFO Creating subscription", flush=True)
            result, sub = datalayer_client.create_subscription_sync(
                sub_prop, cb_subscription_sync
            )
            if result is not Result.OK:
                print("ERROR Creating subscription failed:", result, flush=True)
            sub_prop.close()

            # Add subscription node
            print("INFO Add subscription node", flush=True)
            sub_adr = "framework/metrics/system/cpu-utilisation-percent"
            result = sub.subscribe(sub_adr)
            if result is not Result.OK:
                print("ERROR Adding subscription node failed:", result, flush=True)

            while datalayer_client.is_connected() and not __close_app:
                dt_str = datetime.now().strftime("%H:%M:%S.%f")

                # Float64 -------------------------------------------------------
                addr = "framework/metrics/system/memused-percent"
                result, float64_var = datalayer_client.read_sync(addr)
                with float64_var:
                    float64_value = float64_var.get_float64()
                    print(
                        f"INFO {dt_str} Sync read '{addr}': {float64_value}",
                        flush=True,
                    )

                # Flatbuffers ---------------------------------------------------
                addr = "datalayer/subscriptions/settings"
                result, fbs_var = datalayer_client.read_sync(addr)
                result, fbs_metadata = datalayer_client.metadata_sync(addr)

                metadata_root = Metadata.Metadata.GetRootAsMetadata(
                    fbs_metadata.get_flatbuffers()
                )
                fbs_metadata.close()

                address_read_type = ""
                for i in range(0, metadata_root.ReferencesLength()):
                    reference = metadata_root.References(i)

                    if reference is None:
                        continue

                    if reference.Type().decode("utf-8").lower() == "readtype":
                        address_read_type = reference.TargetAddress().decode("utf-8")
                        break

                result, type_var = datalayer_client.read_sync(address_read_type)
                # Convert to JSON
                with type_var, fbs_var:
                    (
                        result,
                        json,
                    ) = datalayer_system.json_converter().converter_generate_json_complex(
                        fbs_var, type_var, -1
                    )

                    fbs_value = json.get_string()
                    print(
                        f"INFO {dt_str} Sync read '{addr}': {fbs_value}",
                        flush=True,
                    )

                time.sleep(2.0)

            print("ERROR ctrlX Data Layer is NOT connected")
            print("INFO Closing subscription", flush=True)
            sub.close()

        stop_ok = datalayer_system.stop(
            False
        )  # Attention: Doesn't return if any provider or client instance is still running
        print("System Stop", stop_ok, flush=True)


# Response notify callback function
def cb_subscription_sync(
    result: Result, items: List[ctrlxdatalayer.subscription.NotifyItem], userdata
):
    """cb_subscription_sync"""
    if result is not Result.OK:
        print("ERROR notify subscription:", result, flush=True)
        return
    timestamp = items[0].get_timestamp()
    dt = datetime.fromtimestamp(
        timestamp / 10000000 - 11644473600
    )  # convert ldap to unix timestamp
    dt_str = dt.strftime("%d/%m/%Y %H:%M:%S.%f")
    address = items[0].get_address()
    val = items[0].get_data().get_float64()
    print(
        f"INFO {dt_str} Subscription notification '{address}': {val}",
        flush=True,
    )


if __name__ == "__main__":
    main()
