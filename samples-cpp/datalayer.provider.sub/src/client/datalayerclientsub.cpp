/*
* SPDX-FileCopyrightText: Bosch Rexroth AG
*
* SPDX-License-Identifier: MIT
*/

#include <functional>
#include <thread>
#include <limits>

#include <comm/datalayer/sub_properties_generated.h>

#include "datalayerclientsub.h"

// Constructor: initializes member variables; the client instance is created later in connect()
DataLayerClientSub::DataLayerClientSub(comm::datalayer::DatalayerSystem &system)
  : m_datalayerSystem(system)
  , m_datalayerClient(nullptr)
{}

// Destructor: calls disconnect() to release the client instance safely
DataLayerClientSub::~DataLayerClientSub()
{
  disconnect();
}

// Creates a new IClient3 instance for the given connection string and verifies the connection
comm::datalayer::DlResult DataLayerClientSub::connect(const std::string& connection)
{
  std::cout << "INFO" << __func__ << connection << std::endl;
  m_datalayerClient.reset(m_datalayerSystem.factory()->createClient3(connection));
  if (m_datalayerClient == nullptr)
  {
    std::cout << "ERROR: Could not create datalayer client instance" << std::endl;
    return DL_CREATION_FAILED;
  }

  return isConnected();
}

// Returns DL_OK when the client object exists and reports an active connection
comm::datalayer::DlResult DataLayerClientSub::isConnected()
{
  if (m_datalayerClient == nullptr || m_datalayerClient->isConnected() == false)
  {
    return DL_CLIENT_NOT_CONNECTED;
  }

  return DL_OK;
}

// Creates a subscription with a 250 ms sampling interval and 60 s keep-alive timeout.
// The subscription ID groups multiple node addresses under a single callback.
comm::datalayer::DlResult DataLayerClientSub::createSubscriptionSync(const std::string& id)
{
  flatbuffers::FlatBufferBuilder builder;
  // Define the sampling rule: data is collected every 250,000 µs (250 ms)
  auto dataChange = comm::datalayer::CreateSampling(builder, 250000);
  auto rule = comm::datalayer::CreateProperty(builder, comm::datalayer::Properties::Properties_Sampling, dataChange.Union());
  auto rules = builder.CreateVector(&rule, 1);
  auto idTmp = builder.CreateString(id);
  // Build the subscription properties: id, keep-alive 60 s, queue size 250, sampling rule
  builder.Finish(comm::datalayer::CreateSubscriptionProperties(builder, idTmp, 60000, 250, rules));
  comm::datalayer::Variant subscriptionProperties;
  subscriptionProperties.shareFlatbuffers(builder);

  auto result = m_datalayerClient->createSubscriptionSync(subscriptionProperties, publishCallback());
  if (STATUS_FAILED(result))
  {
    std::cout << "createSubscriptionSync failed with " << std::string(result.toString()) << std::endl;
    return result;
  }

  return result;
}

// This method returns a comm::datalayer::PublishCallback function as lambda expression.
// See:
// https://en.cppreference.com/w/cpp/language/lambda
// https://de.cppreference.com/w/cpp/language/lambda
comm::datalayer::PublishCallback DataLayerClientSub::publishCallback()
{
  // Lambda is called by the Data Layer runtime whenever a subscribed node value changes
  return [&](comm::datalayer::DlResult result, const std::vector<comm::datalayer::NotifyItem>& items)
  {
    std::cout << "--------------- Data has been changed! Result: " << result.toString() << std::endl;

    // Check if the Data Layer access was successful
    if (result != DL_OK)
    {
      std::cout << "DL Error" << std::endl;
      return;
    }

    // Guard against an empty items vector (should not happen under normal conditions)
    if (items.empty())
    {
      std::cout << "No items" << std::endl;
      return;
    }

    // Iterate over all changed items and print their type, size, and value
    for (int n = 0; n < items.size(); n++)
    {
      std::cout << "Item #" << n << std::endl;
      std::cout << "  type:" << items[n].data.typeAsString() << std::endl;
      std::cout << "  size:" << items[n].data.getSize() << std::endl;

      // Try to print the value as FLOAT64; fall back to STRING; skip unknown types
      if (comm::datalayer::STATUS_SUCCEEDED(items[n].data.checkConvert(comm::datalayer::VariantType::FLOAT64)))
      {
        std::cout << "  value: " << static_cast<double>(items[n].data) << std::endl;
      }
      else if (comm::datalayer::STATUS_SUCCEEDED(items[n].data.checkConvert(comm::datalayer::VariantType::STRING)))
      {
        std::cout << "  value: " << static_cast<const char*>(items[n].data) << std::endl;
      }
      else
      {
        std::cout << "  can't print value with this data type" << std::endl;
        continue;
      }

      // Decode the notify info FlatBuffer to obtain address, timestamp, and event type
      // (NotifyInfo schema is defined in notify_info.fbs)
      auto result = (items[n].info.verifyFlatbuffers(comm::datalayer::VerifyNotifyInfoBuffer)); // VerifyNotifyInfoBuffer is generated out of notify_info.fbs file
      if (STATUS_FAILED(result))
      {
        std::cout << "Invalid Flatbuffer: Not a NotifyInfoBuffer " << std::string(result.toString()) << std::endl;
        continue;
      }

      auto info = comm::datalayer::GetNotifyInfo(items[n].info.getData()); // GetNotifyData is generated out of .fbs file
      std::cout << "  address: " << info->node()->c_str() << std::endl;
      std::cout << "  timestamp: " << info->timestamp() << std::endl;
      std::cout << "  notifyType: " << comm::datalayer::EnumNamesNotifyType()[info->notifyType()] << std::endl;
    }
  };
}

// Adds the given address to the subscription identified by id.
// Returns DL_OK on success or the error code returned by the Data Layer.
comm::datalayer::DlResult DataLayerClientSub::subscribeSync(const std::string& id, const std::string& address)
{
  auto result = m_datalayerClient->subscribeSync(id, address);
  if (STATUS_FAILED(result))
  {
    std::cout << "subscribeSync of " << address << " with " << id << " failed with: " << std::string(result.toString()) << std::endl;
    return result;
  }

  std::cout << "subscribeSync of " << address << " with " << id << " was successful " << std::string(result.toString()) << std::endl;
  return result;
}

// Removes the subscription identified by id and stops all associated change notifications.
comm::datalayer::DlResult DataLayerClientSub::unsubscribeSync(const std::string& id)
{
  auto result = m_datalayerClient->unsubscribeSync(id);
  if (STATUS_FAILED(result))
  {
    std::cout << "unsubscribeSync from " << id << " failed with: " << std::string(result.toString()) << std::endl;
    return result;
  }
  std::cout << "unsubscribeSync from " << id << " was successful: " << std::string(result.toString()) << std::endl;
  return DL_OK;
}

// Releases the client shared_ptr, which triggers the IClient3 destructor and closes the connection
void DataLayerClientSub::disconnect()
{
  m_datalayerClient = nullptr;
}
