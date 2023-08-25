# ctrlX Data Layer Calculation operations 

## Introduction

This app is a ctrlX Data Layer Provider and Client written in Python.

## Function Description

The app reads two ctrlX Data Layer values and calculates the output value 'out'. All values are float64 value types.

The ctrlX Data Layer addresses of the input values are configured by the string input values 'in1' and 'in2'.

How the out value is calculated can be configured as string value with the node 'mode'.

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

This function manages the ctrlX Data Layer Provider and Client connection.

Furthermore it creates one instance of the BasicArithmeticOperations class and starts it.

If the client connection is lost the main function exits with an error code. If the app is installed as snap the system will restart the application after a short period of time.

### calculations.basic_arithmetic_operations.py

This class BasicArithmeticOperations has several functions.

It is a ctrlX Data Layer Provider for four nodes: 
1. in1: string, specifies the DL address of the first operand
2. in2: string, specifies the DL address of the second operand
3. mode: string, specifies the operation
4. out: float64, the result

!!! important
    The operands have to be __double__ values (FLOAT64)!
    
With the address values of in1 and in2 a subscription is started. Every time when new values are published the value of 'out' is recalculated.

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT
