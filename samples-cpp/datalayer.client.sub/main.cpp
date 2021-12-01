/**
 * MIT License
 * 
 * Copyright (c) 2021 Bosch Rexroth AG
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

#include "datalayerclientsub.h"

static std::string createConnectionString(const char *credIp)
{
  if (std::getenv("SNAP") != nullptr)
  {
    return DL_IPC_AUTO;
  }

  return DL_TCP + std::string(credIp) + std::string(":") + std::to_string(DL_TCP_FRONTEND_FIRST_PORT);
}

int main(int ac, char *av[])
{

#ifdef MY_DEBUG
  std::cout << "Raising SIGSTOP" << std::endl;
  raise(SIGSTOP);
  std::cout << "... Continue..." << std::endl;
#endif

  comm::datalayer::DatalayerSystem datalayerSystem;
  datalayerSystem.start(false);

  auto connectionString = createConnectionString("boschrexroth:boschrexroth@10.0.2.2");
  auto dataLayerClientSub = new DataLayerClientSub(datalayerSystem, connectionString);
  auto result = dataLayerClientSub->connect();
  if (STATUS_FAILED(result))
  {
    dataLayerClientSub->disconnect();
    datalayerSystem.stop(false);
    return 1;
  }

  int counter = 1;
  unsigned int sleepTime=10;
  for (;;)
  {
    std::cout << "Loop #" << counter++ << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    result = dataLayerClientSub->isConnected();
    if (STATUS_FAILED(result))
    {
      dataLayerClientSub->disconnect();
      datalayerSystem.stop(false);
      return 1;
    }

    dataLayerClientSub->createSubscriptionSync("Sub1");
    dataLayerClientSub->subscribeSync("Sub1", "framework/metrics/system/cpu-utilisation-percent");
    dataLayerClientSub->subscribeSync("Sub1", "framework/metrics/system/memavailable-mb");
    std::cout << "Sleeping ..." << std::endl;
    sleep(sleepTime);
    dataLayerClientSub->unsubscribeSync("Sub1");

    dataLayerClientSub->createSubscriptionSync("Sub2");
    std::set<std::string> addresses;
    addresses.insert("framework/metrics/system/memused-percent");
    addresses.insert("framework/metrics/system/memused-mb");
    dataLayerClientSub->subscribeSync_Multi("Sub2", addresses);
    std::cout << "Sleeping ..." << std::endl;
    sleep(sleepTime);
    dataLayerClientSub->unsubscribeSync("Sub2");
  }
}