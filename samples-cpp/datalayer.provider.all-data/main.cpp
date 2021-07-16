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

// This app needs access rights to the Data Layer of your ctrlX.
// TODO These user and password settings are default values.
// TODO Add a new user for this app in the ctrlX und change the settings here.
static const std::string tcpUser = "boschrexroth";
static const std::string tcpPwd = "boschrexroth";

// Check/Change this IP address
// take 127.0.0.1 when you have a ctrlX virtual with PortForwarding
static std::string tcpIpAddress = "192.168.1.1";

static std::string getConnection(const std::string &tcpConnectionArg, int port)
{
  if (is_snap())
  {
    return DL_IPC_AUTO; // ipc://datalayer/backend
  }

  if (tcpConnectionArg.empty())
  {
    return DL_TCP + tcpUser + ":" + tcpPwd + "@" + tcpIpAddress + ":" + std::to_string(port);
  }

  return DL_TCP + tcpConnectionArg + ":" + std::to_string(port);
}

// Add some signal Handling so we are able to abort the program with sending sigint
bool endProcess = false;
static void sigHandler(int sig, siginfo_t *siginfo, void *context)
{
  endProcess = true;
}

int main(int ac, char *av[])
{
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

  std::string tcpConnectionArg = "";

  if (ac > 1)
  {
    // Fetch the connection string from the command line.
    tcpConnectionArg = av[1];
  }

  // Creates a provider instance at Data Layer backend to provide data to Data Layer clients
  std::string providerConnection = getConnection(tcpConnectionArg, DL_TCP_BACKEND_FIRST_PORT);
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

  std::cout << "INFO Creating Data Layer branch 'all-data'" << std::endl;
  auto providerNodeStatic = new ProviderNodeAllData(dlProvider, "all-data/", false);
  providerNodeStatic->RegisterNodes();

  auto providerNodeDynamic = new ProviderNodeAllData(dlProvider, "all-data/", true);
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