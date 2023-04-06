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

    basicArithmeticOperation = BasicArithmeticOperations(
        provider, client, addr_root, id, mode)
    basicArithmeticOperation.register_nodes()

    while basicArithmeticOperation.subscribe() != ctrlxdatalayer.variant.Result.OK:
        basicArithmeticOperation.unsubscribe()
        print("WARN Starting Data Layer subscriptions for",
              addr_root + "/" + id, "failed with: " + str(result))
        print("INFO Retry in 5s", flush=True)
        time.sleep(5.0)
    pass


if __name__ == '__main__':

    print()
    print("===========================================================================")
    print("ctrlX Application in Python:")
    print("- Reads Data Layer values per subscription.")
    print("- Runs a algorithm")
    print("- Provides result as Data Layer Node")
    print()
    print("Will be restarted by the snap system on error.")
    print("===========================================================================", flush=True)

    faulthandler.enable()

    system = ctrlxdatalayer.system.System("")
    system.start(False)

    # Here default argument values are used: ip="192.168.1.1", user="boschrexroth", password="boschrexroth", ssl_port=443
    # Enter ip, user, password etc. if they differ from your application.
    client, client_connection_string = get_client(system)
    if client is None:
        print("ERROR Could get ctrlX Datalayer client connection:", client_connection_string, flush=True)
        system.stop(False)
        sys.exit(1)
    print("INFO ctrlX Datalayer client connection succeeded:", client_connection_string, flush=True)

    # Here default argument values are used: ip="192.168.1.1", user="boschrexroth", password="boschrexroth", ssl_port=443
    # Enter ip, user, password etc. if they differ from your application.
    provider, provider_connection_string = get_provider(system, ip="10.52.244.109",
               user="boschrexroth",
               password="boschrexroth",
               ssl_port=443)
    if provider is None:
        print("ERROR Could get ctrlX Datalayer provider connection:", provider_connection_string, flush=True)
        provider.close()
        client.close()
        system.stop(False)
        sys.exit(2)

    print("INFO ctrlX Datalayer provider connection succeeded:", provider_connection_string, flush=True)

    update_time = 1.0
    bao = BasicArithmeticOperations(
        provider, client, addr_root, "1", "+", int(update_time)*1000)
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
