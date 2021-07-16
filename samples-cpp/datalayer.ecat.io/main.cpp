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
#include <map>
#include <iostream>
#include <chrono>
#include <thread>
#include "signal.h"

bool endProcess = false;
static void hdl (int sig, siginfo_t *siginfo, void *context)
{
  endProcess = true;
}

int main(int ac, char* av[])
{
  comm::datalayer::DlResult result;
  comm::datalayer::Variant data;
  comm::datalayer::Variant dataIn;
  comm::datalayer::DatalayerSystem datalayer;
  datalayer.start(false);
  // connect to client
  // * in process communication (runs as bundle of rexroth-automation) use factory()->createClient()
  // * inter process communication (runs on same target) use factory()->createClient(DL_IPC_AUTO)
  // * on other device use  factory()->createClient("tcp://<ip>:2069")
  comm::datalayer::IClient *client = datalayer.factory()->createClient(DL_IPC_AUTO);

  if (client->isConnected())
  {
    std::cout << "Client connected" << std::endl;
  }
  else
  {
    std::cout << "Client not connected" << std::endl;
  }
  
  result = client->readSync("fieldbuses/ethercat/master/instances/ethercatmaster/realtime_data/output/map", &data);
  std::cout << "Read returned: " << result.toString() << std::endl;

  result = client->readSync("fieldbuses/ethercat/master/instances/ethercatmaster/realtime_data/input/map", &dataIn);
  std::cout << "Read returned: " << result.toString() << std::endl;

  // Create abort struct
  struct sigaction act;
  memset(&act, '\0', sizeof(act));
  act.sa_sigaction = &hdl;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGINT, &act, NULL);

  // First we have to open the realtimememory, reading the whole outputs.
  uint8_t *outData;
  std::cout << "Opening some realtime memory" << std::endl;
  std::shared_ptr<comm::datalayer::IMemoryUser> output;
  result = datalayer.factory()->openMemory(output, "fieldbuses/ethercat/master/instances/ethercatmaster/realtime_data/output");
  if (comm::datalayer::STATUS_FAILED(result))
    std::cout << "Open the memory failed with: " << result.toString() << std::endl;
  
  // We can read the Inputs to get a Start trigger for example
  uint8_t *inData;
  std::cout << "Opening some realtime memory" << std::endl;
  std::shared_ptr<comm::datalayer::IMemoryUser> input;
  result = datalayer.factory()->openMemory(input, "fieldbuses/ethercat/master/instances/ethercatmaster/realtime_data/input");
  if (comm::datalayer::STATUS_FAILED(result))
    std::cout << "Open the memory failed with: " << result.toString() << std::endl;

    
  // After successful reading the outputs we must take the memorylayout.
  // A Memory owner defines the layout of the realtime memory
  // You see it in the datalayer.realtime example (memoryowner.cpp)
  do
  {
    std::cout << "Try to get MemoryMap" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    result = output->getMemoryMap(data);
    std::cout << "Getting MemoryMap with: " << result.toString() << std::endl;
  }  while(comm::datalayer::STATUS_FAILED(result));

  do
  {
    std::cout << "Try to get MemoryMap (Inputs)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    result = input->getMemoryMap(dataIn);
    std::cout << "Getting MemoryMap with: " << result.toString() << std::endl;
  }  while(comm::datalayer::STATUS_FAILED(result));
  
  // Now we got the layout, but we have to convert it cause it is a flatbuffer
  // To imagen the result after converting the data you can take a look at the datalayertree 
  // "devices/ethercatmaster/0/outputs -> map"
  result = data.verifyFlatbuffers(comm::datalayer::VerifyMemoryMapBuffer);
  if(comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "Verify Flatbuffers failed with: " << result.toString() << std::endl;
    return -1;
  }

  // Verify the Input layout, similar to the output
  result = dataIn.verifyFlatbuffers(comm::datalayer::VerifyMemoryMapBuffer);
  if(comm::datalayer::STATUS_FAILED(result))
  {
    std::cout << "Verify Flatbuffers (Inputs) failed with: " << result.toString() << std::endl;
    return -1;
  }

  // At this point we can take the data. It is important to always check the revision number.
  // The revision number will be different if the layout will be changed.
  auto memMap = comm::datalayer::GetMemoryMap(data.getData());
  auto revision =  memMap->revision();
  std::string name;
  uint32_t offset = 0;
  std::map<std::string, uint32_t> mapOfOutputs;
  std::cout << "Get Bitoffset and Name from Memory Map" << std::endl;

  // Iputs
  auto memMapInputs = comm::datalayer::GetMemoryMap(dataIn.getData());
  auto revisionIn =  memMapInputs->revision();
  std::string nameIn;
  uint32_t offsetIn = 0;
  std::map<std::string, uint32_t> mapOfInputs;
  std::cout << "Get Bitoffset and Name from Memory Map (Inputs) " << std::endl;
  
  // Save all the outputs (name and offset) into a map
  for (auto variable = memMap->variables()->begin(); variable != memMap->variables()->end(); variable++)
  {
    if(variable->bitsize() == 1)
    {
      name = variable->name()->str();
      std::cout << "Name of Output we found: " << name << std::endl;
      offset = variable->bitoffset();
      std::cout << "Bitoffset: " << offset << std::endl;
      mapOfOutputs.insert(std::make_pair(name, offset));
    }
  }

  // Save all the inputs (name and offset) into a map
  for (auto variable = memMapInputs->variables()->begin(); variable != memMapInputs->variables()->end(); variable++)
  {
    if(variable->bitsize() == 1)
    {
      nameIn = variable->name()->str();
      std::cout << "Name of Input we found: " << nameIn << std::endl;
      offsetIn = variable->bitoffset();
      std::cout << "Bitoffset: " << offsetIn << std::endl;
      mapOfInputs.insert(std::make_pair(nameIn, offsetIn));
    }
  }


  // Check every second the first input channel if it's true start to toggle
  bool startbit = false;
  while (!startbit)
  {
    std::map<std::string, uint32_t>::iterator it = mapOfInputs.find("S20_EC_BK/S20_DI_16_1/Digital_Input_Channels.DI_Channel_01_Terminal_Point_00_");
    std::cout << "Read Input: " << it->first << " at Offset " << it->second << std::endl;
    
    result = input->beginAccess(inData, revisionIn);
    if (comm::datalayer::STATUS_FAILED(result))
      break;

    uint8_t value = inData[it->second/8];
    if(value)
    {
      startbit = true;    
    }

    input->endAccess();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  

  // Begin to write outputs until SIGINT is send
  bool temp = false;
  while (!endProcess)
  {
    std::cout << "Start to toggle" << std::endl;

    // If first channel would be unset -> end toggle
    std::map<std::string, uint32_t>::iterator itInput = mapOfInputs.find("S20_EC_BK/S20_DI_16_1/Digital_Input_Channels.DI_Channel_01_Terminal_Point_00_");  
    result = input->beginAccess(inData, revisionIn);
    if (comm::datalayer::STATUS_FAILED(result))
      break;

    uint8_t value = inData[itInput->second/8];
    if(!value)
    {
      endProcess = true;    
    }

    input->endAccess();
    if(!temp)
    {
      // Create iterator for the map, after that we can iterate through all variables and set the 
      std::map<std::string, uint32_t>::iterator it = mapOfOutputs.begin();
      while (it != mapOfOutputs.end())
      {
        std::cout << "Set Output: " << it->first << " :: Offset " << it->second << std::endl;
        
        // Memory is form beginAccess till endAccess locked, so do not do stuff you won't need
        result = output->beginAccess(outData, revision);
        if (comm::datalayer::STATUS_FAILED(result))
          break;

        // set toggle bit
        uint8_t value = outData[it->second/8];
        value |= 1 << (it->second % 8);
        outData[it->second/8] = value;  
        output->endAccess();
        it++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }

      temp = true;
    }
    else
    {
      std::map<std::string, uint32_t>::iterator it = mapOfOutputs.begin();
      while (it != mapOfOutputs.end())
      {
        std::cout << "Unset Output: " << it->first << " :: Offset " << it->second << std::endl;
        
        // Memory is form beginAccess till endAccess locked, so do not do stuff you won't need
        result = output->beginAccess(outData, revision);
        if (comm::datalayer::STATUS_FAILED(result))
          break;

        // Set toggle bit
        uint8_t value = outData[it->second/8];
        value &= ~(1 << (it->second % 8));
        outData[it->second/8] = value;  
        output->endAccess();
        it++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }

      temp = false;
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  
  // Set all outputs to save state
  std::map<std::string, uint32_t>::iterator it = mapOfOutputs.begin();
  while (it != mapOfOutputs.end())
  {
    std::cout << "Shutting down application" << std::endl;
    result = output->beginAccess(outData, revision);
    if (comm::datalayer::STATUS_FAILED(result))
      break;

    // Shutting down application
    outData[it->second/8] = 0x00;
    output->endAccess();
    it++; 
  }
  
}