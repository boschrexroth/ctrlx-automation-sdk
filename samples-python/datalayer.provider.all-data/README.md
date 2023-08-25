# ctrlX Data Layer Provider All Data

## Introduction

This Python app provides ctrlX Data Layer nodes representing different data types. 

## Function Description

When this app is started it connects to the ctrlX Data Layer and creates two subtrees with data nodes. 

The values of the nodes in the 'static' subtree cannot be changed by clients but they can be used to test reading different types of data and their numerical limits.

The values of the nodes in the 'dynamic' subtree can be changed (written) by clients.

## Implementation Description

__main.py__ starts the ctrlX Data Layer system and the ctrlX Data Layer provider. Furthermore two instances of the class ProviderNodeAllData are created. One instance is called for creating static (immutable) ctrlX Data Layer nodes. The other instance is called for creating dynamic (changeable) ctrlX Data Layer nodes.

The package __datalayerprovider__ contains the classes for data storage, configuration and ctrlX Data Layer handling.

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT
