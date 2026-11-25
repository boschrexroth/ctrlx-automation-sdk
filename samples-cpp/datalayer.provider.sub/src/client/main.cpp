/*
* SPDX-FileCopyrightText: Bosch Rexroth AG
*
* SPDX-License-Identifier: MIT
*/

#include <csignal>
#include <thread>

#include <ctrlx_datalayer_helper.h>

#include "datalayerclientsub.h"

// Flag to suppress repeated error messages when the connection is lost
static bool g_errorReported = false;

// Add signal handling so we are able to abort the program by sending SIGINT
static bool g_endProcess = false;

// Signal handler: sets the termination flag when an OS signal is received
static void signalHandler(int signal)
{
  std::cout << "signal: " << signal << std::endl;
  g_endProcess = true;
}


int main()
{
  // Register signal handlers to allow graceful shutdown via Ctrl+C, SIGTERM, or SIGABRT
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);
  std::signal(SIGABRT, signalHandler);

  {
    // Initialize and start the ctrlX Data Layer system (false = no verbose logging)
    comm::datalayer::DatalayerSystem datalayerSystem;
    datalayerSystem.start(false);

    // Outer loop: reconnects automatically if the Data Layer connection is lost
    for (;;)
    {
      auto dataLayerClientSub = DataLayerClientSub(datalayerSystem);

      // Build the connection string for the target device.
      //auto clientConnectionString = getConnectionString(); // default: ctrlX CORE or ctrlX COREvirtual with Network Adapter
      auto clientConnectionString = getConnectionString("10.0.2.2","boschrexroth","boschrexroth", 8443);  // ctrlX COREvirtual with Port Forwarding
      dataLayerClientSub.connect(clientConnectionString);

      // Create a named subscription that groups multiple node addresses together
      std::cout << "INFO Creating subscription" << std::endl;
      dataLayerClientSub.createSubscriptionSync("sdk-cpp-sub-1");

      // Subscribe to the first Data Layer node and check for errors
      comm::datalayer::DlResult result = dataLayerClientSub.subscribeSync("sdk-cpp-sub-1", "sdk/cpp/datalayer/provider/sub/myInt64");
      if ( STATUS_FAILED(result) ) {
        std::cout << "ERROR subscribe: " << result.toString() << " : item: 'myInt64'" << std::endl;
        return 1;
      }

      // Subscribe to the second Data Layer node and check for errors
      result = dataLayerClientSub.subscribeSync("sdk-cpp-sub-1", "sdk/cpp/datalayer/provider/sub/myInt64_2");
      if ( STATUS_FAILED(result) ) {
        std::cout << "ERROR subscribe: " << result.toString() << " : item: 'myInt64_2'" << std::endl;
        return 1;
      }

      // Keep running while the connection is alive and no termination signal was received.
      // Incoming data change notifications are delivered asynchronously via the publish callback.
      while (dataLayerClientSub.isConnected() == DL_OK && g_endProcess == false)
      {
        g_errorReported = false;

        //std::cout << "INFO Sleeping ..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }

      // Remove the subscription before disconnecting to release server-side resources
      std::cout << "INFO Unsubscribing..." << std::endl;
      dataLayerClientSub.unsubscribeSync("sdk-cpp-sub-1");

      dataLayerClientSub.disconnect();

      // Exit the loop cleanly when the user requests termination
      if (g_endProcess)
      {
        std::cout << "INFO Terminating process due to user input of Ctrl+C (signal SIGINT)" << std::endl;
        break;
      }

      // Report the connection loss once and wait before attempting to reconnect
      if (!g_errorReported)
      {
        g_errorReported = true;
        std::cout << "ERROR Connection to ctrlX Data Layer failed - retry every 10 seconds" << std::endl;
      }

      std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    // Stop the Data Layer system and release all resources
    datalayerSystem.stop();
  }
  return 0;
}
