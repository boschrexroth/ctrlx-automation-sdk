/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <iostream>
#include <future>
#include <chrono>
#include <memory>

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"

static void run_sync(comm::datalayer::IClient3 *dataLayerClient);
static bool run_sync_read(comm::datalayer::IClient3 *dataLayerClient);
static bool run_sync_browse(comm::datalayer::IClient3 *dataLayerClient);
static bool run_sync_metadata(comm::datalayer::IClient3 *dataLayerClient);
static void run_async(comm::datalayer::IClient3 *dataLayerClient);
static bool run_async_read(comm::datalayer::IClient3 *dataLayerClient);
static bool run_async_browse(comm::datalayer::IClient3 *dataLayerClient);
static bool run_async_metadata(comm::datalayer::IClient3 *dataLayerClient);
static void run(comm::datalayer::IClient3 *dataLayerClient);

static void printStringList(comm::datalayer::Variant &data);
static void printMetadata(comm::datalayer::Variant &data);

//! Retrieve environment variable SNAP
//! @result The content of SNAP ales nullptr if not available
static const char *snapPath()
{
  return std::getenv("SNAP");
}

//! Test if code is runnning in snap environment
//! @result True if running snap environment
static bool isSnap()
{
  return snapPath() != nullptr;
}

//! Get Datalayer connection string
//! @param[in] ip       IP address of the ctrlX CORE: 10.0.2.2 is ctrlX COREvirtual with port forwarding
//! @param[in] user     User name
//! @param[in] password The password
//! @param[in] sslPort  The port number for SSL: 8443 if ctrlX COREvirtual with port forwarding 8443:443
//! @result Connection string
static std::string getConnectionString(
    const std::string &ip = "192.168.1.1",
    const std::string &user = "boschrexroth",
    const std::string &password = "boschrexroth",
    int sslPort = 443)
{
  if (isSnap())
  {
    return DL_IPC;
  }

  std::string connectionString = DL_TCP + user + std::string(":") + password + std::string("@") + ip;

  if (443 == sslPort)
  {
    return connectionString;
  }

  return connectionString + std::string("?sslport=") + std::to_string(sslPort);
}

//! Print the string list of a Variant containing an array of strings
//! @param[in] data  Variant holding the array of strings to print
static void printStringList(comm::datalayer::Variant &data)
{
  if (data.getType() == comm::datalayer::VariantType::ARRAY_OF_STRING)
  {
    std::cout << "Node List: ";
    const char **strArray = data;
    for (uint32_t i = 0; i < data.getCount(); i++)
    {
      std::cout << strArray[i] << " ";
    }
    std::cout << std::endl;
  }
}

//! Print metadata information from a Variant containing a Metadata flatbuffer
//! @param[in] data  Variant holding the serialized Metadata flatbuffer
static void printMetadata(comm::datalayer::Variant &data)
{
  if (STATUS_FAILED(data.verifyFlatbuffers(comm::datalayer::VerifyMetadataBuffer)))
  {
    std::cout << "Invalid Flatbuffer: Not a MetadataBuffer";
    return;
  }

  auto metadata = comm::datalayer::GetMetadata(data.getData());
  if (metadata->displayName() != nullptr && metadata->displayName()->size() > 0)
  {
    std::cout << "Display Name:   " << metadata->displayName()->c_str() << std::endl;
  }

  if (metadata->description() != nullptr && metadata->description()->size() > 0)
  {
    std::cout << "Description:    " << metadata->description()->c_str() << std::endl;
  }

  if (metadata->descriptionUrl() != nullptr && metadata->descriptionUrl()->size() > 0)
  {
    std::cout << "Description URL: " << metadata->descriptionUrl()->c_str() << std::endl;
  }

  std::cout << "Display Format:  " << metadata->displayFormat() << std::endl;

  auto operations = metadata->operations();
  std::cout << "AllowedOperations: read=" << operations->read() << " write:" << operations->write() << " create:" << operations->create() << " delete:" << operations->delete_() << std::endl;

  auto references = metadata->references();
  if (references != nullptr)
  {
    for (flatbuffers::uoffset_t i = 0; i < references->size(); i++)
    {
      if (references->Get(i)->type() != nullptr)
      {
        std::cout << references->Get(i)->type()->c_str() << std::endl;
      }
      if (references->Get(i)->targetAddress() != nullptr)
      {
        std::cout << references->Get(i)->targetAddress()->c_str() << std::endl;
      }
    }
  }
}

//! Perform a synchronous bulk read of multiple Data Layer nodes
//! @param[in] dataLayerClient  Connected Data Layer client instance
//! @result True on success, false on failure
static bool run_sync_read(comm::datalayer::IClient3 *dataLayerClient)
{
  comm::datalayer::VecBulkRequest request;
  comm::datalayer::VecBulkResponse response;

  request.push_back({"framework/metrics/system/cpu-utilisation-percent"});
  request.push_back({"framework/metrics/system/memavailable-mb"});

  auto result = dataLayerClient->readBulkSync(request, response);
  if (result != DL_OK)
  {
    std::cout << "INFO readBulkSync result: " << result.toString() << std::endl;
    return false;
  }
  for (size_t i = 0; i < response.size(); i++)
  {
    std::cout << "INFO readBulkSync response " << i << ": address=" << response[i].address << " result=" << response[i].result.toString() << " data: " << double(response[i].data) << std::endl;
  }
  return true;
}

