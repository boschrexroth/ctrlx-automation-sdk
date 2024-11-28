/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <csignal>
#include <thread>
#include <string>

#include "ctrlx_datalayer_helper.h"
#include "providerNodeAllData.h"

static std::string g_dataLayerAddrBase = "sdk/cpp/datalayer/provider/alldata/";

static bool g_errorReported = false;

// Add signal handling so we are able to abort the program by sending SIGINT
static bool g_endProcess = false;
static void signalHandler(int signal)
{
  std::cout << "signal: " << signal << std::endl;
  g_endProcess = true;
}

int main(void)
{

  // Prepare signal structure to interrupt the endless loop with Ctrl+C (SIGINT)
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);
  std::signal(SIGABRT, signalHandler);

  comm::datalayer::DatalayerSystem datalayerSystem;
  datalayerSystem.start(false);

  // Outer loop
  for (;;)
  {

    // ip="192.168.1.1" or any other IP address if a ctrlX CORE or ctrlX COREvirtual
    // ip="10.0.2.2"    ctrlX COREvirtual with port forwarding
    auto* provider = getProvider(datalayerSystem);

    if (provider == nullptr) {
      // Wait before (re)connecting to the ctrlX Data Layer.
      if (g_endProcess)
      {
        std::cout << "INFO Terminating process due to user input of Ctrl+C (signal SIGINT)" << std::endl;
        datalayerSystem.stop();
        return 0;
      }

      std::cout << "INFO Wait before (re)connecting to the ctrlX Data Layer " << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(10));
      continue;
    }

    g_errorReported = false;

    std::cout << "INFO Creating ctrlX Data Layer node branch " << g_dataLayerAddrBase << std::endl;

    auto providerNodeStatic = new ProviderNodeAllData(provider, g_dataLayerAddrBase, false);
    providerNodeStatic->registerNodes();

    auto providerNodeDynamic = new ProviderNodeAllData(provider, g_dataLayerAddrBase, true);
    providerNodeDynamic->registerNodes();

    // Inner loop
    std::cout << "INFO Running endless loop - will be ended on connection error or user input of Ctrl+C" << std::endl;
    while (g_endProcess == false && provider->isConnected())
    {
      std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    delete providerNodeStatic;
    delete providerNodeDynamic;

    provider->stop();
    delete provider;

    if (g_endProcess)
    {
      std::cout << "INFO Terminating process due to user input of Ctrl+C (signal SIGINT)" << std::endl;
      datalayerSystem.stop();
      return 0;
    }

    // ctrlX Data Layer is disconnected
    if (!g_errorReported)
    {
      g_errorReported = true;
      std::cout << "ERROR Connection to ctrlX Data Layer failed - retry every 10 seconds" << std::endl;
    }
  }
}
