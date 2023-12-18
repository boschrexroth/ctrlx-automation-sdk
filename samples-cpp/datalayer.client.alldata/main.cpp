/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include "datalayerclient.h"

int main(void)
{
  int counter = 1;
  for (;;)
  {
    std::cout << "Loop #" << counter++ << std::endl;

    // ctrlX CORE or ctrlX COREvirtual with network adapter:
    auto dataLayerClient = new DataLayerClient("192.168.1.1", "boschrexroth", "boschrexroth", 443);
    // ctrlX COREvirtual with port forwarding:
    // auto dataLayerClient = new DataLayerClient("10.0.2.2", "boschrexroth", "boschrexroth", 8443);

    dataLayerClient->run();
    delete dataLayerClient; // Callback are no more called

    std::cout << "Sleeping..." << std::endl;
    sleep(10);
  }
  return 0;
}
