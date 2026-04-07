# README datalayer.client

## Introduction

This python app runs __ctrlX Data Layer__ client operations like read, write and so on. It can be used as sample to implement own apps.

## Function Description

This Python app depends from the C++ sample samples-cpp/datalayer.provider.all-data.
It handles the __ctrlX Data Layer__ nodes created by the C++ sample.

## Implementation Description

__main.py__ creates the __ctrlX Data Layer__ connection and and an instance of CallDataLayerClient.

__app/call_datalayer_client.py__ contains the class CallDataLayerClient. This class runs all the data access activities.
___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT
