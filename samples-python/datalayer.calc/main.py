#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import sys

import faulthandler
import time

import ctrlxdatalayer

from calculations.basic_arithmetic_operations import BasicArithmeticOperations
from helper.ctrlx_datalayer_helper import get_client, get_provider

addr_root = "sdk-py-calc"


def start_new_basic_arithmetic_operation(
        provider: ctrlxdatalayer.provider.Provider,
        client: ctrlxdatalayer.client.Client,
        id: str,
        mode: str):
    """start_new_basic_arithmetic_operation
    """

    basicArithmeticOperation = BasicArithmeticOperations(
        provider, client, addr_root, id, mode)
    basicArithmeticOperation.register_nodes()

    while basicArithmeticOperation.subscribe() != ctrlxdatalayer.variant.Result.OK:
        basicArithmeticOperation.unsubscribe()
        print("WARN Starting ctrlX Data Layer subscriptions for",
              addr_root + "/" + id, "failed with: " + str(result))
        print("INFO Retry in 5s", flush=True)
        time.sleep(5.0)
    pass


if __name__ == '__main__':

    print()
    print("===========================================================================")
    print("ctrlX Application in Python:")
    print("- Reads ctrlX Data Layer values per subscription.")
    print("- Runs a algorithm")
    print("- Provides result as ctrlX Data Layer Node")
    print()
    print("Will be restarted by the snap system on error.")
    print("===========================================================================", flush=True)

    faulthandler.enable()

    system = ctrlxdatalayer.system.System("")
    system.start(False)

    # Change ip, user, password etc. if they differ from your application.
    ip="192.168.1.1"            # "10.0.2.2" for ctrlX COREvirtual with Port Forwarding
    user="boschrexroth"
    password="boschrexroth"
    ssl_port=443               # 8443 for ctrlX COREvirtual with Port Forwarding

    client, client_connection_string = get_client(system, ip, user, password, ssl_port)
    if client is None:
        print("ERROR Could get ctrlX Datalayer client connection:", client_connection_string, flush=True)
        system.stop(False)
        sys.exit(1)
    print("INFO ctrlX Datalayer client connection succeeded:", client_connection_string, flush=True)

    provider, provider_connection_string = get_provider(system, ip, user, password, ssl_port)
    if provider is None:
        print("ERROR Could get ctrlX Datalayer provider connection:", provider_connection_string, flush=True)
        provider.close()
        client.close()
        system.stop(False)
        sys.exit(2)

    print("INFO ctrlX Datalayer provider connection succeeded:", provider_connection_string, flush=True)

    update_time = 1.0

    bao = BasicArithmeticOperations(
        provider, client, addr_root, "basic-arithmetic-operation", "+", int(update_time)*1000)
    result = bao.register_nodes()
    if result != ctrlxdatalayer.variant.Result.OK:
        print("ERROR Registering nodes failed with", str(result), flush=True)
        provider.close()
        client.close()
        system.stop(False)
        sys.exit(2)

    result = bao.subscribe()
    if result != ctrlxdatalayer.variant.Result.OK:
        print("ERROR Subscribing values failed with", str(result), flush=True)
        provider.close()
        client.close()
        system.stop(False)
        sys.exit(2)

    # Endless loop
    while client.is_connected() & provider.is_connected():

        if bao.subscription_changed:
            bao.unsubscribe()
            bao.subscribe()

        if bao.value_changed:
            bao.calc()  # call calc() here to be able to debug

        time.sleep(update_time)

    print("ERROR Client connection", client_connection_string,
          "disconnected - exiting application. Will be restarted automatically.", flush=True)

    bao.unsubscribe()

    provider.close()
    client.close()

    # Attention: Doesn't return if any provider or client instance is still running
    stop_ok = system.stop(False)
    print("System Stop", stop_ok, flush=True)

    sys.exit(3)
