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
import getopt

import debugpy

debugging_enabled = False

def breakpoint():
    """Set a breakpoint.
    This function can be used to set a fix breakpoint into the programming code. 
    The call is ineffective if debug is not activated (by providing a valid port number via command line parameter)
    """
    if debugging_enabled:
        debugpy.breakpoint()


def wait_for_client(port: int):
    """Waits until a debug client connects.
    Detailed functions:
    - Set the listen parameters: '0.0.0.0' means listen on all network interfaces
    - Start the debug server
    - Wait until debug client connects.

    Parameters:
    port (int): Port number for the debug connection

    Returns:
    No value, the function blocks and returns if a debug client has been connected.

    """

    address = ('0.0.0.0', port) # Accept incomming calls from all network interfaces
    print('Accepting remote debug client attaches to port', port, flush=True)
    debugpy.listen(address)

    debugpy.wait_for_client()
    print('Debugger is attached!', flush=True)

    breakpoint()

def init():
    """This function initializes remote debugging. It runs following tasks:
    
    Scan the command line parameter list for the option 'debug-port'. 
    If this option is not provided debugging will not be activated otherwise the global flag debugging_enabled is set to True.
    If is debugging the wait_for_client function is called.
    """

    port = 0

    try:
        opts, args = getopt.gnu_getopt(sys.argv[1:], '', ['debug-port='])
    except getopt.GetoptError:
        return
    
    for opt, arg in opts:
        if opt in ("--debug-port"):
            try:
                port = int(arg)
            except:
              return

    if port == 0:
        return

    global debugging_enabled
    debugging_enabled = True

    wait_for_client(port)

