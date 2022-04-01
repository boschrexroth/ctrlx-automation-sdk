# ctrlX Data Layer Client Subscription

## Introduction


The code shows how in Python the values of the ctrlX Data Layer are subscribed. Code snippets can be used in your own Python app.

## Function Description

The app uses a single node and a multi node subscription. 

First the single node subscription is started. It will be stopped after a short period of time. 

Then the multi node subscription is started. It will run until a connection break to the Data Layer is detected. In this case the app unsubscribes the multi node subscription and ends. The app is restarted automatically by snapd.


## Implementation Description

__main.py__ is the single module. It has following tasks:
* Create a Data Layer client connection
* Subscribe/unsubscribe a Data Layer single node subscription
* Subscribe/unsubscribe a Data Layer multi node subscription
* Close app on Data Layer client connection break

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
