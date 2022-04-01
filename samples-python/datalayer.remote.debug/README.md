# Sample Project datalayer.remote.debug

The remote debugging of snaps written in Python is described here [Debugging](../../doc/debug.md), chapter "Python". 

## Enabling Remote Debugging

Remote debugging is enabled when the app is called with a command line parameter --debug-port=....
If this parameter is missing the debugging functions are disabled, the app starts normally.

For debugging in the build environment this parameter doesn't matter.

## Python Package debugpy and Script debugging.py

We use the Python package __debugpy__ for remote debugging. 

To simplify the usage of this package the debugging code is outsourced into the script debugging.py.

To use debugpy and the script in your own Python projects too install the package into your project environment, copy the script debugging.py into your project folder and add the function calls into your code.

___

## License

MIT License

Copyright (c) 2021-2022 Bosch Rexroth AG

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
