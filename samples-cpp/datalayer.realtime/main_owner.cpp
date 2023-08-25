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

#define MEM_SIZE (100)
 // Revision should be unique for this Layout, if you need a new memory layout define a new revision, or use checksum algorithms
#define REVISION (0)

 // Create end process flag witch is set to true if SIGINT is send
static bool g_endProcess = false;
static void signalHandler(int signal)
{
  std::cout << "signal: " << signal << std::endl;
  g_endProcess = true;
}

static void deleteOwnerMemory(comm::datalayer::DatalayerSystem* datalayer,
                              std::shared_ptr<comm::datalayer::IMemoryOwner> ownerMemory)
{
  if (ownerMemory == nullptr)
  {
    return;
  }

  std::cout << "INFO Deleting realtime owner memory" << std::endl;
  comm::datalayer::DlResult result = datalayer->factory()->deleteMemorySync(ownerMemory);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "WARNING Closing realtime owner memory failed with: " << result.toString() << std::endl;
  }
}

// Cleanup closes the memory and stop the datalayersystem
static void cleanup(comm::datalayer::DatalayerSystem* datalayer,
                    comm::datalayer::IProvider* provider,
                    std::shared_ptr<comm::datalayer::IMemoryOwner> input,
                    std::shared_ptr<comm::datalayer::IMemoryOwner> output)
{
  deleteOwnerMemory(datalayer, input);
  deleteOwnerMemory(datalayer, output);

  if (provider != nullptr)
  {
    provider->stop();
    delete provider;
  }

  datalayer->stop();
}

static comm::datalayer::Variant createMemMap(size_t size, uint32_t revision)
{
  // A memory map defines the layout of memory
  // Memory Map contains:
  // {
  //    variables : [Variables]; Array of variables
  //    revision : uint = 0; Revisionnumber changes every time on variables changes
  // }
  // Variables defined like:
  // {
  //   name : string (key); Name of the variable
  //   bitoffset : uint;    Offset (in bits) of variable in memory
  //   bitsize : uint;      Size (in bits) of variable in memory
  //   type : string;       Type information
  // }
  // It defines where all variables lays in memory, the type and valid informations
  // Here is a simple layout
  flatbuffers::FlatBufferBuilder builder;
  std::vector<flatbuffers::Offset<comm::datalayer::Variable>> vecVariables;
  for (uint32_t i = 0; i < (size / 4); i++)
  {
    char name[10];
    sprintf(name, "var%u", i);
    auto variable = comm::datalayer::CreateVariableDirect(builder,
                                                          name,   // name of variable (has to be unique), can be divided by "/" for hierarchical structure
                                                          32 * i, // bit offset of variable in memory
                                                          32,     // size of variable in bits
                                                          comm::datalayer::TYPE_DL_UINT32.c_str());
    vecVariables.push_back(variable);
  }
  auto variables = builder.CreateVectorOfSortedTables(&vecVariables);
  comm::datalayer::MemoryMapBuilder memmap(builder);
  memmap.add_revision(revision);
  memmap.add_variables(variables);
  auto memmapFinished = memmap.Finish();
  builder.Finish(memmapFinished);

  comm::datalayer::Variant result;
  result.copyFlatbuffers(builder);
  return result;
}

// Test if code is runnning in snap environment (of a ctrlX CORE)
static bool isSnap()
{
  return std::getenv("SNAP") != nullptr;
}

