/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <memory>
#include "datalayerclient.h"

int main(void)
{
  int counter = 1;
  for (;;)
  {
    std::cout << "Loop #" << counter++ << std::endl;

    // ctrlX CORE or ctrlX COREvirtual with network adapter:
    //auto dataLayerClient = std::make_unique<DataLayerClient>("192.168.1.1", "boschrexroth", "boschrexroth", 443);
    // ctrlX COREvirtual with port forwarding:
    auto dataLayerClient = std::make_unique<DataLayerClient>("10.0.2.2", "boschrexroth", "boschrexroth", 8443);

    dataLayerClient->run();
    // Callback are no more called - destroyed automatically at end of scope

    std::cout << "Sleeping..." << std::endl;
    sleep(10);
  }
  return 0;
}
