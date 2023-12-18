/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <iostream>

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"

//! Retrieve environment variable SNAP
//! @result The content of SNAP ales nullptr if not available
static const char *snapPath()
{
  return std::getenv("SNAP");
}

//! Test if code is runnning in snap environment
//! @result True if running snap environment
static bool isSnap()
{
  return snapPath() != nullptr;
}

//! Get Datalayer connection string
//! @param[in] ip       IP address of the ctrlX CORE: 10.0.2.2 is ctrlX COREvirtual with port forwarding
//! @param[in] user     User name
//! @param[in] password The password
//! @param[in] sslPort  The port number for SSL: 8443 if ctrlX COREvirtual with port forwarding 8443:443
//! @result Connection string
static std::string getConnectionString(
    const std::string &ip = "192.168.1.1",
    const std::string &user = "boschrexroth",
    const std::string &password = "boschrexroth",
    int sslPort = 443)
{
  if (isSnap())
  {
    return DL_IPC;
  }

  std::string connectionString = DL_TCP + user + std::string(":") + password + std::string("@") + ip;

  if (443 == sslPort)
  {
    return connectionString;
  }

  return connectionString + std::string("?sslport=") + std::to_string(sslPort);
}

// The enry point of the snap.
// Runs an endless loop until stopped or ctrlX Data Layer clientconnection breaks.
int main()
{

  std::cout << "INFO Starting ctrlX Data Layer system (without broker)" << std::endl;
  comm::datalayer::DatalayerSystem datalayerSystem;
  datalayerSystem.start(false);

  auto connectionString = getConnectionString(); // default: ctrlX CORE or ctrlX COREvirtual with Network Adpater
  std::cout << "INFO Creating ctrlX Data Layer client connection to " << connectionString << " ..." << std::endl;
  auto dataLayerClient = datalayerSystem.factory()->createClient(connectionString);

  if (dataLayerClient->isConnected() == false)
  {
    delete dataLayerClient;

    auto connectionString2 = getConnectionString("10.0.2.2", "boschrexroth", "boschrexroth", 8443); // ctrlX COREvirtual with Port Forwarding
    std::cout << "WARNING Cannot connect to " << connectionString << " trying " << connectionString2 << std::endl;

    auto dataLayerClient = datalayerSystem.factory()->createClient(connectionString2);
  }

  int counter = 1;
  while (dataLayerClient->isConnected())
  {
    std::cout << "Loop #" << counter++ << std::endl;

    // Synchronous read of a ctrlX Data Layer node with a simple data type -----------------------------------------------------------
    auto cpuUtilisationPercentAddress = "framework/metrics/system/cpu-utilisation-percent";
    comm::datalayer::Variant cpuUtilisationPercentValue;
    std::cout << "INFO Reading " << cpuUtilisationPercentAddress << " synchronously..." << std::endl;
    auto result = dataLayerClient->readSync(cpuUtilisationPercentAddress, &cpuUtilisationPercentValue);
    if (result != DL_OK)
    {
      std::cout << "WARN Reading " << cpuUtilisationPercentAddress << " failed with: " << result.toString() << std::endl;
    }
    else
    {
      if (cpuUtilisationPercentValue.getType() == comm::datalayer::VariantType::FLOAT64)
      {
        std::cout << "INFO Value of " << cpuUtilisationPercentAddress << ": " << double(cpuUtilisationPercentValue) << " %" << std::endl;
      }
      else
      {
        std::cout << "WARN Value of " << cpuUtilisationPercentAddress << " has unexpected type: " << cpuUtilisationPercentValue.typeAsString() << std::endl;
      }
    }

    // Synchronous read of a ctrlX Data Layer node with a simple flatbuffers data type -----------------------------------------------------------
    auto stateAddress = "framework/state";
    comm::datalayer::Variant stateValue;
    std::cout << "INFO Reading " << stateAddress << " synchronously..." << std::endl;
    result = dataLayerClient->readSync(stateAddress, &stateValue);
    if (result != DL_OK)
    {
      std::cout << "WARN Reading " << stateAddress << " failed with: " << result.toString() << std::endl;
    }
    else
    {
      auto stateTypeAddress = "types/systemhandler/state";
      comm::datalayer::Variant stateType;
      result = dataLayerClient->readSync(stateTypeAddress, &stateType);
      std::cout << "INFO " << stateAddress << " has type: " << stateValue.typeAsString() << std::endl;
      std::string stateAsJsonString;
      result = datalayerSystem.jsonConverter()->generateJson(stateValue, stateType, &stateAsJsonString);
      std::cout << "INFO Value of " << stateAddress << ":" << std::endl
                << stateAsJsonString << std::endl;
    }

    std::cout << "INFO Sleeping..." << std::endl;
    sleep(2);
  }

  std::cout << "ERROR ctrlX Data Layer connection is broken" << std::endl;

  delete dataLayerClient;
  datalayerSystem.stop();

  return 1; // We exit because an error happend
}
