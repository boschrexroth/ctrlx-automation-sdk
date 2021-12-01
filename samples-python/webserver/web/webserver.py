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

from http.server import BaseHTTPRequestHandler
from socketserver import UnixStreamServer

import app.datalayerclient
import web.webtoken

from urllib.parse import unquote, parse_qs, urlparse
from json import dumps, loads


class Server(BaseHTTPRequestHandler):

    # Form parameters saved server side
    readPath    = "framework/metrics/system/cpu-utilisation-percent"
    readValue   = ""
    readResult  = ""
    writePath   = ""
    writeValue  = ""
    writeResult = ""

    def do_HEAD(self):
        return
        
    def do_GET(self):
        # GET Requests from client
        self.send_response(200)

        # Attempt To Send Different Files
        if self.path.startswith("/favicon.ico"):
            self.send_header('Content-type','text/html')
            self.end_headers()
            return
        # Image jpg
        elif self.path.endswith(".jpg"):
            self.send_header('Content-type','image/jpg')
            self.end_headers()
            path = os.getcwd() + "/www/" + self.path.rsplit("/", 1)[1]
            snap_path = os.getenv('SNAP')
            if snap_path is not None:
                path = snap_path + "/www/"+ self.path.rsplit("/", 1)[1]
            with open(path, "rb") as image:
                self.wfile.write(image.read())
            return
        # Image gif
        elif self.path.endswith(".gif"):
            self.send_header('Content-type','image/gif')
            self.end_headers()

            path = os.getcwd() + "/www/"+ self.path.rsplit("/",1)[1]
            snap_path = os.getenv('SNAP')
            if snap_path is not None:
                path = snap_path + "/www/"+ self.path.rsplit("/",1)[1]
            with open(path, "rb") as image:
                self.wfile.write(image.read())
            return
        # CSS
        elif self.path.endswith(".css"):
            self.send_header('Content-type','text/css')
            self.end_headers()
            path = os.getcwd() + "/www/"+ self.path.rsplit("/",1)[1]
            snap_path = os.getenv('SNAP')
            if snap_path is not None:
                path = snap_path + "/www/"+ self.path.rsplit("/",1)[1]
            with open(path, "rb") as stylesheet:
                self.wfile.write(stylesheet.read())
            return
        # HTML
        elif self.path.startswith("/python-webserver"):
            self.send_header('Content-type','text/html')
            self.end_headers()

            # Parsing bearertoken from url 
            parsedUrl = parse_qs(urlparse(self.path).query)
            token = ''
            if 'token' in parsedUrl:
                token = parsedUrl['token'][0]

            # check user permissions of token
            scopes_list = ["rexroth-device.all.rwx","rexroth-python-webserver.web.r","rexroth-python-webserver.web.rw"]
            permissions_json = web.webtoken.checkPermissions(token, scopes_list )


            # if token is invalid
            if permissions_json is None : 

                # Build html error page
                html=   """
                    <!DOCTYPE html>
                    <html style="font-family: Arial">
                        <meta charset="UTF-8">
                        <meta name="viewport" content="width=device-width, initial-scale=1">
                        <head>
                            <title>ctrlX Python Webserver Sample</title>
                        </head>
                    <body> INVALID TOKEN </body>
                """
                self.wfile.write(html.encode("utf-8"))
                return
            
            # Set permissions from json    
            permissions_read = permissions_json['rexroth-device.all.rwx'] or permissions_json['rexroth-python-webserver.web.rw'] or permissions_json['rexroth-python-webserver.web.r']
            permissions_write = permissions_json['rexroth-device.all.rwx'] or permissions_json['rexroth-python-webserver.web.rw']
            
            # Build html
            html = f"""
                <!DOCTYPE html>
                <html style="font-family: Arial">
                    <meta charset="UTF-8">
                    <meta name="viewport" content="width=device-width, initial-scale=1">
                    <link rel="stylesheet" href="/python-webserver/stylesheet.css">
                    <head>
                    <title>ctrlX Python Webserver Sample</title>
                    </head>
                    <body>
                        <header>
                            <h1 fontcolor="#e7913c">PYTHON WEBSERVER SAMPLE</h1>
                        </header>
                        <div class="main">
                            <div class="box">
                                <p><b>Read Data Layer Node</b></p>
                                <form action="/python-webserver?token={token}" method="POST">
                                    <p>Node: <input id="textbox" type="text" name="node" value="{Server.readPath}" {'' if permissions_read else 'disabled' }> <input id="button" type="submit" name="submit" value="Read Value" {'' if permissions_read else 'disabled' }> </p>
                                    <p>Value: {Server.readValue} </p>
                                    <p>Result: {Server.readResult} </p>
                                </form>
                            </div>
                            <div class="box">
                                <p><b>Write Data Layer Node</b></p>
                                <form action="/python-webserver?token={token}" method="POST">
                                    <p>Node: <input id="textbox" type="text" name="node" value="{Server.writePath}" {'' if permissions_write else 'disabled' }> </p>
                                    <p>Value: <input id="textbox" type="text" name="value" value="{Server.writeValue}" {'' if permissions_write else 'disabled' }> <input id="button" type="submit" name="submit" value="Write Value" {'' if permissions_write else 'disabled' }> </p>
                                    <p>Result: {Server.writeResult}  </p>
                                </form>
                            </div>
                            <div class="box">
                                <p><b>User Permissions</b></p>
                                    <p>Full access: {permissions_json['rexroth-device.all.rwx']} </p>
                                    <p>Read/Write: {permissions_json['rexroth-python-webserver.web.rw']} </p>
                                    <p>Read: {permissions_json['rexroth-python-webserver.web.r']}  </p>
                            </div>
                        </div>
                    </body>
                </html>
            """
            self.wfile.write(html.encode("utf-8"))
  

    def do_POST(self):
        # POST Requests from client (submit buttons)
        # Read header data
        headerdata = self.headers
        content_length = int(self.headers['Content-Length'])    # Get the size of data
        post_data = unquote(self.rfile.read(content_length).decode("utf-8"))
        json = dumps(parse_qs(post_data))
        data = loads(json)
    
        # Evaluate post data
        if data['submit'][0] == 'Read Value':

            if 'node' in data:
                Server.readPath = data['node'][0]

                # Data Layer access
                Server.readResult,Server.readValue = app.datalayerclient.read_node(Server.readPath)
                
            else:
                Server.readValue = ''
                Server.readResult = 'INVALID NODE'

        if data['submit'][0] == 'Write Value':

            if 'node' in data and 'value' in data:
                Server.writePath = data['node'][0]
                Server.writeValue = data['value'][0]
                
                # Data Layer access
                Server.writeResult = app.datalayerclient.write_node(Server.writePath,Server.writeValue)

            else:
                Server.writeValue = ''
                Server.writeResult = 'INVALID NODE'

        
        self.send_response(303)
        self.send_header('Content-type', 'text/html')
        self.send_header('Location', self.path)
        self.end_headers()
       
# Because http.server lib is designed for tcp, we need to write our own class to use unix sockets
class UnixSocketHttpServer(UnixStreamServer):
    allow_reuse_address = True
    def get_request(self):
        # dummy client address to avoid errors
        request, client_address = super().get_request()
        return (request, ["local",0])
