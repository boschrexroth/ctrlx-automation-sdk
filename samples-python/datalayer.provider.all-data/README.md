# ctrlX Data Layer Provider All Data

## Introduction

This Python app provides __ctrlX Data Layer__ nodes representing different data types.

## Function Description

When this app is started it connects to the __ctrlX Data Layer__ and creates two subtrees with data nodes.

The values of the nodes in the 'static' subtree cannot be changed by clients but they can be used to test reading different types of data and their numerical limits.

The values of the nodes in the 'dynamic' subtree can be changed (written) by clients.

## Implementation Description

__main.py__ starts the __ctrlX Data Layer__ system and the __ctrlX Data Layer__ provider. Furthermore two instances of the class ProviderNodeAllData are created. One instance is called for creating static (immutable) __ctrlX Data Layer__ nodes. The other instance is called for creating dynamic (changeable) __ctrlX Data Layer__ nodes.

The package __datalayerprovider__ contains the classes for data storage, configuration and __ctrlX Data Layer__ handling.

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT
