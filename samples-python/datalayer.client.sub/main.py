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

import sys

import faulthandler
import time

import ctrlxdatalayer
from ctrlxdatalayer.variant import Result

from helper.ctrlx_datalayer_helper import get_client
import datalayerclient.app


if __name__ == '__main__':

    print()
    print("=============================================================================")
    print("Simple ctrlX Data Layer Client Snap in Python using Data Layer subscriptions.")
    print("Will be restarted by the snap system.")
    print("=============================================================================", flush=True)

    faulthandler.enable()

    with ctrlxdatalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)

        datalayer_client, datalayer_client_connection_string = get_client(datalayer_system)
        if datalayer_client  is None:
            print("WARNING Connecting", datalayer_client_connection_string, "failed.", flush=True)
            sys.exit(1)

        with datalayer_client:  # datalayer_client is closed automatically when leaving with block

            subscription_properties = ctrlxdatalayer.subscription.create_properties(
                "python-datalayer-client-sub", publish_interval=100)

            if subscription_properties is None:
                print("ERROR create_properties() returned: None", flush=True)
                sys.exit(1)

            with subscription_properties:
                result, subscription = datalayerclient.app.subscribe_single(
                    datalayer_client, subscription_properties)
                if result != Result.OK:
                    print("ERROR subscribe_single() failed with:", result, flush=True)
                    sys.exit(1)

                if subscription is None:
                    print("ERROR subscribe_single() returned None", flush=True)
                    sys.exit(1)

                with subscription:
                    time.sleep(10.0)
                    subscription.unsubscribe_all()

                result, subscription = datalayerclient.app.subscribe_multi(
                    datalayer_client, subscription_properties)
                if result != Result.OK:
                    print("ERROR subscribe_multi() failed with:", result, flush=True)
                    sys.exit(1)

                if subscription is None:
                    print("ERROR subscribe_multi() returned None", flush=True)
                    sys.exit(1)

                with subscription:

                    # Endless loop
                    while datalayer_client.is_connected():
                        time.sleep(1.0)

                    subscription.unsubscribe_all()

    # Attention: Doesn't return if any provider or client instance is still running
    stop_ok = datalayer_system.stop(False)
    print("System Stop", stop_ok, flush=True)

    sys.exit(0)