//! Perform a synchronous bulk browse of multiple Data Layer nodes
//! @param[in] dataLayerClient  Connected Data Layer client instance
//! @result True on success, false on failure
static bool run_sync_browse(comm::datalayer::IClient3 *dataLayerClient)
{
  comm::datalayer::VecBulkRequest request;
  comm::datalayer::VecBulkResponse response;

  request.push_back({"framework/metrics/system"});
  request.push_back({"framework/metrics/process"});

  auto result = dataLayerClient->browseBulkSync(request, response);
  if (result != DL_OK)
  {
    std::cout << "INFO browseBulkSync result: " << result.toString() << std::endl;
    return false;
  }
  for (size_t i = 0; i < response.size(); i++)
  {
    std::cout << "INFO browseBulkSync response " << i << ": address=" << response[i].address << " result=" << response[i].result.toString() << std::endl;
    printStringList(response[i].data);
  }
  return true;
}

//! Perform a synchronous bulk metadata request for multiple Data Layer nodes
//! @param[in] dataLayerClient  Connected Data Layer client instance
//! @result True on success, false on failure
static bool run_sync_metadata(comm::datalayer::IClient3 *dataLayerClient)
{
  comm::datalayer::VecBulkRequest request;
  comm::datalayer::VecBulkResponse response;

  request.push_back({"framework/metrics/system/cpu-utilisation-percent"});
  request.push_back({"framework/metrics/system/memavailable-mb"});

  auto result = dataLayerClient->metadataBulkSync(request, response);
  if (result != DL_OK)
  {
    std::cout << "INFO metadataBulkSync result: " << result.toString() << std::endl;
    return false;
  }
  for (size_t i = 0; i < response.size(); i++)
  {
    std::cout << "INFO metadataBulkSync response " << i << ": address=" << response[i].address << " result=" << response[i].result.toString() << std::endl;
    printMetadata(response[i].data);
  }
  return true;
}

//! Create a BulkCallback that stores the response and signals the given promise
//! @param[in] response  Reference to the response vector to be filled by the callback
//! @param[in] promise   Promise to signal when the callback is invoked
//! @result BulkCallback lambda
static comm::datalayer::BulkCallback callback(comm::datalayer::VecBulkResponse &response, std::promise<void> &promise)
{
  return [&](const comm::datalayer::VecBulkResponse &res)
  {
    response = res;
    promise.set_value();
  };
}

//! Perform an asynchronous bulk read of multiple Data Layer nodes
//! Blocks until the callback signals completion via a promise/future.
//! @param[in] dataLayerClient  Connected Data Layer client instance
//! @result True on success, false on failure
static bool run_async_read(comm::datalayer::IClient3 *dataLayerClient)
{
  comm::datalayer::VecBulkRequest request;
  comm::datalayer::VecBulkResponse response;

  std::promise<void> promise;
  auto future = promise.get_future();

  request.push_back({"framework/metrics/system/cpu-utilisation-percent"});
  request.push_back({"framework/metrics/system/memavailable-mb"});

  auto result = dataLayerClient->readBulkAsync(request, [&](const comm::datalayer::VecBulkResponse &res)
                                               {
    response = res;
    promise.set_value(); });

  // auto result = dataLayerClient->readBulkAsync(request, callback(response, promise));

  if (result != DL_OK)
  {
    std::cout << "INFO readBulkAsync result: " << result.toString() << std::endl;
    return false;
  }

  if (future.wait_for(std::chrono::seconds(5)) == std::future_status::timeout)
  {
    std::cout << "INFO readBulkAsync timeout: no response within 5 seconds" << std::endl;
    return false;
  }

  for (size_t i = 0; i < response.size(); i++)
  {
    std::cout << "INFO readBulkAsync response " << i << ": address=" << response[i].address << " result=" << response[i].result.toString() << " data: " << double(response[i].data) << std::endl;
  }
  return true;
}

//! Perform an asynchronous bulk browse of multiple Data Layer nodes
//! Blocks until the callback signals completion via a promise/future.
//! @param[in] dataLayerClient  Connected Data Layer client instance
//! @result True on success, false on failure
static bool run_async_browse(comm::datalayer::IClient3 *dataLayerClient)
{
  comm::datalayer::VecBulkRequest request;
  comm::datalayer::VecBulkResponse response;

  std::promise<void> promise;
  auto future = promise.get_future();

  request.push_back({"framework/metrics/system"});
  request.push_back({"framework/metrics/process"});

  auto result = dataLayerClient->browseBulkAsync(request, [&](const comm::datalayer::VecBulkResponse &res)
                                                 {
    response = res;
    promise.set_value(); });

  // auto result = dataLayerClient->browseBulkAsync(request, callback(response, promise));

  if (result != DL_OK)
  {
    std::cout << "INFO browseBulkAsync result: " << result.toString() << std::endl;
    return false;
  }

  if (future.wait_for(std::chrono::seconds(5)) == std::future_status::timeout)
  {
    std::cout << "INFO browseBulkAsync timeout: no response within 5 seconds" << std::endl;
    return false;
  }

  for (size_t i = 0; i < response.size(); i++)
  {
    std::cout << "INFO browseBulkAsync response " << i << ": address=" << response[i].address << " result=" << response[i].result.toString() << std::endl;
    printStringList(response[i].data);
  }
  return true;
}

