# ctrlX Data Layer Provider Subscription

The app demonstrates shows how a provider supports "__ctrlX Data Layer__" subscription interface.

## Function Description

Provider supported the subscription interface.

## Preparation

## Implementation Description

### Definition Files

### Modules

* The module __provider.py__ manages the __ctrlX Data Layer__ connection and the creation of the __ctrlX Data Layer__ node.
* The module __helper/ctrlx_datalayer_helper.py__ Provides methods to easily establish a __ctrlX Data Layer__ connection.
* The module __datalayerprovider/sub_provider_node.py__ contains the class SubProviderFolderNode with the callback function needed by the __ctrlX Data Layer__.
* The module __client.py__ simple __ctrlX Data Layer__ client uses a single node subscription.
* The module __datalayerclient/app.py__ helper classes and function for the client.

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT
