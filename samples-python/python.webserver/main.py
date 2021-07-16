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
import time

from http.server import HTTPServer
from web.webserver import Server, UnixSocketHttpServer


def main():
    # Use unix sockets if app running as snap
    if 'SNAP' in os.environ:
        run_webserver_unixsock()
    else:
        run_webserver_tcp()
    

def run_webserver_tcp():
    serverPort = 1234
    hostname   = 'localhost'
    with HTTPServer(('',serverPort),Server) as webServer:
   
        print(time.asctime(),'Server UP - TCP/IP - ',hostname,':',serverPort)
        # Shutdown server after 100 request (auto-restart through snapcraft)
        for lp in range(100):
            webServer.handle_request()

        webServer.server_close()
        print(time.asctime(),'Server DOWN - TCP/IP')

def run_webserver_unixsock():
    
    sock_dir = os.getenv('SNAP_DATA') + '/package-run/rexroth-python-webserver/'
    sock_file   = sock_dir + 'web.sock'
    if not os.path.exists(sock_dir):
        os.makedirs(sock_dir)
    try: 
        os.unlink(sock_file)
    except OSError: pass
    
    with UnixSocketHttpServer(sock_file,Server) as webServer:
   
        print(time.asctime(),'Server UP - UNIX SOCKET - ',sock_file)
        # Shutdown server after 100 request (auto-restart through snapcraft)
        for lp in range(100):
            webServer.handle_request()

        webServer.server_close()
        os.remove(sock_file)
        print(time.asctime(),'Server DOWN - UNIX SOCKET')

if __name__ == '__main__':
    main()