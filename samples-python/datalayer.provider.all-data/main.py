#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import signal
import sys
import time

import ctrlxdatalayer

from alldataprovider.nodeManagerAllData import NodeManagerAllData
from helper.ctrlx_datalayer_helper import get_provider

address_root = "sdk-py-provider-alldata/"

close_app = False


def handler(signum, frame):
    """handler
    """
    global close_app
    close_app = True
    #print('Here you go signum: ', signum, close_app, flush=True)


def main():
    """main
    """
    signal.signal(signal.SIGINT, handler)
    signal.signal(signal.SIGTERM, handler)
    signal.signal(signal.SIGABRT, handler)

    datalayer_system = ctrlxdatalayer.system.System("")
    datalayer_system.start(False)

    # Try SSL port 8443
    provider, connection_string = get_provider(datalayer_system)
    if provider is None:
        print("ERROR Connecting", connection_string, "failed.", flush=True)
        datalayer_system.stop(False)
        sys.exit(1)

    print("INFO Connecting", connection_string, "succeeded.", flush=True)

    with provider:  # provider.close() is called automatically when leaving with block

        nodeManager = NodeManagerAllData(
            provider, "sdk-py-provider-alldata/")
        nodeManager.create_dynamic_nodes()
        nodeManager.create_static_nodes()

        while provider.is_connected() and not close_app:
            time.sleep(5.0)

        print("ERROR: ctrlX Data Layer provider is NOT connected", flush=True)

        provider.stop()

    # Attention: Doesn't return if any provider or client instance is still running
    stop_ok = datalayer_system.stop(False)
    print("System Stop", stop_ok, flush=True)


if __name__ == "__main__":
    main()
