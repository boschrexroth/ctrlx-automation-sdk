#!/usr/bin/env python3

# MIT License
#
# Copyright (c) 2020-2022 Bosch Rexroth AG
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

import faulthandler
import signal
import sys
import time

import ctrlxdatalayer

from app.call_datalayer_client import CallDataLayerClient
from helper.ctrlx_datalayer_helper import get_client

close_app = False


def handler(signum, frame):
    global close_app
    close_app = True
#    print('Here you go signum: ', signum, close_app)


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
    print("========================================================================")
    print()

    with ctrlxdatalayer.system.System("") as datalayer_system:
        print("INFO Starting Data Layer system")
        datalayer_system.start(False)

        datalayer_client, connection_string = get_client(datalayer_system)
        if datalayer_client is None:
            print("WARNING Connecting", connection_string, "failed.")
            datalayer_system.stop(False)
            sys.exit(1)

        print("INFO Connecting", connection_string, "succeeded.")

        with datalayer_client:  # datalayer_client is closed automatically when leaving with block

            print("INFO Creating Python Data Layer Client instance")
            calldatalayerclient = CallDataLayerClient(datalayer_client)

            while datalayer_client.is_connected() and not close_app:
                calldatalayerclient.run()
                time.sleep(1.0)

            print("ERROR Data Layer is NOT connected")

        print("INFO Stopping Data Layer system")
        # Attention: Doesn't return if any provider or client instance is still runnning
        stop_ok = datalayer_system.stop(False)
        print("System Stop", stop_ok)
