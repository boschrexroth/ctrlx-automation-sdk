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

static std::string g_dataLayerAddrBase = "sdk-cpp-alldata/"; // snap/snapcraft.yaml name:

// Add some signal Handling so we are able to abort the program with sending sigint
static bool g_endProcess = false;
static void signalHandler(int signal)
{
  std::cout << "signal: " << signal << std::endl;
  g_endProcess = true;
}

static void shutdownMessage()
{
  std::cout << "ERROR Connection to ctrlX CORE failed." << std::endl;

  std::cout << "Check physical network connection!" << std::endl;
  std::cout << "Check connection parameters:" << std::endl;
  std::cout << "ctrlX COREvirtual via port forwarding: IP=10.0.2.2, is port 2070 forwarded?" << std::endl;
  std::cout << "ctrlX COREvirtual with network adapter or real: Is IP address correct?" << std::endl;
  std::cout << "Is ctrlX CORE in Operation mode? " << std::endl;
  std::cout << std::endl;

  std::cout << "Shutting down!" << std::endl;
  if (std::getenv("SNAP") != nullptr)
  {
    std::cout << " Restarting automatically..." << std::endl;
  }
}

int main(void)
{

  // Prepare signal structure to interrupt the endless loop with ctrl + c
  std::signal(SIGINT, signalHandler);

  comm::datalayer::DatalayerSystem datalayerSystem;
  datalayerSystem.start(false);

  // ip="192.168.1.1" or any other IP address if a ctrlX CORE or ctrlX COREvirtual
  // ip="10.0.2.2"    ctrlX COREvirtual with port forwarding
  auto* provider = getProvider(datalayerSystem);
  if (provider == nullptr)
  {
    shutdownMessage();
    datalayerSystem.stop(false);
    return 1;
  }

  std::cout << "INFO Creating ctrlX Data Layer branch " << g_dataLayerAddrBase << std::endl;
  auto providerNodeStatic = new ProviderNodeAllData(provider, g_dataLayerAddrBase, false);
  providerNodeStatic->registerNodes();

  auto providerNodeDynamic = new ProviderNodeAllData(provider, g_dataLayerAddrBase, true);
  providerNodeDynamic->registerNodes();

  std::cout << "INFO Running endless loop - end with Ctrl+C" << std::endl;
  while (g_endProcess == false)
  {
    if (provider->isConnected() == false)
    {
      shutdownMessage();
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  delete providerNodeStatic;

  delete providerNodeDynamic;

  provider->stop();
  delete provider;

  datalayerSystem.stop(false); // Attention: Doesn't return if any provider or client instance is still runnning

  return g_endProcess ? 0 : 1;
}
