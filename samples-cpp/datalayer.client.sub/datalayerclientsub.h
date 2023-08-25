/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <iostream>

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/metadata_generated.h"
#include "comm/datalayer/notify_info_generated.h"

class DataLayerClientSub final
{
private:
  comm::datalayer::DatalayerSystem m_datalayerSystem;

  comm::datalayer::IClient2* m_datalayerClient;

  comm::datalayer::PublishCallback publishCallback();

public:

  DataLayerClientSub(const comm::datalayer::DatalayerSystem& datalayerSystem);

  ~DataLayerClientSub();

  comm::datalayer::DlResult connect(const std::string& clientConnection);

  comm::datalayer::DlResult isConnected();

  comm::datalayer::DlResult createSubscriptionSync(const std::string& id);

  comm::datalayer::DlResult subscribeSync(const std::string& id, const std::string& address);

  comm::datalayer::DlResult subscribeSyncMulti(const std::string& id, const std::set<std::string>& addresses);

  comm::datalayer::DlResult unsubscribeSync(const std::string& id);

  void disconnect();
};
