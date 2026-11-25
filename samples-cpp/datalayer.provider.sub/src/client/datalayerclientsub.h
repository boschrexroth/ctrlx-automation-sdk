/*
* SPDX-FileCopyrightText: Bosch Rexroth AG
*
* SPDX-License-Identifier: MIT
*/

#include <stdio.h>
#include <iostream>

#include <comm/datalayer/datalayer.h>
#include <comm/datalayer/datalayer_system.h>
#include <comm/datalayer/metadata_generated.h>
#include <comm/datalayer/notify_info_generated.h>

// DataLayerClientSub manages a ctrlX Data Layer client connection and provides
// synchronous subscription management for monitoring node value changes.
class DataLayerClientSub final
{
private:
  // Reference to the shared Data Layer system used to create client instances
  comm::datalayer::DatalayerSystem &m_datalayerSystem;

  // The underlying Data Layer client instance (IClient3 supports subscriptions)
  std::shared_ptr<comm::datalayer::IClient3> m_datalayerClient;

  // Returns the publish callback that is invoked whenever subscribed node values change
  comm::datalayer::PublishCallback publishCallback();

public:

  // Constructor: stores a reference to the Data Layer system
  DataLayerClientSub(comm::datalayer::DatalayerSystem &system);

  // Destructor: ensures the client is disconnected when the object is destroyed
  ~DataLayerClientSub();

  // Connects to the Data Layer using the provided connection string
  comm::datalayer::DlResult connect(const std::string& connection);

  // Returns DL_OK if the client is currently connected, DL_CLIENT_NOT_CONNECTED otherwise
  comm::datalayer::DlResult isConnected();

  // Creates a named subscription with default sampling and keep-alive settings
  comm::datalayer::DlResult createSubscriptionSync(const std::string& id);

  // Adds a single Data Layer node address to an existing subscription
  comm::datalayer::DlResult subscribeSync(const std::string& id, const std::string& address);

  // Removes a subscription and stops receiving change notifications for it
  comm::datalayer::DlResult unsubscribeSync(const std::string& id);

  // Disconnects from the Data Layer and releases the client instance
  void disconnect();
};
