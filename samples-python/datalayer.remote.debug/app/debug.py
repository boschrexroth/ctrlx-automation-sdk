#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

""" Script to use debug.py

This module demonstrates the usage of debug.py.

Usage:
    Copy it into your project folder.
    Insert the statement 'import debug'
    Call   debug.remote_debugging_wait_for_client(port=xxx)

Were xxx is the port number where debug.py is listening e.g. 15678
"""

import debugpy

def remote_debugging_wait_for_client(port: int):
    """Waits until a remote debug client connects.
    Detailed functions:
    - Set the listen parameters: '0.0.0.0' means 'listen on all network interfaces'
    - Start the debug server
    - Wait until debug client connects.

    Parameters:
    port (int): Port number for the debug connection

    Returns:
    No value, the function blocks and returns if a debug client has been connected.

    """

    print('Accepting remote debug client attaches to port', port, flush=True)
    address = ('0.0.0.0', port) # Accept incomming calls from all network interfaces
    debugpy.listen(address)

    debugpy.wait_for_client()
    print('Debugger is attached!', flush=True)

    debugpy.breakpoint()

