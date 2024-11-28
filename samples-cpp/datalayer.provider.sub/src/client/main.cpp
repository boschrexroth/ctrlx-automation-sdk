/*
* SPDX-FileCopyrightText: Bosch Rexroth AG
*
* SPDX-License-Identifier: MIT
*/

#include <csignal>
#include <thread>

#include <ctrlx_datalayer_helper.h>

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
  std::signal(SIGTERM, signalHandler);
  std::signal(SIGABRT, signalHandler);

  {
    comm::datalayer::DatalayerSystem datalayerSystem;
    datalayerSystem.start(false);

    for (;;)
    {
      auto dataLayerClientSub = DataLayerClientSub(datalayerSystem);

      //auto clientConnectionString = getConnectionString(); // default: ctrlX CORE or ctrlX COREvirtual with Network Adpater
      auto clientConnectionString = getConnectionString("10.0.2.2","boschrexroth","boschrexroth", 8443);  // ctrlX COREvirtual with Port Forwarding
      dataLayerClientSub.connect(clientConnectionString);

      std::cout << "INFO Creating subscription" << std::endl;
      dataLayerClientSub.createSubscriptionSync("sdk-cpp-sub-1");
      comm::datalayer::DlResult result = dataLayerClientSub.subscribeSync("sdk-cpp-sub-1", "sdk/cpp/datalayer/provider/sub/myInt64");

      if ( STATUS_FAILED(result) ) {
        std::cout << "ERROR subscribe: " << result.toString() << " : item: 'myInt64'" << std::endl;
        return 1;
      }
      result = dataLayerClientSub.subscribeSync("sdk-cpp-sub-1", "sdk/cpp/datalayer/provider/sub/myInt64_2");
      if ( STATUS_FAILED(result) ) {
        std::cout << "ERROR subscribe: " << result.toString() << " : item: 'myInt64_2'" << std::endl;
        return 1;
      }

      while (dataLayerClientSub.isConnected() == DL_OK && g_endProcess == false)
      {
        g_errorReported = false;

        //std::cout << "INFO Sleeping ..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

      }

      std::cout << "INFO Unsubscribing..." << std::endl;
      dataLayerClientSub.unsubscribeSync("sdk-cpp-sub-1");

      dataLayerClientSub.disconnect();

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
  }
  return 0;
}
