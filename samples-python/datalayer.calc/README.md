# ctrlX Data Layer Calculation operations 

## Introduction

This app is a ctrlX Data Layer Provider and Client written in Python.

## Function Description

The app reads two Data Layer values and calculates the output value 'out'. All values are float64 value types.

The Data Layer addresses of the input values are configured by the string input values 'in1' and 'in2'.

How the out value is calculated can be configured as string value of the node 'mode'.

 Supported operations are:

- +
- -
- *
- /

## Implementation Description 

There are two Python scripts:

- main.py - the main function
- calculations.basic_arithmetic_operations.py - Class called BasicArithmeticOperations which reads the input values and calculates the output value.

### main.py

This function manages the Data Layer Provider and Client connection.

Furthermore it creates one instance of the BasicArithmeticOperations class and starts it.

If the client connection is lost the main function exits with an error code.

If the app is installed as snap the system will restart the application after a short period of time.

### calculations.basic_arithmetic_operations.py

This class BasicArithmeticOperations has several functions.

It is a Data Layer Provider for four nodes: 
1. in1: string, specifies the DL address of the first operand
2. in2: string, specifies the DL address of the second operand
3. mode: string, specifies the operation
4. out: float64, the result

!!! important
    The operands have to be 'double' values (FLOAT64)!
    
With the address values of in1 and in2 a subscription is started. Every time when new values are published the value of out is recalculated.

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
