#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import sys
import signal
import time
from datetime import datetime
import ctrlxdatalayer

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
                f"ERROR Connecting {datalayer_client_connection_string} failed.",
                flush=True
            )
            sys.exit(1)

        with (
            datalayer_client
        ):  # datalayer_client is closed automatically when leaving with block
            if datalayer_client.is_connected() is False:
                print(f"ERROR ctrlX Data Layer is NOT connected: {datalayer_client_connection_string}",
                    flush=True
                )
                sys.exit(1)

            while datalayer_client.is_connected() and not __close_app:
                dt_str = datetime.now().strftime("%H:%M:%S.%f")

                # Float64 -------------------------------------------------------
                addr = "framework/metrics/system/memused-percent"
                result, float64_var = datalayer_client.read_sync(addr)
                float64_value = float64_var.get_float64()
                print(
                    f"INFO {dt_str} Sync read '{addr}': {float64_value} : result {result}",
                    flush=True
                )

                time.sleep(2.0)

            print("ERROR ctrlX Data Layer is NOT connected")

        stop_ok = datalayer_system.stop(
            False
        )  # Attention: Doesn't return if any provider or client instance is still running
        print("System Stop", stop_ok, flush=True)


if __name__ == "__main__":
    main()
