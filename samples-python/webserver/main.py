#!/usr/bin/env python3

# MIT License
#
# Copyright (c) 2020-2021 Bosch Rexroth AG
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

import os
import sys
import threading

import http.server
import web.request_handler
import web.unix_socket_server

import app.datalayer


httpServerPort = 12345
token = 'eyJhbGciOiJFUzM4NCIsInR5cCI6IkpXVCJ9.eyJleHAiOjE2NjAxNDg3NzQsImlhdCI6MTY2MDExOTk3NCwiaWQiOiIxMDAwIiwibmFtZSI6ImJvc2NocmV4cm90aCIsIm5vbmNlIjoiMGU0NTVhODMtMThlOC00YjY2LTllMWUtYTE0NWM2ZWIzZWQzIiwicGxjaGFuZGxlIjowLCJyZW1vdGVhdXRoIjoiIiwic2NvcGUiOlsicmV4cm90aC1kZXZpY2UuYWxsLnJ3eCJdfQ.VqCCRh2ga1Ujn5C_vBAf7dZHXNr6gY0Aqvrwu39_6L9d7fWBYXr-MmqdYxGB85fHBhs56MFrCacYjN5SbctqSyH1LTeXLKAdP4Etx8V7B2QB_5XZdVCLqIwYOAU8Gdzv'

def main():

    web.request_handler.data_layer = app.datalayer.DataLayer()
    web.request_handler.data_layer.start()

    client, connection_string = web.request_handler.data_layer.connect_client(ip="10.0.2.2", https_port=8443)
    if client == None:
        print("ERROR Could not connect", connection_string, flush=True)
        return

    new_thread = threading.Thread(target=thread_start)
    new_thread.start()

    new_thread.join()

    web.request_handler.data_layer.stop()

    sys.exit(0)


def thread_start():

    # If running with a snap (on a ctrlX) start UNIX socket
    # If running as app in an app builder envorinemtn start a TCP server
    run_webserver_unixsock() if 'SNAP' in os.environ else run_webserver_tcp()


def run_webserver_tcp():

    with http.server.HTTPServer(('', httpServerPort), web.request_handler.RequestHandler) as http_server:

        print('TCP/IP server started - listening on 0.0.0.0:', httpServerPort)
        print("")
        print(
            '------------------Copy this link into the address field of your browser ----------------------')
        print('http://localhost:'+str(httpServerPort) +
              '/python-webserver?token='+token)
        print(
            '----------------------------------------------------------------------------------------------', flush=True)

        http_server.serve_forever()

        http_server.server_close()


def run_webserver_unixsock():

    sock_dir = os.getenv('SNAP_DATA') + '/package-run/sdk-py-webserver/'
    if not os.path.exists(sock_dir):
        os.makedirs(sock_dir)

    sock_file = sock_dir + 'web.sock'

    try:
        os.unlink(sock_file)
    except OSError:
        pass

    with web.unix_socket_server.UnixSocketServer(sock_file, web.request_handler.RequestHandler) as http_server:

        print('UNIX SOCKET server started - listening on', sock_file, flush=True)

        http_server.serve_forever()

        http_server.server_close()
        os.remove(sock_file)


if __name__ == '__main__':
    main()
