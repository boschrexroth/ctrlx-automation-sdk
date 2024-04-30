/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <csignal>
#include <thread>
#include <iostream>

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/memory_map_generated.h"

 // Create end process flag witch is set to true if SIGINT is send
static bool g_endProcess = false;
static void sigIntHandler(int signal)
{
  std::cout << "signal: " << signal << std::endl;
  g_endProcess = true;
}

static void closeUserMemory(comm::datalayer::DatalayerSystem& datalayer,
                            std::shared_ptr<comm::datalayer::IMemoryUser>& userMemory)
{
  if (!userMemory)
  {
    return;
  }

  std::cout << "INFO Closing shared user memory" << std::endl;

  comm::datalayer::DlResult result = datalayer.factory()->closeMemory(userMemory);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "WARNING Closing shared user memory failed with: " << result.toString() << std::endl;
  }
}
// Cleanup closes the memory and stop the datalayersystem
static void cleanup(comm::datalayer::DatalayerSystem& datalayer,
                    std::shared_ptr<comm::datalayer::IMemoryUser>& input,
                    std::shared_ptr<comm::datalayer::IMemoryUser>& output)
{
  closeUserMemory(datalayer, input);
  closeUserMemory(datalayer, output);

  datalayer.stop();
}

int main()
{
  comm::datalayer::DatalayerSystem datalayerSystem;

  comm::datalayer::DlResult result;
  comm::datalayer::Variant data;

  std::shared_ptr<comm::datalayer::IMemoryUser> inputMemUser;
  uint8_t* inData;
  
  std::shared_ptr<comm::datalayer::IMemoryUser> outputMemUser;
  uint8_t* outData;

  std::string dlBasePath = "sdk/cpp/datalayer/sharedmemory/";

  // Structure to interrupt the do while loops with SIGINT
  std::signal(SIGINT, sigIntHandler);

  // A shared memory user always starts the ctrlX Data Layer WITHOUT a broker:
  // On the ctrlX the rexroth-automationcore snap has started the broker,
  // in an App Builder Env the shared memory owner has started it.
  datalayerSystem.start(false);

  // Input user memory ------------------------------------------------

  std::string dlPath = dlBasePath + "input";
  std::cout << "INFO Opening shared memory " << dlPath << " ---------------------" << std::endl;
  result = datalayerSystem.factory()->openMemory(inputMemUser, dlPath);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Opening shared memory " << dlPath << " failed with: " << result.toString() << std::endl;
    cleanup(datalayerSystem, inputMemUser, outputMemUser);
    return 1;
  }

  // Because openMemory() works async it might be still running.
  // We have to try getMemoryMap() several times.
  int tries = 10;
  std::cout << "INFO Try to get memory map of " << dlPath << std::endl;
  do
  {
    if (g_endProcess)
    {
      cleanup(datalayerSystem, inputMemUser, outputMemUser);
      return 0;
    }

    // Wait, then try getMemoryMap()
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    result = inputMemUser->getMemoryMap(data);

    if (comm::datalayer::STATUS_SUCCEEDED(result))
    {
      break;
    }

    tries--;
  } while (tries > 0);

  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR getMemoryMap of " << dlPath << " failed with: " << result.toString() << std::endl;
    cleanup(datalayerSystem, inputMemUser, outputMemUser);
    return 1;
  }

  auto memMap = comm::datalayer::GetMemoryMap(data.getData());
  auto revision = memMap->revision();
  std::cout << "INFO Revision Number of Memory Map: " << revision << std::endl;

  // Output user memory ------------------------------------------------

  dlPath = dlBasePath + "output";
  std::cout << "INFO Opening shared memory " << dlPath << " ---------------------" << std::endl;

  result = datalayerSystem.factory()->openMemory(outputMemUser, dlPath);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Opening shared memory " << dlPath << " failed with: " << result.toString() << std::endl;
    cleanup(datalayerSystem, inputMemUser, outputMemUser);
    return 1;
  }

  // Output memory map is equal to input memory map - so we renounce reading it.
  
  std::cout << "INFO Start reading from input memory and writing to output memory" << std::endl;
  do
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    result = inputMemUser->beginAccess(inData, revision);
    if (STATUS_FAILED(result))
    {
      std::cout << "ERROR Accessing input memory failed with: " << result.toString() << std::endl;
      break;
    }

    std::cout << "INFO First byte of input memory: '";
    std::cout << unsigned(inData[0]);
    std::cout << "' - will be incremented and written to output." << std::endl;

    result = outputMemUser->beginAccess(outData, revision);
    if (STATUS_FAILED(result))
    {
      std::cout << "ERROR Accessing output memory failed with: " << result.toString() << std::endl;
      inputMemUser->endAccess();
      break;
    }

    size_t size = 0;
    if (comm::datalayer::STATUS_FAILED(outputMemUser->getSize(size)))
    {
      std::cout << "WARNING Can't read output memory size" << std::endl;
    }
    else
    {
      // Read 
      for (uint32_t i = 0; i < size; i++)
      {
        uint8_t next = inData[i] + 1;
        if (next == 0)
        {
          // Skip 0 - 0 will be interpreted a 'not changed' in the system test
          next = 1;
        }
        outData[i] = next;
      }
    }

    inputMemUser->endAccess();
    outputMemUser->endAccess();

  } while (!g_endProcess);


  cleanup(datalayerSystem, inputMemUser, outputMemUser);

  return g_endProcess ? 0 : 1;
}
