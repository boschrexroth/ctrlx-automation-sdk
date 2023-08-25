# ctrlX Data Layer Client Subscription

## Introduction


The code shows how in Python the values of the ctrlX Data Layer are subscribed. Code snippets can be used in your own Python app.

## Function Description

The app uses a single node and a multi node subscription. 

First the single node subscription is started. It will be stopped after a short period of time. 

Then the multi node subscription is started. It will run until a connection break to the ctrlX Data Layer is detected. In this case the app unsubscribes the multi node subscription and ends. The app is restarted automatically by snapd.


## Implementation Description

__main.py__ is the single module. It has following tasks:
* Create a ctrlX Data Layer client connection
* Subscribe/unsubscribe a ctrlX Data Layer single node subscription
* Subscribe/unsubscribe a ctrlX Data Layer multi node subscription
* Close app on ctrlX Data Layer client connection break

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT
