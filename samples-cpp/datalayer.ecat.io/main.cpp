/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <map>
#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/memory_map_generated.h"

#include "ctrlx_datalayer_helper.h"

static bool g_endProcess = false;
static void signalHandler(int signal)
{
  std::cout << "INFO Signal received: " << signal << std::endl;
  g_endProcess = true;
}

int run(comm::datalayer::DatalayerSystem& datalayerSystem, comm::datalayer::IClient* client)
{
  comm::datalayer::DlResult dlResult;
  comm::datalayer::Variant outputMap;
  comm::datalayer::Variant inputMap;

  std::string address = "fieldbuses/ethercat/master/instances/ethercatmaster/realtime_data/output/map";
  std::cout << "INFO Reading " << address << std::endl;
  dlResult = client->readSync(address, &outputMap);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cout << "ERROR Reading " << address << " failed with: " << dlResult.toString() << std::endl;
    return 1;
  }

  address = "fieldbuses/ethercat/master/instances/ethercatmaster/realtime_data/input/map";
  std::cout << "INFO Reading " << address << std::endl;
  dlResult = client->readSync(address, &inputMap);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cout << "ERROR Reading " << address << " failed with: " << dlResult.toString() << std::endl;
    return 1;
  }

  // First we have to open the realtime memory, reading the whole outputs.
  uint8_t* outData;
  address = "fieldbuses/ethercat/master/instances/ethercatmaster/realtime_data/output";
  std::cout << "INFO Opening " << address << std::endl;
  std::shared_ptr<comm::datalayer::IMemoryUser> rtMemUserOutput;
  dlResult = datalayerSystem.factory()->openMemory(rtMemUserOutput, address);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cout << "ERROR Opening " << address << " failed with: " << dlResult.toString() << std::endl;
    return 1;
  }

  // We can read the inputs to get a start trigger for example
  uint8_t* inData;
  address = "fieldbuses/ethercat/master/instances/ethercatmaster/realtime_data/input";
  std::cout << "INFO Opening " << address << std::endl;
  std::shared_ptr<comm::datalayer::IMemoryUser> rtMemUserInput;
  dlResult = datalayerSystem.factory()->openMemory(rtMemUserInput, address);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cout << "ERROR Opening " << address << " failed with: " << dlResult.toString() << std::endl;
    return 1;
  }

  // After successful reading the outputs we must take the memory layout.
  // A memory owner defines the layout of the realtime memory.
  // You see it in the datalayer.realtime example (memoryowner.cpp)
  std::cout << "INFO Try to get output memory map" << std::endl;
  int tries = 10;
  do
  {
    if (g_endProcess)
    {
      return 0;
    }

    dlResult = rtMemUserOutput->getMemoryMap(outputMap);
    if (comm::datalayer::STATUS_SUCCEEDED(dlResult))
    {
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    tries--;
  } while (tries > 0);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cout << "ERROR Getting output memory map failed with: " << dlResult.toString() << std::endl;
    return 1;
  }

  std::cout << "INFO Try to get input memory map" << std::endl;
  tries = 10;
  do
  {
    if (g_endProcess)
    {
      return 0;
    }

    dlResult = rtMemUserInput->getMemoryMap(inputMap);
    if (comm::datalayer::STATUS_SUCCEEDED(dlResult))
    {
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    tries--;
  } while (tries > 0);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cout << "ERROR Getting input memory map failed with: " << dlResult.toString() << std::endl;
    return 1;
  }

  // Now we got the layout, but we have to convert it cause it is a flatbuffer
  // To imagen the result after converting the data you can take a look at the datalayertree
  // "devices/ethercatmaster/0/outputs -> map"
  dlResult = outputMap.verifyFlatbuffers(comm::datalayer::VerifyMemoryMapBuffer);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cout << "ERROR Verify output map flatbuffers failed with: " << dlResult.toString() << std::endl;
    return -1;
  }

  // Verify the Input layout, similar to the output
  dlResult = inputMap.verifyFlatbuffers(comm::datalayer::VerifyMemoryMapBuffer);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cout << "ERROR Verify input map flatbuffers failed with: " << dlResult.toString() << std::endl;
    return -1;
  }

  // At this point we can take the data. It is important to always check the revision number.
  // The revision number will be different if the layout will be changed.
  auto memMap = comm::datalayer::GetMemoryMap(outputMap.getData());
  auto revision = memMap->revision();
  std::string name;
  uint32_t offset = 0;
  std::map<std::string, uint32_t> mapOfOutputs;

  std::cout << "INFO Reading Bitoffset and Name of DOs" << std::endl;
  // Save all the outputs (name and offset) into a map
  for (auto variable = memMap->variables()->begin(); variable != memMap->variables()->end(); variable++)
  {
    if (variable->bitsize() == 1)
    {
      name = variable->name()->str();
      offset = variable->bitoffset();
      std::cout << "INFO Name: " << name << " Bitoffset: " << offset << std::endl;
      mapOfOutputs.insert(std::make_pair(name, offset));
    }
  }

  // Iputs
  auto memMapInputs = comm::datalayer::GetMemoryMap(inputMap.getData());
  auto revisionIn = memMapInputs->revision();
  std::string nameIn;
  uint32_t offsetIn = 0;
  std::map<std::string, uint32_t> mapOfInputs;

  std::cout << "INFO Reading Bitoffset and Name of DIs" << std::endl;
  // Save all the inputs (name and offset) into a map
  for (auto variable = memMapInputs->variables()->begin(); variable != memMapInputs->variables()->end(); variable++)
  {
    if (variable->bitsize() == 1)
    {
      name = variable->name()->str();
      offset = variable->bitoffset();
      std::cout << "INFO Name: " << name << " Bitoffset: " << offset << std::endl;
      mapOfInputs.insert(std::make_pair(nameIn, offsetIn));
    }
  }

  // Check every second the first input channel if it's true start to toggle
  bool startbit = false;
  while (!startbit)
  {
    if (g_endProcess)
    {
      return 0;
    }

    std::map<std::string, uint32_t>::iterator it = mapOfInputs.find("S20_EC_BK/S20_DI_16_1/Digital_Input_Channels.DI_Channel_01_Terminal_Point_00_");
    std::cout << "INFO Read Input: " << it->first << " at Offset " << it->second << std::endl;

    dlResult = rtMemUserInput->beginAccess(inData, revisionIn);
    if (comm::datalayer::STATUS_FAILED(dlResult))
    {
      break;
    }

    uint8_t value = inData[it->second / 8];
    if (value)
    {
      startbit = true;
    }

    rtMemUserInput->endAccess();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  // Begin to write outputs until SIGINT is send
  bool temp = false;
  while (true)
  {
    if (g_endProcess)
    {
      return 0;
    }

    std::cout << "INFO Start to toggle" << std::endl;

    // If first channel would be unset -> end toggle
    std::map<std::string, uint32_t>::iterator itInput = mapOfInputs.find("S20_EC_BK/S20_DI_16_1/Digital_Input_Channels.DI_Channel_01_Terminal_Point_00_");
    dlResult = rtMemUserInput->beginAccess(inData, revisionIn);
    if (comm::datalayer::STATUS_FAILED(dlResult))
    {
      break;
    }

    uint8_t value = inData[itInput->second / 8];
    if (!value)
    {
      g_endProcess = true;
    }

    rtMemUserInput->endAccess();
    if (!temp)
    {
      // Create iterator for the map, after that we can iterate through all variables and set the
      std::map<std::string, uint32_t>::iterator it = mapOfOutputs.begin();
      while (it != mapOfOutputs.end())
      {
        if (g_endProcess)
        {
          return 0;
        }

        std::cout << "INFO Set Output: " << it->first << " :: Offset " << it->second << std::endl;

        // Memory is form beginAccess till endAccess locked, so do not do stuff you won't need
        dlResult = rtMemUserOutput->beginAccess(outData, revision);
        if (comm::datalayer::STATUS_FAILED(dlResult))
        {
          break;
        }

        // set toggle bit
        uint8_t value = outData[it->second / 8];
        value |= 1 << (it->second % 8);
        outData[it->second / 8] = value;

        rtMemUserOutput->endAccess();
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
        if (g_endProcess)
        {
          return 0;
        }

        std::cout << "INFO Unset Output: " << it->first << " :: Offset " << it->second << std::endl;

        // Memory is form beginAccess till endAccess locked, so do not do stuff you won't need
        dlResult = rtMemUserOutput->beginAccess(outData, revision);
        if (comm::datalayer::STATUS_FAILED(dlResult))
        {
          break;
        }

        // Set toggle bit
        uint8_t value = outData[it->second / 8];
        value &= ~(1 << (it->second % 8));
        outData[it->second / 8] = value;

        rtMemUserOutput->endAccess();
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
    if (g_endProcess)
    {
      return 0;
    }

    dlResult = rtMemUserOutput->beginAccess(outData, revision);
    if (comm::datalayer::STATUS_FAILED(dlResult))
    {
      break;
    }

    outData[it->second / 8] = 0x00;
    
    rtMemUserOutput->endAccess();
    it++;
  }

  return 0;
}

comm::datalayer::IClient* getClientRetry(comm::datalayer::DatalayerSystem& datalayerSystem)
{

  comm::datalayer::IClient* client = getClient(datalayerSystem);
  if (client != nullptr)
  {
    return client;
  }

  return getClient(datalayerSystem, "10.0.2.2", "boschrexroth", "boschrexroth", 8443);
}

int main(void)
{
  std::signal(SIGINT, signalHandler);

  comm::datalayer::DatalayerSystem datalayerSystem;
  datalayerSystem.start(false);

  comm::datalayer::IClient* client = getClientRetry(datalayerSystem);

  int exitCode = 1;
  if (client == nullptr)
  {
    std::cout << "ERROR Client connection failed" << std::endl;
  }
  else
  {
    exitCode = run(datalayerSystem, client);
    std::cout << "INFO Shutting down application" << std::endl;
  }

  delete client;
  datalayerSystem.stop();

  return exitCode;
}
