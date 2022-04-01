/**
 * MIT License
 *
 * Copyright (c) 2021 Bosch Rexroth AG
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <functional>
#include <thread>
#include <limits>

#include "comm/datalayer/sub_properties_generated.h"

#include "datalayerclientsub.h"
DataLayerClientSub::DataLayerClientSub(const comm::datalayer::DatalayerSystem &datalayerSystem)
{
  _datalayerSystem = datalayerSystem;
}

comm::datalayer::DlResult DataLayerClientSub::connect(const std::string &clientConnection)
{
  std::cout << "_datalayerSystem.factory()->createClient2: " << clientConnection << std::endl;
  _datalayerClient = _datalayerSystem.factory()->createClient2(clientConnection);
  if (_datalayerClient == nullptr)
  {
    std::cout << "ERROR: Could not create datalayer client instance" << std::endl;
    return DL_CREATION_FAILED;
  }

  return isConnected();
}

comm::datalayer::DlResult DataLayerClientSub::isConnected()
{
  if (_datalayerClient == nullptr || _datalayerClient->isConnected() == false)
  {
    return DL_CLIENT_NOT_CONNECTED;
  }

  return DL_OK;
}

comm::datalayer::DlResult DataLayerClientSub ::createSubscriptionSync(const std::string &id)
{
  flatbuffers::FlatBufferBuilder builder;
  auto dataChange = comm::datalayer::CreateSampling(builder, 250000);
  auto rule = comm::datalayer::CreateProperty(builder, comm::datalayer::Properties::Properties_Sampling, dataChange.Union());
  auto rules = builder.CreateVector(&rule, 1);
  auto idTmp = builder.CreateString(id);
  builder.Finish(comm::datalayer::CreateSubscriptionProperties(builder, idTmp, 60000, 250, rules));
  comm::datalayer::Variant subscriptionProperties;
  subscriptionProperties.shareFlatbuffers(builder);

  auto result = _datalayerClient->createSubscriptionSync(subscriptionProperties, publishCallback());
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
  return [&](comm::datalayer::DlResult result, const std::vector<comm::datalayer::NotifyItem> &items)
  {
    std::cout << "--------------- Data has been changed! Result: " << result.toString() << std::endl;

    // check if datalayer-exess was ok
    if (result != DL_OK)
    {
      std::cout << "DL Error" << std::endl;
      return;
    }

    // check if items is not a nullpointer
    if (items.empty())
    {
      std::cout << "No items" << std::endl;
      return;
    }

    for (int n = 0; n < items.size(); n++)
    {
      std::cout << "Item #" << n << std::endl;
      std::cout << "  type:" << items[n].data.typeAsString() << std::endl;
      std::cout << "  size:" << items[n].data.getSize() << std::endl;

      if (comm::datalayer::STATUS_SUCCEEDED(items[n].data.checkConvert(comm::datalayer::VariantType::FLOAT64)))
      {
        std::cout << "  value: " << static_cast<double>(items[n].data) << std::endl;
      }
      else if (comm::datalayer::STATUS_SUCCEEDED(items[n].data.checkConvert(comm::datalayer::VariantType::STRING)))
      {
        std::cout << "  value: " << static_cast<const char *>(items[n].data) << std::endl;
      }
      else
      {
        std::cout << "  can't print value with this data type" << std::endl;
        continue;
      }

      // get the info of the notify_item (defined in notify_info.fbs)

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

comm::datalayer::DlResult DataLayerClientSub::subscribeSync(const std::string &id, const std::string &address)
{
  auto result = _datalayerClient->subscribeSync(id, address);
  if (STATUS_FAILED(result))
  {
    std::cout << "subscribeSync of " << address << " with " << id << " failed with: " << std::string(result.toString()) << std::endl;
    return result;
  }

  std::cout << "subscribeSync of " << address << " with " << id << " was successful " << std::string(result.toString()) << std::endl;
  return result;
}

comm::datalayer::DlResult DataLayerClientSub::subscribeSync_Multi(const std::string &id, const std::set<std::string> &addresses)
{
  auto result = _datalayerClient->subscribeSync(id, addresses);
  if (STATUS_FAILED(result))
  {
    std::cout << "subscribeSync_Multi "
              << " with " << id << " failed with: " << std::string(result.toString()) << std::endl;
    return result;
  }
  std::cout << "subscribeSync_Multi "
            << " with " << id << " was successful " << std::string(result.toString()) << std::endl;
  return result;
}

comm::datalayer::DlResult DataLayerClientSub::unsubscribeSync(const std::string &id)
{
  auto result = _datalayerClient->unsubscribeSync(id);
  if (STATUS_FAILED(result))
  {
    std::cout << "unsubscribeSync from " << id << " failed with: " << std::string(result.toString()) << std::endl;
    return result;
  }
  std::cout << "unsubscribeSync from " << id << " was successful: " << std::string(result.toString()) << std::endl;
  return DL_OK;
}

void DataLayerClientSub::disconnect()
{
  delete _datalayerClient;
}
