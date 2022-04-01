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

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/memory_map_generated.h"
#include <stdio.h>
#include <signal.h>
#include <thread>
#include <iostream>

#include "ctrlx_datalayer_helper.h"

// Creat end process flag witch is set to true if SIGINT is send
bool endProcess = false;
static void hdl(int sig, siginfo_t *siginfo, void *context)
{
  endProcess = true;
}

void closeUserMemory(comm::datalayer::DatalayerSystem *datalayer,
                     std::shared_ptr<comm::datalayer::IMemoryUser> userMemory)
{
  if (userMemory == nullptr)
  {
    return;
  }

  std::cout << "INFO Closing realtime user memory" << std::endl;

  comm::datalayer::DlResult result = datalayer->factory()->closeMemory(userMemory);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "WARN Closing realtime user memory failed with: " << result.toString() << std::endl;
  }
}
// Cleanup closes the memory and stop the datalayersystem
void cleanup(comm::datalayer::DatalayerSystem *datalayer,
             comm::datalayer::IClient *client,
             std::shared_ptr<comm::datalayer::IMemoryUser> input,
             std::shared_ptr<comm::datalayer::IMemoryUser> output)
{
  closeUserMemory(datalayer, input);
  closeUserMemory(datalayer, output);

  delete client;
  datalayer->stop();
}

int main(int ac, char *av[])
{
  comm::datalayer::DlResult result;
  comm::datalayer::Variant data;
  comm::datalayer::DatalayerSystem datalayer;
  uint8_t *inData;
  uint8_t *outData;

  // Start datalayer without a broker - a broker already exists in automation snap
  datalayer.start(false);

  comm::datalayer::IClient *client = getClient(datalayer); // ctrlX CORE (virtual)
  //comm::datalayer::IClient *client = getClient(datalayer, "10.0.2.2", "boschrexroth", "boschrexroth", 8443); // ctrlX CORE virtual with port forwarding
  if (client == nullptr)
  {
    std::cout << "ERROR Creating Datalayer client connection" << std::endl;
    datalayer.stop();
    return 1;
  }

  // Read the Memory Map to take the revision number, the revision number is necessary to work with the memory
  result = client->readSync("sdk-cpp-realtime/rt/output/map", &data);
  if (STATUS_FAILED(result))
  {
    std::cout << "ERROR Reading Memory Map failed with " << result.toString() << std::endl;
    delete client;
    datalayer.stop();
    return 2;
  }

  auto memMap = comm::datalayer::GetMemoryMap(data.getData());
  auto revision = memMap->revision();
  std::cout << "INFO Revision Number of Memory Map: " << revision << std::endl;

  std::cout << "INFO Opening realtime memory areas" << std::endl;
  std::shared_ptr<comm::datalayer::IMemoryUser> input;
  result = datalayer.factory()->openMemory(input, "sdk-cpp-realtime/rt/input");
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Opening of sdk-cpp-realtime/rt/input failed with: " << result.toString() << std::endl;
    delete client;
    datalayer.stop();
    return 3;
  }

  std::shared_ptr<comm::datalayer::IMemoryUser> output;
  result = datalayer.factory()->openMemory(output, "sdk-cpp-realtime/rt/output");
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Opening of sdk-cpp-realtime/rt/output failed with: " << result.toString() << std::endl;
    delete client;
    datalayer.stop();
    return 4;
  }

  // Structure to interrupt the do while loop with SIGINT
  struct sigaction act;
  memset(&act, '\0', sizeof(act));
  act.sa_sigaction = &hdl;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGINT, &act, NULL);

  std::cout << "INFO start producing some output data" << std::endl;
  do
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    result = input->beginAccess(inData, revision);
    if (STATUS_FAILED(result))
    {
      std::cout << "WARN Input memory not present at the moment: " << result.toString() << std::endl;
      continue;
    }

    std::cout << "INFO first byte of input: " << inData[0] << std::endl;

    result = output->beginAccess(outData, revision);
    if (STATUS_FAILED(result))
    {
      std::cout << "WARN Output memory not present at the moment: " << result.toString() << std::endl;
      input->endAccess();
      continue;
    }

    std::cout << "INFO Checking memory size" << std::endl;
    size_t size = 0;
    if (comm::datalayer::STATUS_FAILED(output->getSize(size)))
    {
      std::cout << "WARN Can't read memory size" << std::endl;
    }
    else
    {
      std::cout << "INFO Increment output" << std::endl;
      for (uint32_t i = 0; i < size; i++)
      {
        outData[i] = outData[i] + 1;
      }
    }

    input->endAccess();
    output->endAccess();

  } while (!endProcess);

  cleanup(&datalayer, client, input, output);

  return 0;
}