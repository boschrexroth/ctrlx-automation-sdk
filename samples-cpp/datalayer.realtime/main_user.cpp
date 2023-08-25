/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/memory_map_generated.h"
#include <stdio.h>
#include <csignal>
#include <thread>
#include <iostream>

 // Create end process flag witch is set to true if SIGINT is send
static bool g_endProcess = false;
static void signalHandler(int signal)
{
  std::cout << "signal: " << signal << std::endl;
  g_endProcess = true;
}

static void closeUserMemory(comm::datalayer::DatalayerSystem* datalayer,
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
    std::cout << "WARNING Closing realtime user memory failed with: " << result.toString() << std::endl;
  }
}
// Cleanup closes the memory and stop the datalayersystem
static void cleanup(comm::datalayer::DatalayerSystem* datalayer,
                    std::shared_ptr<comm::datalayer::IMemoryUser> input,
                    std::shared_ptr<comm::datalayer::IMemoryUser> output)
{
  closeUserMemory(datalayer, input);
  closeUserMemory(datalayer, output);

  datalayer->stop();
}

int main()
{
  comm::datalayer::DlResult result;
  comm::datalayer::Variant data;
  comm::datalayer::DatalayerSystem datalayer;
  uint8_t* inData;
  uint8_t* outData;

  // A realtime memory user always starts the ctrlX Data Layer WITHOUT a broker:
  // On the ctrlX the rexroth-automationcore snap has started the broker,
  // in an App Builder Env the realtime memory owner has started it.
  datalayer.start(false);

  std::cout << "INFO Opening realtime memory sdk-cpp-realtime/rt/input ------------------" << std::endl;
  std::shared_ptr<comm::datalayer::IMemoryUser> input;
  result = datalayer.factory()->openMemory(input, "sdk-cpp-realtime/rt/input");
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Opening of sdk-cpp-realtime/rt/input failed with: " << result.toString() << std::endl;
    cleanup(&datalayer, nullptr, nullptr);
    return 1;
  }

  std::cout << "INFO Opening realtime memory sdk-cpp-realtime/rt/output ------------------" << std::endl;
  std::shared_ptr<comm::datalayer::IMemoryUser> output;
  result = datalayer.factory()->openMemory(output, "sdk-cpp-realtime/rt/output");
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Opening of sdk-cpp-realtime/rt/output failed with: " << result.toString() << std::endl;
    cleanup(&datalayer, input, output);
    return 1;
  }

  // Structure to interrupt the do while loops with SIGINT
  std::signal(SIGINT, signalHandler);

  while (true)
  {

    result = input->getMemoryMap(data);
    std::cout << "INFO Try to get memory map of sdk-cpp-realtime/rt/input: " << result.toString() << std::endl;

    if (comm::datalayer::STATUS_SUCCEEDED(result))
    {
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    if (g_endProcess)
    {
      cleanup(&datalayer, input, output);
      return 0;
    }
  };

  auto memMap = comm::datalayer::GetMemoryMap(data.getData());
  auto revision = memMap->revision();
  std::cout << "INFO Revision Number of Memory Map: " << revision << std::endl;
  std::cout << "INFO Start reading from input memory and writing to output memory" << std::endl;
  do
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    result = input->beginAccess(inData, revision);
    if (STATUS_FAILED(result))
    {
      std::cout << "WARNING Accessing input memory failed with: " << result.toString() << std::endl;
      break;
    }

    std::cout << "INFO First byte of input memory: '";
    std::cout << unsigned(inData[0]);
    std::cout << "' - will be incremented and written to output." << std::endl;

    result = output->beginAccess(outData, revision);
    if (STATUS_FAILED(result))
    {
      std::cout << "WARNING Accessing output memory failed with: " << result.toString() << std::endl;
      input->endAccess();
      break;
    }

    size_t size = 0;
    if (comm::datalayer::STATUS_FAILED(output->getSize(size)))
    {
      std::cout << "WARNING Can't read output memory size" << std::endl;
    }
    else
    {
      for (uint32_t i = 0; i < size; i++)
      {
        uint8_t next = outData[i] + 1;
        if (next == 0)
        {
          // Skip 0 - 0 will be interpreted a 'not changed' in the system test
          next = 1;
        }
        outData[i] = next;
      }
    }

    input->endAccess();
    output->endAccess();

  } while (!g_endProcess);


  cleanup(&datalayer, input, output);

  return 0;
}
