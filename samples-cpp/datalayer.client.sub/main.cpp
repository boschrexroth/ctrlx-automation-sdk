/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <csignal>
#include <thread>

#include "ctrlx_datalayer_helper.h"

#include "datalayerclientsub.h"

static bool g_errorReported = false;

// Add signal handling so we are able to abort the program by sending SIGINT
static bool g_endProcess = false;
static void signalHandler(int signal)
{
  std::cout << "signal: " << signal << std::endl;
  g_endProcess = true;
}


int main()
{
  // Prepare signal structure to interrupt the endless loop with Ctrl+C (SIGINT)
  std::signal(SIGINT, signalHandler);

  comm::datalayer::DatalayerSystem datalayerSystem;
  datalayerSystem.start(false);

  for (;;)
  {
    auto dataLayerClientSub = new DataLayerClientSub(datalayerSystem);

    //auto clientConnectionString = getConnectionString(); // default: ctrlX CORE or ctrlX COREvirtual with Network Adpater
    auto clientConnectionString = getConnectionString("10.0.2.2","boschrexroth","boschrexroth", 8443);  // ctrlX COREvirtual with Port Forwarding
    dataLayerClientSub->connect(clientConnectionString);

    while (dataLayerClientSub->isConnected() == DL_OK && g_endProcess == false)
    {
      g_errorReported = false;

      std::cout << "INFO Creating subscription" << std::endl;

      dataLayerClientSub->createSubscriptionSync("sdk-cpp-sub-1");
      dataLayerClientSub->subscribeSync("sdk-cpp-sub-1", "framework/metrics/system/cpu-utilisation-percent");

      dataLayerClientSub->createSubscriptionSync("sdk-cpp-sub-2");
      std::set<std::string> addresses;
      addresses.insert("framework/metrics/system/memused-percent");
      addresses.insert("framework/metrics/system/memused-mb");
      dataLayerClientSub->subscribeSyncMulti("sdk-cpp-sub-2", addresses);

      std::cout << "INFO Sleeping ..." << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(10));

      std::cout << "INFO Unsubscribing..." << std::endl;
      dataLayerClientSub->unsubscribeSync("sdk-cpp-sub-1");
      dataLayerClientSub->unsubscribeSync("sdk-cpp-sub-2");
    }

    dataLayerClientSub->disconnect();
    delete dataLayerClientSub;

    if (g_endProcess)
    {
      std::cout << "INFO Terminating process due to user input of Ctrl+C (signal SIGINT)" << std::endl;
      break;
    }

    // ctrlX Data Layer is disconnected
    if (!g_errorReported)
    {
      g_errorReported = true;
      std::cout << "ERROR Connection to ctrlX Data Layer failed - retry every 10 seconds" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));

  }

  datalayerSystem.stop();
  return 0;
}