int main(void)
{
  comm::datalayer::DlResult result;
  comm::datalayer::DatalayerSystem datalayer;

  if (isSnap())
  {
    // Running on ctrlX CORE: Start datalayer without a broker - a broker already exists in rexroth-automationcore snap
    std::cout << "INFO Using existing ctrlX Data Layer." << std::endl;
    datalayer.start(false);
  }
  else
  {
    // Running in App Builder Env: Start datalayer WITH a broker - we need him for exchange of data via shared memory
    std::cout << "INFO Starting own ctrlX Data Layer." << std::endl;
    datalayer.start(true);
  }


  // Get provider instance
  comm::datalayer::IProvider* provider = datalayer.factory()->createProvider(DL_IPC);
  if (provider == nullptr)
  {
    std::cout << "ERROR Creating ctrlX Data Layer provider connection." << std::endl;
    cleanup(&datalayer, nullptr, nullptr, nullptr);
    return 1;
  }

  result = provider->start();
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Provider could not be started." << std::endl;
    cleanup(&datalayer, provider, nullptr, nullptr);
    return 1;
  }

  if (provider->isConnected() == false)
  {
    std::cout << "ERROR Provider is NOT connected." << std::endl;
    cleanup(&datalayer, provider, nullptr, nullptr);
    return 1;
  }

  // Lets create a loopback realtime memory - every cycle this example will copy output memory to input memory
  // Note:
  //  * input is data an owner provides for other realtime users
  //  * output is data on owner consumes from other realtime users
  std::cout << "INFO Creating realtime memory nodes: sdk-cpp-realtime/rt/" << std::endl;
  std::shared_ptr<comm::datalayer::IMemoryOwner> input;
  result = datalayer.factory()->createMemorySync(input, "sdk-cpp-realtime/rt/input", provider, MEM_SIZE, comm::datalayer::MemoryType_Input);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Creation of sdk-cpp-realtime/rt/input failed with: " << result.toString() << std::endl;
    cleanup(&datalayer, provider, input, nullptr);
    return 1;
  }

  std::shared_ptr<comm::datalayer::IMemoryOwner> output;
  result = datalayer.factory()->createMemorySync(output, "sdk-cpp-realtime/rt/output", provider, MEM_SIZE, comm::datalayer::MemoryType_Output);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Creation of sdk-cpp-realtime/rt/output failed with: " << result.toString() << std::endl;
    cleanup(&datalayer, provider, input, output);
    return 1;
  }

  // Memory layout is defined by owner
  // * defining a memory layout is optional
  // * a realtime user can access the layout
  // * you can access all variables using non realtime client directly
  comm::datalayer::Variant memMap = createMemMap(MEM_SIZE, REVISION);
  std::cout << "Setting memory map for input and output buffer" << std::endl;
  result = input->setMemoryMap(memMap);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Setting input memMap failed with: " << result.toString() << std::endl;
    cleanup(&datalayer, provider, input, output);
    return 1;
  }

  result = output->setMemoryMap(memMap);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Setting output memMap failed with: " << result.toString() << std::endl;
    cleanup(&datalayer, provider, input, output);
    return 1;
  }

  // Input ---------------------------------------
  uint8_t* inData = nullptr;
  //inData = new uint8_t[MEM_SIZE];
  result = input->beginAccess(inData, REVISION);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Accessing input memory failed with: " << result.toString() << std::endl;
    cleanup(&datalayer, provider, input, output);
    return 1;
  }

  std::cout << "INFO Filling memory with start value 0" << std::endl;
  memset(inData, 0, MEM_SIZE);

  input->endAccess();

  // Structure to interrupt the do while loop with SIGINT ----------------------------------
  std::signal(SIGINT, signalHandler);

  std::cout << "INFO Start copy output to input" << std::endl;
  do
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    uint8_t* inData;
    uint8_t* outData;
    result = output->beginAccess(outData, REVISION);
    if (comm::datalayer::STATUS_FAILED(result))
    {
      std::cout << "WARNING Accessing output memory failed with: " << result.toString() << std::endl;
      continue;
    }

    // Note: you will get here if an user has written the output once
    result = input->beginAccess(inData, REVISION);
    if (comm::datalayer::STATUS_FAILED(result))
    {
      std::cout << "WARNING Accessing input memory failed with: " << result.toString() << std::endl;
      output->endAccess();
      continue;
    }

    std::cout << "INFO First byte of output memory: '";
    std::cout << unsigned(outData[0]);
    std::cout << "' - will be copied to input" << std::endl;

    memcpy(inData, outData, MEM_SIZE);

    input->endAccess();
    output->endAccess();

  } while (!g_endProcess);

  cleanup(&datalayer, provider, input, output);

  return 0;
}
