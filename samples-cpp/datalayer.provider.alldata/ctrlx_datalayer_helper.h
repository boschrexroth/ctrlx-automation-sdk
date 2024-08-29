/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef CTRLX_DATALAYER_HELPER_H
#define CTRLX_DATALAYER_HELPER_H

 /*! \file
  *
  * This header file provides auxiliary methods to create ctrlX Datalayer client and provider connections to ctrlX CORE devices.
  * It can be used for both running in an app build environment (QEMU VM) and within the snap environment on the ctrlX CORE.
  *
  * Feel free to use it in your projects and to change it if necessary.
  *
  * For ease of use, the default values for IP address, user, password and SSL port are chosen to match the settings of a
  * newly created ctrlX CORE device:
  *
  *    ip="192.168.1.1"
  *    user="boschrexroth"
  *    password="boschrexroth"
  *    ssl_port=443
  *
  * If these values do not suit your use case, explicitly pass the parameters that require different values.
  * Here some examples:
  *
  * 1. ctrlX CORE or ctrlX COREvirtual with another IP address, user and password:
  *
  *     client, client_connection = get_client(system, ip="192.168.1.100", user="admin", password="-$_U/{X$aG}Z3/e<")
  *
  * 2. ctrlX COREvirtual with port forwarding running on the same host as the app build environment (QEMU VM):
  *
  *     client, client_connection = get_client(system, ip="10.0.2.2", ssl_port=8443)
  *
  *     Remarks:
  *     10.0.2.2 is the IP address of the host from the point of view of the app build environment (QEMU VM).
  *     8443 is the host port which is forwarded to the SSL port (=433) of the ctrlX COREvirtual
  *
  *
  * IMPORTANT:
  * You need not change the parameter settings before building a snap and installing the snap on a ctrlX CORE.
  * The method get_connection_string detects the snap environment and uses automatically inter process communication.
  * Therefor the connection string to the ctrlX Datalayer is:
  *
  *     "ipc://"
  *
  */

#include <stdio.h>
#include <iostream>

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"

  //! Retrieve environment variable SNAP
static const char* snapPath()
{
  return std::getenv("SNAP");
}

//! Test if code is runnning in snap environment
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
  const std::string& ip = "192.168.1.1",
  const std::string& user = "boschrexroth",
  const std::string& password = "boschrexroth",
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

//! Creates a Datalayer Client instance and connects. Automatically reconnects if the connection is interrupted.
//! @param[in] datalayerSystem Datalayer.System instance
//! @param[in] ip       IP address of the ctrlX CORE: 10.0.2.2 is ctrlX COREvirtual with port forwarding
//! @param[in] user     User name
//! @param[in] password The password
//! @param[in] sslPort  The port number for SSL: 8443 if ctrlX COREvirtual with port forwarding 8443:443
//! @result IClient instance or nullptr on error
static comm::datalayer::IClient3* getClient(comm::datalayer::DatalayerSystem& datalayerSystem,
                                           const std::string& ip = "192.168.1.1",
                                           const std::string& user = "boschrexroth",
                                           const std::string& password = "boschrexroth", int sslPort = 443)
{
  std::string connectionString = getConnectionString(ip, user, password, sslPort);
  comm::datalayer::IClient3* client = datalayerSystem.factory()->createClient3(connectionString);
  if (client->isConnected())
  {
    return client;
  }

  delete client;

  return nullptr;
}

//! Creates a Datalayer Provider instance and connects. Automatically reconnects if the connection is interrupted.
//! @param[in] datalayerSystem Datalayer.System instance
//! @param[in] ip       IP address of the ctrlX CORE: 10.0.2.2 is ctrlX COREvirtual with port forwarding
//! @param[in] user     User name
//! @param[in] password The password
//! @param[in] sslPort  The port number for SSL: 8443 if ctrlX COREvirtual with port forwarding 8443:443
//! @result IProvider instance or nullptr on error
static comm::datalayer::IProvider3* getProvider(comm::datalayer::DatalayerSystem& datalayerSystem,
                                               const std::string& ip = "192.168.1.1",
                                               const std::string& user = "boschrexroth",
                                               const std::string& password = "boschrexroth", int sslPort = 443)
{
  std::string connectionString = getConnectionString(ip, user, password, sslPort);
  comm::datalayer::IProvider3* provider = datalayerSystem.factory4()->createProvider3(connectionString);
  if (provider->start() == DL_OK)
  {
    return provider;
  }

  delete provider;

  return nullptr;
}

#endif
