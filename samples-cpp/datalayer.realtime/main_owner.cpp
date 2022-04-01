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

#define MEM_SIZE (100)
// Revision should be unique for this Layout, if you need a new Memorylayout define a new revision, or use checksum algorithms
#define REVISION (0)

bool endProcess = false;

static void hdl(int sig, siginfo_t *siginfo, void *context)
{
  endProcess = true;
}

void deleteOwnerMemory(comm::datalayer::DatalayerSystem *datalayer,
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
    std::cout << "WARN Closing realtime owner memory failed with: " << result.toString() << std::endl;
  }
}

// Cleanup closes the memory and stop the datalayersystem
void cleanup(comm::datalayer::DatalayerSystem *datalayer,
             comm::datalayer::IProvider *provider,
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

comm::datalayer::Variant createMemMap(size_t size, uint32_t revision)
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

int main(int ac, char *av[])
{
  comm::datalayer::DlResult result;
  comm::datalayer::Variant data;
  comm::datalayer::DatalayerSystem datalayer;
  // Start datalayer without a broker - a broker already exists in rexroth-automationcore snap
  datalayer.start(false);

  // Get provider instance
  comm::datalayer::IProvider *provider = getProvider(datalayer); // ctrlX CORE (virtual)
  //comm::datalayer::IProvider *provider = getProvider(datalayer, "10.0.2.2", "boschrexroth", "boschrexroth", 8443); // ctrlX CORE virtual with port forwarding

  if (provider == nullptr)
  {
    std::cout << "ERROR Creating Datalayer provider connection." << std::endl;
    cleanup(&datalayer, nullptr, nullptr, nullptr);
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
    cleanup(&datalayer, provider, nullptr, nullptr);
    return 2;
  }

  std::shared_ptr<comm::datalayer::IMemoryOwner> output;
  result = datalayer.factory()->createMemorySync(output, "sdk-cpp-realtime/rt/output", provider, MEM_SIZE, comm::datalayer::MemoryType_Output);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Creation of sdk-cpp-realtime/rt/output failed with: " << result.toString() << std::endl;
    cleanup(&datalayer, provider, input, nullptr);
    return 3;
  }

  // Memory layout is defined by owner
  // * defining a memory layout is optional
  // * a realtime user can access the layout
  // * you can access all variables using non realtime client directly
  comm::datalayer::Variant memMap = createMemMap(MEM_SIZE, REVISION);
  std::cout << "Set memory map for input and output buffer" << std::endl;
  result = input->setMemoryMap(memMap);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Set input memMap failed with: " << result.toString() << std::endl;
    cleanup(&datalayer, provider, input, output);
    return 4;
  }

  result = output->setMemoryMap(memMap);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Set output memMap failed with: " << result.toString() << std::endl;
    cleanup(&datalayer, provider, input, output);
    return 5;
  }

  uint8_t *inData;
  result = input->beginAccess(inData, REVISION);
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "ERROR Fill input memory failed with: " << result.toString() << std::endl;
    cleanup(&datalayer, provider, input, output);
    return 6;
  }

  std::cout << "INFO Fill memory" << std::endl;
  memset(inData, 0, MEM_SIZE);
  input->endAccess();

  // Structure to interrupt the do while loop with SIGINT
  struct sigaction act;
  memset(&act, '\0', sizeof(act));
  act.sa_sigaction = &hdl;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGINT, &act, NULL);

  std::cout << "INFO Start copy output to input" << std::endl;
  do
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    uint8_t *inData;
    uint8_t *outData;
    std::cout << "INFO Check if my memory is accessed ... " << std::endl;
    result = output->beginAccess(outData, REVISION);
    if (comm::datalayer::STATUS_FAILED(result))
    {
      std::cout << "WARN No user connected or access to output failed with: " << result.toString() << std::endl;
      continue;
    }

    // Note: you will get here if an user has written the output once
    result = input->beginAccess(inData, REVISION);
    if (comm::datalayer::STATUS_FAILED(result))
    {
      std::cout << "WARN Output to input failed with: " << result.toString() << std::endl;
      output->endAccess();
      continue;
    }

    std::cout << "INFO Copy output to input" << std::endl;
    memcpy(inData, outData, MEM_SIZE);

    input->endAccess();
    output->endAccess();

  } while (!endProcess);

  cleanup(&datalayer, provider, input, output);
}