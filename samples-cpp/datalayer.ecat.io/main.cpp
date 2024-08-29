/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <chrono>
#include <comm/datalayer/memory_generated.h>
#include <csignal>
#include <cstddef>
#include <iostream>
#include <map>
#include <ostream>
#include <stdio.h>
#include <thread>
#include <vector>

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/memory_map_generated.h"

#include "ctrlx_datalayer_helper.h"

static bool g_endProcess = false;
static void signalHandler(int signal) {
  std::cout << "INFO Signal received: " << signal << std::endl;
  g_endProcess = true;
}

static bool getMemoryMap(comm::datalayer::DatalayerSystem &datalayerSystem,
                  comm::datalayer::IClient *client,
                  std::shared_ptr<comm::datalayer::IMemoryUser> &memoryUser,
                  const std::string &address, uint32_t *revision,
                  std::vector<uint32_t> *byteIndices,
                  std::vector<uint8_t> *bitIndices) {

  comm::datalayer::DlResult dlResult;
  comm::datalayer::Variant variantMap;

  const std::string addressMap = address + "/map";
  std::cout << "INFO Reading " << addressMap << std::endl;
  dlResult = client->readSync(addressMap, &variantMap);
  if (comm::datalayer::STATUS_FAILED(dlResult)) {
    std::cout << "ERROR Reading " << addressMap
              << " failed with: " << dlResult.toString() << std::endl;
    return false;
  }

  std::cout << "INFO Opening " << address << std::endl;
  dlResult = datalayerSystem.factory()->openMemory(memoryUser, address);
  if (comm::datalayer::STATUS_FAILED(dlResult)) {
    std::cout << "ERROR Opening " << address
              << " failed with: " << dlResult.toString() << std::endl;
    return false;
  }

  int tries = 10;

  do {
    if (g_endProcess) {
      return false;
    }

    std::cout << "INFO Try #" << std::to_string(tries) << " to get memory map"
              << std::endl;

    dlResult = memoryUser->getMemoryMap(variantMap);
    if (comm::datalayer::STATUS_SUCCEEDED(dlResult)) {
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    tries--;
  } while (tries > 0);
  if (comm::datalayer::STATUS_FAILED(dlResult)) {
    std::cout << "ERROR getMemoryMap() failed with: " << dlResult.toString()
              << std::endl;
    return false;
  }

  dlResult =
      variantMap.verifyFlatbuffers(comm::datalayer::VerifyMemoryMapBuffer);
  if (comm::datalayer::STATUS_FAILED(dlResult)) {
    std::cout << "ERROR verifyFlatbuffers() failed with: "
              << dlResult.toString() << std::endl;
    return false;
  }

  // At this point we can take the data. It is important to always check the
  // revision number. The revision number will be different if the layout will
  // be changed.
  auto memMap = comm::datalayer::GetMemoryMap(variantMap.getData());
  *revision = memMap->revision();

  std::string name;
  uint32_t offset = 0;
  uint32_t byteIndex = 0;
  uint8_t bitIndex = 0;

  std::cout << "INFO Reading Bitoffset and Name" << std::endl;
  for (auto variable = memMap->variables()->begin();
       variable != memMap->variables()->end(); variable++) {
    if (variable->bitsize() == 1) {
      name = variable->name()->str();
      offset = variable->bitoffset();
      byteIndex = offset >> 3;
      bitIndex = offset & 7;
      byteIndices->push_back(byteIndex);
      bitIndices->push_back(bitIndex);
      std::cout << "INFO Name: " << name << " Bitoffset: " << offset
                << " Byte index: " << byteIndex << " Bit index: " << bitIndex
                << std::endl;
    }
  }

  if (byteIndices->size() == 0) {
    std::cout << "ERROR No bit values (bitsize=1) found" << std::endl;
    return false;
  }

  std::cout << "INFO " << byteIndices->size() << " bit values (bitsize=1) found"
            << std::endl;
  return true;
}

static bool copyInputToOutput(
    comm::datalayer::DatalayerSystem &datalayerSystem,
    comm::datalayer::IClient *client,
    std::shared_ptr<comm::datalayer::IMemoryUser> &memoryUserInput,
    uint32_t revisionInput, std::vector<uint32_t> &byteIndicesInput,
    std::vector<uint8_t> &bitIndicesInput,
    std::shared_ptr<comm::datalayer::IMemoryUser> &memoryUserOutput,
    uint32_t revisionOutput, std::vector<uint32_t> &byteIndicesOutput,
    std::vector<uint8_t> &bitIndicesOutput, bool *buffer) {

  // Read input values -------------------------------------
  uint8_t *dataInput;
  comm::datalayer::DlResult dlResult =
      memoryUserInput->beginAccess(dataInput, revisionInput);
  if (comm::datalayer::STATUS_FAILED(dlResult)) {
    std::cout << "ERROR beginAccess() input memory failed with: "
              << dlResult.toString() << std::endl;
    return false;
  }

  for (size_t i = 0; i < sizeof(buffer); i++) {
    uint8_t allBits = dataInput[byteIndicesInput[i]];
    buffer[i] = (allBits & (1 << bitIndicesInput[i])) > 0;
  }

  memoryUserInput->endAccess();

  // Write putput values ---------------------------------------
  uint8_t *dataOutput;
  dlResult = memoryUserOutput->beginAccess(dataOutput, revisionOutput);
  if (comm::datalayer::STATUS_FAILED(dlResult)) {
    std::cout << "ERROR beginAccess() output memory failed with: "
              << dlResult.toString() << std::endl;
    return false;
  }

  for (size_t i = 0; i < sizeof(buffer); i++) {
    uint8_t allBits = dataOutput[byteIndicesOutput[i]];
    uint8_t bitMask = 1 << bitIndicesOutput[i];
    if (buffer[i]) {
      allBits |= bitMask;
    } else {
      bitMask = ~bitMask;
      allBits &= bitMask;
    }
    dataOutput[byteIndicesOutput[i]] = allBits;
  }

  memoryUserOutput->endAccess();

  return true;
}

static comm::datalayer::IClient *
getClientRetry(comm::datalayer::DatalayerSystem &datalayerSystem) {

  // ctrlX OS virtual with network adapter
  comm::datalayer::IClient *client = getClient(datalayerSystem);
  if (client != nullptr) {
    return client;
  }

  // ctrlX OS virtual with port forwarding
  return getClient(datalayerSystem, "10.0.2.2", "boschrexroth", "boschrexroth",
                   8443);
}

int main(void) {

  std::signal(SIGINT, signalHandler);

  comm::datalayer::DatalayerSystem datalayerSystem;
  datalayerSystem.start(false);

  comm::datalayer::IClient *client = nullptr;
  while (g_endProcess == false) {

    // Sleep after error (or at startup)
    std::this_thread::sleep_for(std::chrono::seconds(10));

    if (client != nullptr) {
      // In case of retry
      delete client;
    }
    comm::datalayer::IClient *client = getClientRetry(datalayerSystem);

    if (client == nullptr) {
      std::cout << "ERROR Client connection failed" << std::endl;
      continue;
    }

    // Get input map
    std::shared_ptr<comm::datalayer::IMemoryUser> memoryUserInput;
    std::string dlAddressInput =
        "fieldbuses/ethercat/master/instances/ethercatmaster/realtime_data/input";
    uint32_t revisionInput;
    std::vector<uint32_t> byteIndicesInput;
    std::vector<uint8_t> bitIndicesInput;

    bool boolResult =
        getMemoryMap(datalayerSystem, client, memoryUserInput, dlAddressInput,
                     &revisionInput, &byteIndicesInput, &bitIndicesInput);
    if (boolResult == false) {
      continue;
    }

    // Get output map
    std::shared_ptr<comm::datalayer::IMemoryUser> memoryUserOutput;
    std::string dlAddressOutput =
        "fieldbuses/ethercat/master/instances/ethercatmaster/realtime_data/output";
    uint32_t revisionOutput;
    std::vector<uint32_t> byteIndicesOutput;
    std::vector<uint8_t> bitIndicesOutput;

    boolResult =
        getMemoryMap(datalayerSystem, client, memoryUserOutput, dlAddressOutput,
                     &revisionOutput, &byteIndicesOutput, &bitIndicesOutput);
    if (boolResult == false) {
      continue;
    }

    // Find minimal size of bytes
    size_t minSize = byteIndicesInput.size() < byteIndicesOutput.size()
                         ? byteIndicesInput.size()
                         : byteIndicesOutput.size();
    bool *buffer = new bool[minSize];

    // Copy loop
    while (g_endProcess == false) {

     std::cout << "INFO Copying input to output..." << std::endl;

      boolResult = copyInputToOutput(
          datalayerSystem, client, memoryUserInput, revisionInput,
          byteIndicesInput, bitIndicesInput, memoryUserOutput, revisionOutput,
          byteIndicesOutput, bitIndicesOutput, buffer);
      if (boolResult == false) {
        break;
      }

      // Sleep before nect copy
      std::this_thread::sleep_for(std::chrono::seconds(5));
    }
  }

  delete client;

  datalayerSystem.stop();

  return 0;
}
