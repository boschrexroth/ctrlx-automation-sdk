# README datalayer.client

## Introduction

This python app runs Data Layer client operations like read, write and so on. It can be used as sample to implement own apps.

## Function Description

This Python app depends from the C++ sample samples-cpp/datalayer.provider.all-data. 
It handles the Data Layer nodes created by the C++ sample.

## Implementation Description 

__main.py__ creates the Data Layer connection and and an instance of CallDataLayerClient.

__app/call_datalayer_client.py__ contains the class CallDataLayerClient. This class runs all the data access activities.
___

## License

MIT License

Copyright (c) 2020-2022 Bosch Rexroth AG

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
