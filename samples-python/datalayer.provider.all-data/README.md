# ctrlX Data Layer Provider All Data

## Introduction

This Python app provides ctrlX Data Layer nodes representing different data types. 

## Function Description

When this app is started it connects to the ctrlX Data Layer and creates two subtrees with data nodes. 

The values of the nodes in the 'static' subtree cannot be changed by clients but they can be used to test reading different types of data and their numerical limits.

The values of the nodes in the 'dynamic' subtree can be changed (written) by clients.

## Implementation Description

__main.py__ starts the Data Layer system and the Data Layer provider. Furthermore two instances of the class ProviderNodeAllData are created. One instance is called for creating static (immutable) Data Layer nodes. The other instance is called for creating dynamic (changeable) Data Layer nodes.

The package __datalayerprovider__ contains the classes for data storage, configuration and Data Layer handling.

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
