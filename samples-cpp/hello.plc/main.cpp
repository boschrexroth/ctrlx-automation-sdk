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

/*
 * This snap-program determines symbolic PLC variables from plc-program 'PLC_PRG' and writes in all variables of
 * the type string 'HelloDeveloper', in all integers '1337' and in all floats the value '0.815f'.
 */

#include <stdio.h>
#include <iostream>

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"

#include "ctrlx_datalayer_helper.h"

//! Get PLC apps from vector
comm::datalayer::DlResult getApps(comm::datalayer::Variant &data, std::vector<std::string> &apps)
{
  if (data.getType() == comm::datalayer::VariantType::ARRAY_OF_STRING)
  {
    const char **str = data;
    for (uint32_t i = 0; i < data.getCount(); i++)
    {
      std::cout << "Apps: " << str[i] << std::endl;
      apps.push_back(str[i]);
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  return comm::datalayer::DlResult::DL_RT_NOVALIDDATA;
}

// Function takes the value from "i" to "data" and
// then writes the new "data" to the address "addr" of the data layer client "IClient"
comm::datalayer::DlResult setIntValue(comm::datalayer::Variant &data, comm::datalayer::IClient *client, std::string addr, int16_t i)
{
  comm::datalayer::DlResult result;
  result = data.setValue(i);
  if (STATUS_SUCCEEDED(result))
  {
    result = client->writeSync(addr, &data);
    if (STATUS_SUCCEEDED(result))
    {
      return comm::datalayer::DlResult::DL_OK;
    }
  }

  std::cout << "setting int value failed with: " << result.toString() << std::endl;
  return comm::datalayer::DlResult::DL_FAILED;
}

// Function takes the value from "str" to "data" and
// then writes the new "data" to the address "addr" of the data layer client "IClient"
comm::datalayer::DlResult setStringValue(comm::datalayer::Variant &data, comm::datalayer::IClient *client, std::string addr, std::string str)
{
  comm::datalayer::DlResult result;
  result = data.setValue(str);
  if (STATUS_SUCCEEDED(result))
  {
    result = client->writeSync(addr, &data);
    if (STATUS_SUCCEEDED(result))
    {
      return comm::datalayer::DlResult::DL_OK;
    }
  }

  std::cout << "setting string value failed with: " << result.toString() << std::endl;
  return comm::datalayer::DlResult::DL_FAILED;
}

// Function takes the value from "value" to "data" and
// then writes the new "data" to the address "addr" of the data layer client "IClient"
comm::datalayer::DlResult setFloatValue(comm::datalayer::Variant &data, comm::datalayer::IClient *client, std::string addr, float value)
{
  comm::datalayer::DlResult result;
  result = data.setValue(value);
  if (STATUS_SUCCEEDED(result))
  {
    result = client->writeSync(addr, &data);
    if (STATUS_SUCCEEDED(result))
    {
      return comm::datalayer::DlResult::DL_OK;
    }
  }

  std::cout << "setting value failed with: " << result.toString() << std::endl;
  return comm::datalayer::DlResult::DL_FAILED;
}

int run(comm::datalayer::IClient *client)
{

  // getting all PLC Applications nodes
  comm::datalayer::Variant data;
  comm::datalayer::DlResult result = client->browseSync("plc/app", &data);
  if (STATUS_FAILED(result))
  {
    std::cout << "ERROR Browsing of PLC nodes 'plc/app' failed with: " << result.toString() << std::endl;
    return 1;
  }

  // Create dynamic string-array
  std::vector<std::string> plcApp;
  // write all Plc-Applications from "data" in the dynamic string-array "plcApp"
  result = getApps(data, plcApp);
  if (STATUS_FAILED(result))
  {
    std::cout << "ERROR Reading data of PLC nodes 'plc/app' failed with: " << result.toString() << std::endl;
    return 1;
  }

  if (plcApp.size() <= 0)
  {
    std::cout << "ERROR No PLC application available" << std::endl;
    return 4;
  }

  // set the path to the symbol variables of the first application in the plc-program "PLC_PRG"
  std::string addr = "plc/app/" + plcApp[0] + "/sym/PLC_PRG/";
  // read all symbol variables into "data"
  result = client->browseSync(addr, &data);
  if (STATUS_FAILED(result))
  {
    std::cout << "ERROR Browsing of symbols of '"<< addr << "' failed with: " << result.toString() << std::endl;
    return 5;
  }

  // Create dynamic string-array
  std::vector<std::string> symVar;
  // write all symbol-variables from "data" in the dynamic string-array "symVar"
  result = getApps(data, symVar);
  if (STATUS_FAILED(result))
  {
    std::cout << "ERROR Reading data failed with: " << result.toString() << std::endl;
    return 6;
  }

  // run through the loop for each symbol-variable in "data"
  for (uint32_t i = 0; i < data.getCount(); i++)
  {
    comm::datalayer::Variant value;
    // write the path to the current symbol variable in "temp"
    // set the path to the symbol-variable
    std::string temp = addr + symVar[i];
    // read the value of this symbol-variable
    result = client->readSync(temp, &value);
    if (STATUS_FAILED(result))
    {
      std::cout << "WARN Reading of " << temp << "failed with: " << result.toString() << std::endl;
    }

    // determine the data type of the symbol-variable read
    switch (value.getType())
    {
    // Set 'helloDeveloper' to string-Variable in the plc application
    case comm::datalayer::VariantType::STRING:
      result = setStringValue(value, client, temp, "HelloDeveloper");
      if (STATUS_FAILED(result))
      {
        std::cout << "WARN Set STRING value into '" << temp << "' failed with: " << result.toString() << std::endl;
      }
      else
      {
        std::cout << "INFO Set STRING value into '" << temp << "' succeeded" << std::endl;
      }
      break;

    // Set '1337' to INT-Variable in the plc application
    case comm::datalayer::VariantType::INT16:
      result = setIntValue(value, client, temp, 1337);
      if (STATUS_FAILED(result))
      {
        std::cout << "WARN Set INT16 value into '" << temp << "' failed with: " << result.toString() << std::endl;
      }
      else
      {
        std::cout << "INFO Set INT16 value into '" << temp << "' succeeded" << std::endl;
      }
      break;

    // Set '0.815f' to Float-Variable in the plc application
    case comm::datalayer::VariantType::FLOAT32:
      result = setFloatValue(value, client, temp, 0.815f);
      if (STATUS_FAILED(result))
      {
        std::cout << "WARN Set FLOAT32 value into '" << temp << "' failed with: " << result.toString() << std::endl;
      }
      else
      {
        std::cout << "INFO Set FLOAT32 value into '" << temp << "' succeeded" << std::endl;
      }
      break;

    default:
      std::cout << "WARN Variant type not supported  - only STRING, INT16 or FLOAT32 supported" << std::endl;
      break;
    }
  }

  return 0;
}

int main()
{
  int exitCode = 0;
  comm::datalayer::DatalayerSystem datalayer;
  datalayer.start(false);

  comm::datalayer::IClient *client;

  // Try ctrlX CORE (virtual)
  client = getClient(datalayer);
  if (client == nullptr)
  {
    // Try ctrlX CORE virtual with port forwarding
    client = getClient(datalayer, "10.0.2.2", "boschrexroth", "boschrexroth", 8443);
  }
  if (client == nullptr)
  {
    std::cout << "ERROR Creating client connection failed." << std::endl;
    exitCode = 1;
  }
  else
  {
    exitCode = run(client);
    delete client;
  }

  datalayer.stop();

  return exitCode;
}