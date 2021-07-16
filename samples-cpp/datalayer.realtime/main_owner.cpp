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
 

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/memory_map_generated.h"
#include <stdio.h>
#include <signal.h>
#include <thread>
#include <iostream>

#define MEM_SIZE (100)
// Revision should be unique for this Layout, if you need a new Memorylayout define a new revision, or use checksum algorithms
#define REVISION (0)

bool endProcess = false;

static void hdl(int sig, siginfo_t *siginfo, void *context)
{
  endProcess = true;
}

comm::datalayer::Variant createMemMap(size_t size, uint32_t revision)
{
  // A memory map defines the layout of memory
  // Memory Map contains:
  // {
  //    variables : [Variables]; Arry of variables
  //    revision : uint = 0; Revisionnumber changes every time on variables changes
  // }
  // Variables definded like:
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
  for (uint32_t i = 0; i < (size/4); i++)
  {
    char name[10];
    sprintf(name, "var%u", i);
    auto variable = comm::datalayer::CreateVariableDirect(builder,
      name, // name of variable (has to be unique), can be divided by "/" for hierarchical structure
      32 * i, // bit offset of variable in memory
      32, // size of variable in bits
      comm::datalayer::TYPE_DL_UINT32.c_str()
    );
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

int main(int ac, char* av[])
{
  comm::datalayer::DlResult result;
  comm::datalayer::Variant data;
  comm::datalayer::DatalayerSystem datalayer;
  // Start datalayer without a broker - a broker already exists in rexroth-automationcore snap
  datalayer.start(false);

  // Lets create a loopback realtime memory - every cycle this example will copy output memory to input memory
  // Note:
  //  * input is data an owner provides for other realtime users
  //  * output is data on owner consumes from other realtime users
  comm::datalayer::IProvider *provider = datalayer.factory()->createProvider("tcp://boschrexroth:boschrexroth@192.168.1.1:2070"); // "tcp://boschrexroth:boschrexroth@127.0.0.1:2070"
  result = provider->start();
  if (comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "Starting provider failed with: " << result.toString() << std::endl;
    std::cout << "... Exiting" << std::endl;
    return -1;
  }

  std::cout << "creating some realtime memory" << std::endl;
  std::shared_ptr<comm::datalayer::IMemoryOwner> input;
  std::shared_ptr<comm::datalayer::IMemoryOwner> output;
  result = datalayer.factory()->createMemorySync(input, "example/rt/input", provider, MEM_SIZE, comm::datalayer::MemoryType_Input);
  if (comm::datalayer::STATUS_FAILED(result))
    std::cout << "creation of input failed with: " << result.toString() << std::endl;
  result = datalayer.factory()->createMemorySync(output, "example/rt/output", provider, MEM_SIZE, comm::datalayer::MemoryType_Output);
  if (comm::datalayer::STATUS_FAILED(result))
    std::cout << "creation of output failed with: " << result.toString() << std::endl;

  // Memory layout is defined by owner
  // * defining a memory layout is optional
  // * a realtime user can access the layout
  // * you can access all variables using non realtime client directly
  comm::datalayer::Variant memMap = createMemMap(MEM_SIZE, REVISION);
  std::cout << "Set memory map for input and output buffer" << std::endl;
  result = input->setMemoryMap(memMap);
  if (comm::datalayer::STATUS_FAILED(result))
    std::cout << "Set input memMap failed with: " << result.toString() << std::endl;
  
  result = output->setMemoryMap(memMap);
  if (comm::datalayer::STATUS_FAILED(result))
    std::cout << "Set ouput memMap failed with: " << result.toString() << std::endl;

  {
    uint8_t *inData;
    result = input->beginAccess(inData, REVISION);
    if (comm::datalayer::STATUS_SUCCEEDED(result))
    {
      std::cout <<"fill memory" << std::endl;
      memset(inData, 0, MEM_SIZE);
      input->endAccess();
    }
    else{
      std::cout <<"fill memory failed with: " << result.toString()  << std::endl;
    }
  }

  std::cout << "start copy output to input" << std::endl;

  // Structure to interrupt the do while loop with SIGINT
  struct sigaction act;
  memset(&act, '\0', sizeof(act));
  act.sa_sigaction = &hdl;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGINT, &act, NULL);

  // Copy output to input every second
  std::this_thread::sleep_for(std::chrono::seconds(1));
  do
  {
    std::cout << "check ... ";
    uint8_t *inData;
    uint8_t *outData;
    result = output->beginAccess(outData, REVISION);
    if(comm::datalayer::STATUS_SUCCEEDED(result))
    {
      // Note: you will get here if an user has written the output once
      result = input->beginAccess(inData, REVISION);
      if (comm::datalayer::STATUS_SUCCEEDED(result))
      {
        memcpy(inData, outData, MEM_SIZE);
        std::cout << "copy output to input done on user access :-)"  << std::endl;
        input->endAccess();
      }
      else{
        std::cout << "copy output to input failed with: " << result.toString() << std::endl;
      }
      output->endAccess();
    }
    else{
      std::cout << "no user connected or access to output failed with: " << result.toString()  << std::endl;
    }
    printf("\n");
    std::this_thread::sleep_for(std::chrono::seconds(1));
  } while (!endProcess);

  std::cout << "finish my work" << std::endl;

  result = datalayer.factory()->deleteMemorySync(input);
  if (comm::datalayer::STATUS_FAILED(result))
    std::cout << "Cannot delete input memory: " << result.toString() << std::endl;
  result = datalayer.factory()->deleteMemorySync(output);
  if (comm::datalayer::STATUS_FAILED(result))
    std::cout << "Cannot delete output memory: " << result.toString() << std::endl;

  result = provider->stop();
  if (comm::datalayer::STATUS_FAILED(result))
    std::cout << "Stopping provider failed with: " << result.toString() << std::endl;

  // Don't forget to delete your provider to avoid memleaks  
  delete provider;
  datalayer.stop();
}