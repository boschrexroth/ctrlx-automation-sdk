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

import signal
import sys
import time

import ctrlxdatalayer

from alldataprovider.nodeManagerAllData import NodeManagerAllData
from helper.ctrlx_datalayer_helper import get_provider

address_root = "sdk-py-provider-alldata/"

close_app = False


def handler(signum, frame):
    global close_app
    close_app = True
    #print('Here you go signum: ', signum, close_app, flush=True)


def main():

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

        print("ERROR: Data Layer provider is NOT connected", flush=True)

        provider.stop()

    # Attention: Doesn't return if any provider or client instance is still running
    stop_ok = datalayer_system.stop(False)
    print("System Stop", stop_ok, flush=True)


if __name__ == "__main__":
    main()
