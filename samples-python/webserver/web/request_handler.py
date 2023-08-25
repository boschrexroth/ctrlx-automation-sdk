# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import os
import traceback

import http.server

import web.web_token

from urllib.parse import unquote, parse_qs, urlparse
from json import dumps, loads

import app.datalayer

data_layer: app.datalayer.DataLayer


class RequestHandler(http.server.BaseHTTPRequestHandler):
    """RequestHandler

    Args:
        http (BaseHTTPRequestHandler): request handler

    """

    # Form parameters saved server side
    # "datalayer/subscriptions/settings"
    # "framework/metrics/system/cpu-utilisation-percent"
    readPath = "datalayer/subscriptions/settings"
    readValue = ""
    readResult = ""
    writePath = "sdk-py-provider-alldata/dynamic/int64"
    writeValue = "123456789"
    writeResult = ""

    def do_HEAD(self):
        """do_HEAD
        """
        return

    def get_www_file_path(self, relative_path):
        """def get_www_file_path(self, relative_path):


        Args:
            relative_path (string): relative path

        Returns:
            str: result path
        """
        # relative_path: 'www/xxx' or '/python-webserver/stylesheet.css'

        if len(relative_path) <= 0:
            relative_path = self.path

        print("get_www_file_path relative_path: %s", relative_path)

        rel_www_path = "/www/" + os.path.basename(relative_path)

        snap_path = os.getenv('SNAP')

        result_path = os.getcwd() + rel_www_path if snap_path is None else snap_path + rel_www_path

        print("get_www_file_path result_path:", result_path, flush=True)

        return result_path

    def send_response_and_header(self, response, content_type):
        """send_response_and_header

        Args:
            response (Response): Response
            content_type (str): content type
        """
        self.send_response(response)

        self.send_header('Content-type', content_type)
        self.end_headers()

    def send_file_response(self, content_type, rel_path=""):
        """send_file_response
        """
        path = self.get_www_file_path(rel_path)

        try:
            with open(path, "rb") as bufferedReader:
                self.send_response_and_header(200, content_type)
                self.wfile.write(bufferedReader.read())
        except Exception:
            print("EXCEPTION Opening and sending file:", path)
            print(traceback.format_exc(), flush=True)
            self.send_response_and_header(404, content_type)

    def send_html_file_response(self, rel_path=""):
        """send_html_file_response

        Args:
            rel_path (str, optional): path. Defaults to "".
        """
        path = self.get_www_file_path(rel_path)
        content_type = 'text/html'

        try:
            with open(path) as bufferedReader:
                self.send_response_and_header(200, content_type)
                self.wfile.write(bytes(bufferedReader.read(), 'utf-8'))
        except Exception:
            print("EXCEPTION opening and sending file:", path)
            print(traceback.format_exc(), flush=True)
            self.send_response_and_header(404, content_type)

    def do_GET(self):
        """do_GET
        """
        # GET Requests from client
        print("GET", self.path, flush=True)

        # Attempt To Send Different Files
        if self.path.endswith(".png"):
            # https://www.w3schools.com/html/html_favicon.asp
            # A favicon is a small image displayed next to the page title in the browser tab.
            # Browsers are caching this file
            self.send_file_response('image/png')
            return

        # Image jpg
        if self.path.endswith(".jpg"):
            self.send_file_response('image/jpg')
            return

        # Image gif
        if self.path.endswith(".gif"):
            self.send_file_response('image/gif')
            return

        # CSS
        if self.path.endswith(".css"):
            self.send_file_response('text/css')
            return

        # HTML
        if self.path.startswith("/python-webserver?token="):

            # Parsing bearer token from url
            parsedUrl = parse_qs(urlparse(self.path).query)
            token = ''
            if 'token' in parsedUrl:
                token = parsedUrl['token'][0]

            # check user permissions of token
            scopes_list = ["rexroth-device.all.rwx",
                           "rexroth-python-webserver.web.r", "rexroth-python-webserver.web.rw"]
            permissions_json = web.web_token.check_permissions(
                token, scopes_list)

            # if token is invalid
            if permissions_json is None:

                # self.write_www_file(content_type='text/html', rel_path='www/html-invalid-token.html')
                self.send_html_file_response(rel_path='www/invalid-token.html')
                return

            self.send_response_and_header(200, 'text/html')

            # Read HTML page from file and replace static with dynamic content
            path = self.get_www_file_path('/www/index.html')
            htmlX = open(path).read().replace('$(token)', token)

            # Enable/disable HTML objects according permissions
            permissions_read = permissions_json['rexroth-device.all.rwx'] or permissions_json[
                'rexroth-python-webserver.web.rw'] or permissions_json['rexroth-python-webserver.web.r']
            permissions_write = permissions_json['rexroth-device.all.rwx'] or permissions_json['rexroth-python-webserver.web.rw']
            htmlX = htmlX.replace(
                '$(permissions_read_text)', '' if permissions_read else "'disabled'")  # Text must be surrounded by ' '
            htmlX = htmlX.replace(
                '$(permissions_write_text)', '' if permissions_write else "'disabled'")  # Text must be surrounded by ' '

            # Set read content
            htmlX = htmlX.replace('$(Server.readPath)',
                                  str(RequestHandler.readPath))
            htmlX = htmlX.replace('$(Server.readValue)',
                                  str(RequestHandler.readValue))
            htmlX = htmlX.replace('$(Server.readResult)',
                                  str(RequestHandler.readResult))

            htmlX = htmlX.replace('$(Server.writePath)',
                                  str(RequestHandler.writePath))
            htmlX = htmlX.replace('$(Server.writeValue)',
                                  str(RequestHandler.writeValue))
            htmlX = htmlX.replace('$(Server.writeResult)',
                                  str(RequestHandler.writeResult))

            # Show permissions: 'True' or 'False'
            htmlX = htmlX.replace('$(permissions_rwx)', str(
                permissions_json['rexroth-device.all.rwx']))
            htmlX = htmlX.replace('$(permissions_rw)', str(
                permissions_json['rexroth-python-webserver.web.rw']))
            htmlX = htmlX.replace('$(permissions_r)', str(
                permissions_json['rexroth-python-webserver.web.r']))

            self.wfile.write(htmlX.encode("utf-8"))
            return

        self.send_response(404)

    def do_POST(self):
        """do_POST
        """
        # Get the size of data
        content_length = int(self.headers['Content-Length'])
        content = self.rfile.read(content_length).decode("utf-8")
        post_data = unquote(content)
        json = dumps(parse_qs(post_data))
        data = loads(json)

        print("POST", str(data), flush=True)

        # Evaluate post data
        if data['submit'][0] == 'Read Value':

            if 'node' in data:
                RequestHandler.readPath = data['node'][0]

                # ctrlX Data Layer access
                RequestHandler.readResult, RequestHandler.readValue = data_layer.read_node(
                    RequestHandler.readPath)

            else:
                RequestHandler.readValue = ''
                RequestHandler.readResult = 'INVALID NODE'

        if data['submit'][0] == 'Write Value':

            if 'node' in data and 'value' in data:
                RequestHandler.writePath = data['node'][0]
                RequestHandler.writeValue = data['value'][0]

                # ctrlX Data Layer access
                RequestHandler.writeResult = data_layer.write_node(
                    RequestHandler.writePath, RequestHandler.writeValue)

            else:
                RequestHandler.writeValue = ''
                RequestHandler.writeResult = 'INVALID NODE'

        self.send_response(303)
        self.send_header('Content-type', 'text/html')
        self.send_header('Location', self.path)
        self.end_headers()
