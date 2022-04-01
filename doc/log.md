# Logging 

## Introduction

During development, test phase and at runtime it is very useful - and in case of problems essential - that an application publishes messages. These messages can be informations, diagnostics or errors according the event which triggered the message. These messages should be displayed and stored uniform.

We recommend to use the build-in Linux systemd logging service called __journald.__

## The Using of Linux Kernel Log Levels

The interface to journald depends from the programming language. But for all these predefined Linux log levels (values) are common:

0. EMERGENCY    The system is unusable (and will be shut down)
1. ALERT        There is a severe error, action must be taken immediately
2. CRITICAL	    There is a critical error
3. ERROR		A job was aborted in case of an error
4. WARNING	    There is an abnormal condition but the actual job can be finished
5. NOTICE	    There is a normal but significant condition
6. INFO         A messages informs about an internal state
7. DEBUG	    Message shows details regarding program flow or results

__Be carefull with level EMERGENCY: Messages with this level are sent to all consoles!__


## Logging Samples in the SDK

In these folders of ctrlX SDK logging samples for different programming languages are provided:

- samples-cpp/diagnostics.logbook
- samples-net/diagnostics.logbook
- samples-python/python.logbook 


## Support

If you've any questions visit the [ctrlX AUTOMATION Community](https://developer.community.boschrexroth.com/)

___

## License

MIT License

Copyright (c) 2021 Bosch Rexroth AG

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