//! Perform an asynchronous bulk metadata request for multiple Data Layer nodes
//! Blocks until the callback signals completion via a promise/future.
//! @param[in] dataLayerClient  Connected Data Layer client instance
//! @result True on success, false on failure
static bool run_async_metadata(comm::datalayer::IClient3 *dataLayerClient)
{
  comm::datalayer::VecBulkRequest request;
  comm::datalayer::VecBulkResponse response;

  std::promise<void> promise;
  auto future = promise.get_future();

  request.push_back({"framework/metrics/system/cpu-utilisation-percent"});
  request.push_back({"framework/metrics/system/memavailable-mb"});

  auto result = dataLayerClient->metadataBulkAsync(request, [&](const comm::datalayer::VecBulkResponse &res)
                                                   {
    response = res;
    promise.set_value(); });

  // auto result = dataLayerClient->metadataBulkAsync(request, callback(response, promise));

  if (result != DL_OK)
  {
    std::cout << "INFO metadataBulkAsync result: " << result.toString() << std::endl;
    return false;
  }

  if (future.wait_for(std::chrono::seconds(5)) == std::future_status::timeout)
  {
    std::cout << "INFO metadataBulkAsync timeout: no response within 5 seconds" << std::endl;
    return false;
  }

  for (size_t i = 0; i < response.size(); i++)
  {
    std::cout << "INFO metadataBulkAsync response " << i << ": address=" << response[i].address << " result=" << response[i].result.toString() << std::endl;
    printMetadata(response[i].data);
  }
  return true;
}

//! Run all asynchronous bulk operations: read, browse, and metadata
//! @param[in] dataLayerClient  Connected Data Layer client instance
static void run_async(comm::datalayer::IClient3 *dataLayerClient)
{
  if (!run_async_read(dataLayerClient)) {
    return;
  }

  if (!run_async_browse(dataLayerClient)) {
    return;
  }

  if (!run_async_metadata(dataLayerClient)) {
    return;
  }
}

//! Run all synchronous bulk operations: read, browse, and metadata
//! @param[in] dataLayerClient  Connected Data Layer client instance
static void run_sync(comm::datalayer::IClient3 *dataLayerClient)
{
  if (!run_sync_read(dataLayerClient)) {
    return;
  }

  if (!run_sync_browse(dataLayerClient)) {
    return;
  }

  if (!run_sync_metadata(dataLayerClient)) {
    return;
  }
}

//! Run all synchronous and asynchronous bulk operations
//! @param[in] dataLayerClient  Connected Data Layer client instance
static void run(comm::datalayer::IClient3 *dataLayerClient)
{
  run_sync(dataLayerClient);
  run_async(dataLayerClient);
}

// The enry point of the snap.
// Runs an endless loop until stopped or ctrlX Data Layer clientconnection breaks.
int main()
{

  std::cout << "INFO Starting ctrlX Data Layer system (without broker)" << std::endl;
  comm::datalayer::DatalayerSystem datalayerSystem;
  datalayerSystem.start(false);
  {
    // Creates a Datalayer Client instance and connects. Automatically reconnects if the connection is interrupted.
    auto connectionString = getConnectionString(); // default: ctrlX CORE or ctrlX COREvirtual with Network Adpater
    std::cout << "INFO Creating ctrlX Data Layer client connection to " << connectionString << " ..." << std::endl;
    auto dataLayerClient = std::unique_ptr<comm::datalayer::IClient3>(datalayerSystem.factory()->createClient3(connectionString));

    if (dataLayerClient->isConnected() == false)
    {
      auto connectionString2 = getConnectionString("10.0.2.2", "boschrexroth", "boschrexroth", 8443); // ctrlX COREvirtual with Port Forwarding
      std::cout << "WARNING Cannot connect to " << connectionString << " trying " << connectionString2 << std::endl;

      dataLayerClient.reset(datalayerSystem.factory()->createClient3(connectionString2));
    }

    int counter = 1;
    while (dataLayerClient->isConnected())
    {
      std::cout << "Loop #" << counter++ << std::endl;

      run(dataLayerClient.get());

      std::cout << "INFO Sleeping..." << std::endl;
      sleep(2);
    }

    std::cout << "ERROR ctrlX Data Layer connection is broken" << std::endl;
  }
  datalayerSystem.stop();

  return 1; // We exit because an error happend
}
