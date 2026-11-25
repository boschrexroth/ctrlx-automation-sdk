/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <iostream>
#include <memory>
#include <future>

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/metadata_generated.h"

#include "sampleSchema_generated.h"

// Manages a ctrlX Data Layer client connection and demonstrates synchronous
// and asynchronous read, write, create, remove, browse, and metadata operations.
class DataLayerClient final
{
private:
  // Base addresses for dynamic (client-created) and static (provider-owned) nodes
  std::string m_dataLayerAddressDynamic = "sdk/cpp/datalayer/provider/alldata/dynamic/cpp.client.alldata/";
  std::string m_dataLayerAddressStatic = "sdk/cpp/datalayer/provider/alldata/static/";

  // Connection parameters
  std::string m_ip;
  std::string m_user;
  std::string m_password;
  int m_sslPort;

  // Owning handle to the Data Layer client instance
  std::unique_ptr<comm::datalayer::IClient3> m_client;

  // Result of the last synchronous operation
  comm::datalayer::DlResult m_result;
  // Result of the last asynchronous operation (set inside the response callback)
  comm::datalayer::DlResult m_resultAsync;

  // Data buffer for synchronous operations
  comm::datalayer::Variant m_data;
  // Data buffer filled by the asynchronous response callback
  comm::datalayer::Variant m_dataAsync;

  comm::datalayer::DatalayerSystem m_datalayerSystem;

  // Returns a lambda that stores the async result and signals the given promise
  comm::datalayer::IClient::ResponseCallback responseCallback(std::promise<void>& promise);

  // Prints node address, operation result, and data value to stdout
  void println(const std::string& text, comm::datalayer::DlResult result, comm::datalayer::Variant* data);
  // Prints the value of a Variant according to its type; returns DL_UNSUPPORTED for unknown types
  comm::datalayer::DlResult print(comm::datalayer::Variant* data);

  // Blocks until the promise is fulfilled or the timeout (counter seconds) expires
  bool waitForResponseCallback(int counter, std::promise<void>& promise);

  // Synchronous helper wrappers for a single named node
  void readSync(const std::string& node);
  void writeSync(const std::string& node);
  void createSync(const std::string& node);

  // Lifecycle and operation steps called from run()
  bool start();    // Starts the Data Layer system and connects the client
  void ping();     // Sends sync and async ping requests
  void read();     // Reads all static nodes (sync + async)
  void create();   // Creates dynamic nodes with sample values
  void remove();   // Creates and removes a temporary node (sync + async)
  void browse();   // Browses the root node (sync + async)
  void write();    // Writes updated values to dynamic nodes
  void metadata(); // Reads and prints metadata for a sample node
  void stop();     // Releases the client connection

public:
  // Constructs the client with optional connection parameters
  DataLayerClient(const std::string& ip = "192.168.1.1", const std::string& user = "boschrexroth", const std::string& password = "boschrexroth", int sslPort = 443);
  ~DataLayerClient() = default;

  // Executes the full demonstration sequence: connect, operate, disconnect
  void run();
};
