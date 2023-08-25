#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import faulthandler
import signal
import sys
import time

import ctrlxdatalayer

from app.call_datalayer_client import CallDataLayerClient
from helper.ctrlx_datalayer_helper import get_client

close_app = False


def handler(signum, frame):
    """handler
    """
    global close_app
    close_app = True
    # print('Here you go signum: ', signum, close_app, flush=True)


if __name__ == '__main__':

    faulthandler.enable()
    signal.signal(signal.SIGINT, handler)
    signal.signal(signal.SIGTERM, handler)
    signal.signal(signal.SIGABRT, handler)

    print()
    print("========================================================================")
    print("rexroth-python-client - A ctrlX Data Layer Client App in Python")
    print()
    print("These functions are used synchronously and asynchronously:")
    print("  ping_..")
    print("  read_..")
    print("  create_..")
    print("  remove_..")
    print("  browse_..")
    print("  write_..")
    print("  metadata_..")
    print()
    print("Precondition:")
    print("Build and install the snap 'sdk-cpp-alldata' from the SDK folder")
    print("samples-cpp/datalayer.provider.all-data")
    print("========================================================================", flush=True)

    with ctrlxdatalayer.system.System("") as datalayer_system:
        print("INFO Starting ctrlX Data Layer system", flush=True)
        datalayer_system.start(False)

        datalayer_client, connection_string = get_client(datalayer_system)
        if datalayer_client is None:
            print("WARNING Connecting", connection_string, "failed.", flush=True)
            datalayer_system.stop(False)
            sys.exit(1)

        print("INFO Connecting", connection_string, "succeeded.", flush=True)

        with datalayer_client:  # datalayer_client is closed automatically when leaving with block

            print("INFO Creating Python ctrlX Data Layer Client instance", flush=True)
            calldatalayerclient = CallDataLayerClient(datalayer_client)

            while datalayer_client.is_connected() and not close_app:
                calldatalayerclient.run()
                time.sleep(1.0)

            print("ERROR ctrlX Data Layer is NOT connected", flush=True)

        print("INFO Stopping ctrlX Data Layer system")
        # Attention: Doesn't return if any provider or client instance is still runnning
        stop_ok = datalayer_system.stop(False)
        print("System Stop", stop_ok, flush=True)
