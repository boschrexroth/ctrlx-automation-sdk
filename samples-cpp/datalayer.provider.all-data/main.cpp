/**
 * MIT License
 *
 * Copyright (c) 2020-2022 Bosch Rexroth AG
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

#include <signal.h>
#include <thread>
#include <string>

#include "ctrlx_datalayer_helper.h"
#include "providerNodeAllData.h"

static std::string dl_addr_base = "sdk-cpp-alldata/"; // snap/snapcraft.yaml name:

// Add some signal Handling so we are able to abort the program with sending sigint
bool endProcess = false;
static void sigHandler(int sig, siginfo_t* siginfo, void* context)
{
  endProcess = true;
}

static void shutdownMessage()
{
  std::cout << "ERROR Connection to ctrlX CORE failed." << std::endl;

  std::cout << "Check physical network connection!" << std::endl;
  std::cout << "Check connection parameters:" << std::endl;
  std::cout << "ctrlX CORE virtual via port forwarding: IP=10.0.2.2, is port 2070 forwarded?" << std::endl;
  std::cout << "ctrlX CORE virtual with network adapter or real: Is IP address correct?" << std::endl;
  std::cout << "Is ctrlX CORE in Operation mode? " << std::endl;
  std::cout << std::endl;

  std::cout << "Shutting down!" << std::endl;
  if (getenv("SNAP") != nullptr)
  {
    std::cout << " Restarting automatically..." << std::endl;
  }
}

int main()
{

#ifdef MY_DEBUG
  std::cout << "Raising SIGSTOP" << std::endl;
  //raise(SIGSTOP);
  std::cout << "... Continue..." << std::endl;
#endif

  // Prepare signal structure to interrupt the endless loop with ctrl + c
  struct sigaction act;
  memset(&act, '\0', sizeof(act));
  act.sa_sigaction = &sigHandler;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGINT, &act, NULL);

  comm::datalayer::DatalayerSystem datalayerSystem;
  datalayerSystem.start(false);

  // ip="192.168.1.1" or any other IP address if a ctrlX CORE or ctrlX CORE virtual
  // ip="10.0.2.2"    ctrlX CORE virtual with port forwarding
  auto* provider = getProvider(datalayerSystem);
  if (provider == nullptr)
  {
    shutdownMessage();
    datalayerSystem.stop(false);
    return 1;
  }

  std::cout << "INFO Creating Data Layer branch " << dl_addr_base << std::endl;
  auto providerNodeStatic = new ProviderNodeAllData(provider, dl_addr_base, false);
  providerNodeStatic->RegisterNodes();

  auto providerNodeDynamic = new ProviderNodeAllData(provider, dl_addr_base, true);
  providerNodeDynamic->RegisterNodes();

  std::cout << "INFO Running endless loop - end with Ctrl+C" << std::endl;
  while (endProcess == false)
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

  return endProcess ? 0 : 1;
}
