#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import os
from http.server import HTTPServer

from web.server import Server, UnixSocketHttpServer


def main():
    """main
    """
    # Start webserver to get load/save rest requests
    # Use unix sockets if app is running as snap
    if 'SNAP' in os.environ:
        webserver = create_webserver_unixsock()
    else:
        webserver = create_webserver_tcp()

    webserver.serve_forever()

    print("Server DOWN", flush=True)


def create_webserver_tcp():
    """create_webserver_tcp
    """
    serverPort = 1234
    hostname = 'localhost'

    webserver = HTTPServer(('', serverPort), Server)
    print("Server started, listening on TCP SOCKET", hostname, ':', serverPort, flush=True)
    return webserver


def create_webserver_unixsock():
    """create_webserver_unixsock
    """
    sock_dir = os.getenv('SNAP_DATA') + '/package-run/sdk-py-appdata/'
    sock_file = sock_dir + 'web.sock'
    if not os.path.exists(sock_dir):
        os.makedirs(sock_dir)
    try:
        os.unlink(sock_file)
    except OSError:
        pass

    webserver = UnixSocketHttpServer(sock_file, Server)
    print('Server started, listening on UNIX SOCKET', sock_file, flush=True)
    return webserver


if __name__ == '__main__':
    main()
