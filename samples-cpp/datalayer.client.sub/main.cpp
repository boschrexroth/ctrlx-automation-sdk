/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include "ctrlx_datalayer_helper.h"

#include "datalayerclientsub.h"

int main()
{

  comm::datalayer::DatalayerSystem datalayerSystem;
  datalayerSystem.start(false);

  auto dataLayerClientSub = new DataLayerClientSub(datalayerSystem);

  auto clientConnectionString = getConnectionString(); // default: ctrlX CORE or ctrlX COREvirtual with Network Adpater
  //auto clientConnectionString = getConnectionString("10.0.2.2","boschrexroth","boschrexroth", 8443);  // ctrlX COREvirtual with Port Forwarding
  auto result = dataLayerClientSub->connect(clientConnectionString);
  if (STATUS_FAILED(result))
  {
    std::cout << "ERROR Connection failed: " << clientConnectionString << std::endl;
    dataLayerClientSub->disconnect();
    datalayerSystem.stop(false);
    return 1;
  }

  int counter = 1;
  unsigned int sleepTime = 10;
  for (;;)
  {
    result = dataLayerClientSub->isConnected();
    if (STATUS_FAILED(result))
    {
      break;
    }

    std::cout << "Loop #" << counter++ << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    dataLayerClientSub->createSubscriptionSync("Sub1");
    dataLayerClientSub->subscribeSync("Sub1", "framework/metrics/system/cpu-utilisation-percent");
    dataLayerClientSub->subscribeSync("Sub1", "framework/metrics/system/memavailable-mb");
    std::cout << "Sleeping ..." << std::endl;
    sleep(sleepTime);
    dataLayerClientSub->unsubscribeSync("Sub1");

    dataLayerClientSub->createSubscriptionSync("Sub2");
    std::set<std::string> addresses;
    addresses.insert("framework/metrics/system/memused-percent");
    addresses.insert("framework/metrics/system/memused-mb");
    dataLayerClientSub->subscribeSyncMulti("Sub2", addresses);
    std::cout << "Sleeping ..." << std::endl;
    sleep(sleepTime);
    dataLayerClientSub->unsubscribeSync("Sub2");
  }

  std::cout << "ERROR ctrlX Data Layer is disconnected" << std::endl;
  dataLayerClientSub->disconnect();
  datalayerSystem.stop(false); // Attention: Doesn't return if any provider or client instance is still runnning
  return 0;
}
