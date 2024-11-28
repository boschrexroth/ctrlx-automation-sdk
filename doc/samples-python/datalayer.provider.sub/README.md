# ctrlX Data Layer Provider

The app demonstrates shows how a provider supports "ctrlX Data Layer" subscription interface.

## Function Description

Provider supported the subscription interface.

## Preparation

## Implementation Description

### Definition Files

### Modules

* The module __provider.py__ manages the ctrlX Data Layer connection and the creation of the ctrlX Data Layer node.
* The module __helper/ctrlx_datalayer_helper.py__ Provides methods to easily establish a ctrlX Data Layer connection.
* The module __datalayerprovider/sub_provider_node.py__ contains the class SubProviderFolderNode with the callback function needed by the ctrlX Data Layer.
* The module __client.py__ simple ctrlX Data Layer client uses a single node subscription.
* The module __datalayerclient/app.py__ helper classes and function for the client.

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT
