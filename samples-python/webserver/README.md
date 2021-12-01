# ctrlX Data Layer + Webserver

## Introduction

This python app deploys a simple webserver including Data Layer connection and user permissions with webinterface accessible from ctrlX webpage.

## Function Description

The app runs a simple webserver with reverse proxy and Data Layer access, setup user permissions through package-manifest and token authentification.

Once the installation of the snap was finished, you can access to the provided webpage through the sidebar or overview menu of ctrlX CORE webpage.

![ctrlX Integration](./docs/images/ctrlx_integration.png)


The Python-Webpage allows you to read and write at the Data Layer (depends on permissions of the current user) and shows permissions of the current user. 

![Python Webserver](./docs/images/python_webserver.png)

You can find the configuration of user permissions (scopes) at settings section <ins>Users & Permissions > Users</ins>

![ctrlX User Permissions](./docs/images/ctrlx_userpermissions.png)

The integration of the app inside ctrlX Core webpage (sidebar and overview integration, reverse proxy, bearertoken, scopes) is done by the package-manifest (<ins>./configs/package-assets/rexroth-python-webserver.package-manifest.json</ins>). Take a look at [package-manifest](./../package-manifest.md) to find out the basics.


## Implementation Description

The app has these modules:

* __main.py__ starts the web server. In snap environment an Unix socket connection is established otherwise a TCP/IP connectin.
* __web/webserver.py__ contains the web server implementation. Internally he Python package http.server is used.
* __web/webtoken.py__  decodes the token string using the Python package jwt
* __app/datalayerclient.py__ handles the Data Layer node data access.
___

## License

MIT License

Copyright (c) 2020-2021 Bosch Rexroth AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
