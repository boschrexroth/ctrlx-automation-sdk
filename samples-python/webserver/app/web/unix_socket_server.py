# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

# Because http.server lib is designed for tcp, we need to write our own class to use unix sockets


from socketserver import UnixStreamServer


class UnixSocketServer(UnixStreamServer):
    """UnixSocketServer

    Args:
        UnixStreamServer (UnixStreamServer): socket server
    """

    def get_request(self):
        """get_request

        Returns:
            Request: request
        """
        request, _ = super().get_request()
        return (request, ["local", 0])
