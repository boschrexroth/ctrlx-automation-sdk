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

#include "sampleSchema_generated.h"

class DataLayerClient final
{
private:
  std::string m_dataLayerAddressDynamic = "sdk/cpp/datalayer/provider/alldata/dynamic/";
  std::string m_dataLayerAddressStatic = "sdk/cpp/datalayer/provider/alldata/static/";
  std::string m_ip;
  std::string m_user;
  std::string m_password;
  int m_sslPort;
  comm::datalayer::IClient* m_client;
  comm::datalayer::DlResult m_result;
  comm::datalayer::DlResult m_resultAsync;
  comm::datalayer::Variant m_data;
  comm::datalayer::Variant m_dataAsync;

  comm::datalayer::DatalayerSystem m_datalayerSystem;

  comm::datalayer::IClient::ResponseCallback responseCallback();

  void println(const std::string& text, comm::datalayer::DlResult result, comm::datalayer::Variant* data);
  comm::datalayer::DlResult print(comm::datalayer::Variant* data);

  bool waitForResponseCallback(int counter);
  void readSync(const std::string& node);
  void writeSync(const std::string& node);
  void createSync(const std::string& node);

  bool start();
  void ping();
  void read();
  void create();
  void remove();
  void browse();
  void write();
  void metadata();
  void stop();

public:
  DataLayerClient(const std::string& ip = "192.168.1.1", const std::string& user = "boschrexroth", const std::string& password = "boschrexroth", int sslPort = 443);
  ~DataLayerClient() = default;
  void run();
};
