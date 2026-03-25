#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import faulthandler
import signal
import time

import ctrlxdatalayer

from app.calculations.basic_arithmetic_operations import BasicArithmeticOperations
from app.helper.ctrlx_datalayer_helper import get_client, get_provider

addr_root = "sdk/py/calc"

__close_app = False

def handler(signum, frame):
    """handler"""
    global __close_app
    __close_app = True
    print('Here you go signum: ', signum, __close_app, flush=True)

def main():
    """main"""
    signal.signal(signal.SIGINT, handler)
    signal.signal(signal.SIGTERM, handler)
    signal.signal(signal.SIGABRT, handler)

    print()
    print("===========================================================================")
    print("ctrlX Application in Python:")
    print("- Reads ctrlX Data Layer values per subscription.")
    print("- Runs a algorithm")
    print("- Provides result as ctrlX Data Layer Node")
    print()
    print("Will be restarted by the snap system on error.")
    print(
        "===========================================================================",
        flush=True,
    )

    faulthandler.enable()

    with ctrlxdatalayer.system.System("") as system:
        system.start(False)

        # Change ip, user, password etc. if they differ from your application.
        ip = "10.0.2.2"  # "10.0.2.2" for ctrlX COREvirtual with Port Forwarding
        user = "boschrexroth"
        password = "boschrexroth"
        ssl_port = 8443  # 8443 for ctrlX COREvirtual with Port Forwarding

        client, client_connection_string = get_client(system, ip, user, password, ssl_port)
        if client is None:
            print(
                "ERROR Could get ctrlX Datalayer client connection:",
                client_connection_string,
                flush=True,
            )
            return

        with client:
            print(
                "INFO ctrlX Datalayer client connection succeeded:",
                client_connection_string,
                flush=True,
            )

            provider, provider_connection_string = get_provider(
                system, ip, user, password, ssl_port
            )
            if provider is None:
                print(
                    "ERROR Could get ctrlX Datalayer provider connection:",
                    provider_connection_string,
                    flush=True,
                )
                return

            with provider:
                print(
                    "INFO ctrlX Datalayer provider connection succeeded:",
                    provider_connection_string,
                    flush=True,
                )

                update_time = 1.0

                bao = BasicArithmeticOperations(
                    provider,
                    client,
                    addr_root,
                    "basic-arithmetic-operation",
                    "+",
                    int(update_time) * 1000,
                )
                result = bao.register_nodes()
                if result != ctrlxdatalayer.variant.Result.OK:
                    print("ERROR Registering nodes failed with", str(result), flush=True)
                    return

                result = bao.subscribe()
                if result != ctrlxdatalayer.variant.Result.OK:
                    print("ERROR Subscribing values failed with", str(result), flush=True)
                    return

                # Endless loop
                while client.is_connected() and provider.is_connected() and not __close_app:
                    if bao.subscription_changed:
                        bao.unsubscribe()
                        bao.subscribe()

                    if bao.value_changed:
                        bao.calc()  # call calc() here to be able to debug

                    time.sleep(update_time)

                print(
                    "ERROR Client connection",
                    client_connection_string,
                    "disconnected - exiting application. Will be restarted automatically.",
                    flush=True,
                )

                bao.unsubscribe()

        # Attention: Doesn't return if any provider or client instance is still running
        stop_ok = system.stop(False)
        print("System Stop", stop_ok, flush=True)


if __name__ == "__main__":
    while not __close_app:
        main()
