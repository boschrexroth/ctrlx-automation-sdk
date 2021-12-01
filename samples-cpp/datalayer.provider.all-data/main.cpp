/**
 * MIT License
 * 
 * Copyright (c) 2020-2021 Bosch Rexroth AG
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

#include "providerNodeAllData.h"

static std::string dl_addr_base = "sdk-cpp-alldata/"; // snap/snapcraft.yaml name:

static std::string getConnection()
{
  if (is_snap())
  {
    return DL_IPC_AUTO; // ipc://datalayer/backend
  }

  // 10.0.2.2 - build environment is VM and target is ctrlX virtual with port forwarding
  return DL_TCP + std::string("boschrexroth:boschrexroth@10.0.2.2:") + std::to_string(DL_TCP_BACKEND_FIRST_PORT);
}

// Add some signal Handling so we are able to abort the program with sending sigint
bool endProcess = false;
static void sigHandler(int sig, siginfo_t *siginfo, void *context)
{
  endProcess = true;
}

int main(int ac, char *av[])
{
  #ifdef MY_DEBUG
  std::cout << "Raising SIGSTOP" << std::endl;
  //  raise(SIGSTOP);
  std::cout << "... Continue..." << std::endl;
#endif

  // Prepare signal structure to interrupt the endless loop with ctrl + c
  struct sigaction act;
  memset(&act, '\0', sizeof(act));
  act.sa_sigaction = &sigHandler;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGINT, &act, NULL);

  std::cout << "INFO Creating Data Layer System" << std::endl;
  comm::datalayer::DatalayerSystem datalayerSystem;
  std::cout << "INFO Starting Data Layer System" << std::endl;
  datalayerSystem.start(false);

  // Creates a provider instance at Data Layer backend to provide data to Data Layer clients
  std::string providerConnection = getConnection();
  std::cout << "INFO Creating Data Layer Provider, using connection " + providerConnection << std::endl;
  auto *dlProvider = datalayerSystem.factory()->createProvider(providerConnection);

  std::cout << "INFO Starting Data Layer Provider" << std::endl;
  auto result = dlProvider->start();

  if (STATUS_FAILED(result))
  {
    std::cout << "FATAL Starting Data Layer Provider failed with " << result.toString() << std::endl;
    return -1;
  }
  std::cout << "INFO Data Layer Provider started" << std::endl;

  std::cout << "INFO Pause 1s while Data Layer Provider connection will be established" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Pause until dlProvider->isConnected() is called

  std::cout << "INFO Creating Data Layer branch " << dl_addr_base << std::endl;
  auto providerNodeStatic = new ProviderNodeAllData(dlProvider, dl_addr_base, false);
  providerNodeStatic->RegisterNodes();

  auto providerNodeDynamic = new ProviderNodeAllData(dlProvider, dl_addr_base, true);
  providerNodeDynamic->RegisterNodes();

  std::cout << "INFO Running endless loop - end with Ctrl+C" << std::endl;
  bool providerConnectionError = false;
  while (endProcess == false)
  {
    if (dlProvider->isConnected() == false)
    {
      if (providerConnectionError == false)
      {
        providerConnectionError = true; // Logg only once
        std::cout << "ERROR Data Layer Provider connection " + providerConnection + " is interrupted" << std::endl;
      }
    }
    else
    {
      if (providerConnectionError)
      {
        providerConnectionError = false;
        std::cout << "INFO Data Layer Provider connection " + providerConnection + " is reconnected" << std::endl;
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  delete providerNodeStatic;
  providerNodeStatic = NULL;

  delete providerNodeDynamic;
  providerNodeDynamic = NULL;

  dlProvider->stop();
  delete dlProvider;
  dlProvider = NULL;

  datalayerSystem.stop(false);

  return 0;
}