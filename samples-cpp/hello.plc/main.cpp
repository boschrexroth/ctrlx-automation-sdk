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

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include <stdio.h>
#include <iostream>

// Function sorts values from "data" into a dynamic array of the type "string"
comm::datalayer::DlResult getVector (comm::datalayer::Variant &data, std::vector<std::string> &string)
{
  if (data.getType() == comm::datalayer::VariantType::ARRAY_OF_STRING)
  {
    const char ** str = data;
    for (uint32_t i = 0; i < data.getCount(); i++)
    {
      std::cout << "Apps: " << str[i] << std::endl;
      string.push_back(str[i]);
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  return comm::datalayer::DlResult::DL_RT_NOVALIDDATA;
}

// Function takes the value from "i" to "data" and
// then writes the new "data" to the address "addr" of the data layer client "IClient"
comm::datalayer::DlResult setIntValue (comm::datalayer::Variant &data, comm::datalayer::IClient* client, std::string addr, int16_t i)
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
comm::datalayer::DlResult setStringValue (comm::datalayer::Variant &data, comm::datalayer::IClient* client, std::string addr, std::string str)
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
comm::datalayer::DlResult setFloatValue (comm::datalayer::Variant &data, comm::datalayer::IClient* client, std::string addr, float value)
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

int main(int ac, char* av[])
{
  comm::datalayer::DlResult result;
  comm::datalayer::Variant data;
  comm::datalayer::DatalayerSystem datalayer;
  datalayer.start(false);

  // connect to client
  // * in process communication (runs as bundle of rexroth-automation) use factory()->createClient()
  // * inter process communication (runs on same target) use factory()->createClient(DL_IPC_AUTO)
  // * on other device use  factory()->createClient("tcp://<user>:<password>@<ip>:2069")
  comm::datalayer::IClient *client = datalayer.factory()->createClient("tcp://boschrexroth:boschrexroth@127.0.0.1:2069"); // or "tcp://boschrexroth:boschrexroth@192.168.1.1:2069"
    
  // set timeout of all requests to 2 seconds
  client->setTimeout(comm::datalayer::TimeoutSetting::Ping, 2000);

  // ping datalayer on control
  result = client->pingSync();
  if (STATUS_FAILED(result))
  {
    std::cout << "ping datalayer client failed with: " << result.toString() << std::endl;
    return -1;
  }
  std::cout << "ping datalayer client successful" << std::endl;
   
  
  // getting all PLC Applications nodes
  result = client->browseSync("plc/app", &data);
  if (STATUS_FAILED(result))
  {
    std::cout << "browsing of PLC applications failed with: " << result.toString() << std::endl;
    return -1;
  }
  
  // Create dynamic string-array
  std::vector<std::string> plcApp;
  // write all Plc-Applications from "data" in the dynamic string-array "plcApp"
  result = getVector(data, plcApp);
  if (STATUS_FAILED(result))
  {
    std::cout << "reading data failed with: " << result.toString() << std::endl;
  }

  // set the path to the symbol variables of the first application in the plc-program "PLC_PRG"
  std::string addr = "plc/app/" + plcApp[0] + "/sym/PLC_PRG/";
  // read all symbol variables into "data"
  result = client->browseSync(addr, &data);
  if (STATUS_FAILED(result))
  {
    std::cout << "browsing of symbols failed with: "  << result.toString() << std::endl; 
  }

  // Create dynamic string-array
  std::vector<std::string> symVar;
  // write all symbol-variables from "data" in the dynamic string-array "symVar"
  result = getVector(data, symVar);
  if (STATUS_FAILED(result))
  {
    std::cout << "reading data failed with: " << result.toString() << std::endl;
  }

  // run through the loop for each symbol-variable in "data"
  for (uint32_t i = 0; i < data.getCount(); i++)
  {
    comm::datalayer::Variant value;
    // write the path to the current symbol variable in "temp"
    // set the path to the symbol-variable
    std:: string temp = addr + symVar[i];
    // read the value of this symbol-variable
    result = client->readSync(temp, &value);
    if (STATUS_FAILED(result))
    {
      std::cout << "browsing of " << addr << "failed with: "  << result.toString() << std::endl;  
    }

    // determine the data type of the symbol-variable read
    switch (value.getType())
    {
      // Set 'helloDeveloper' to string-Variable in the plc application
      case comm::datalayer::VariantType::STRING:
        result = setStringValue(value, client, temp, "HelloDeveloper");
        if (STATUS_FAILED(result))
        {
          std::cout << "failed to set value with: " << result.toString() << std::endl; 
        }
        else
        {
          std::cout << "writing of string value successful" << std::endl;
        }
        break;
        
      // Set '1337' to INT-Variable in the plc application
      case comm::datalayer::VariantType::INT16:
        result = setIntValue(value, client, temp, 1337);
        if (STATUS_FAILED(result))
        {
          std::cout << "failed to set value with: " << result.toString() << std::endl;
        }
        else
        {
          std::cout << "writing of int16 value successful" << std::endl;
        }
        break;
        
      // Set '0.815f' to Float-Variable in the plc application
      case comm::datalayer::VariantType::FLOAT32:
        result = setFloatValue(value, client, temp, 0.815f);
        if(STATUS_FAILED(result))
        {
          std::cout << "failed to set value with: " << result.toString() << std::endl;
        }
        else
        {
          std::cout << "writing of float32 value successful" << std::endl;
        }
        break;
            
      default:
        std::cout << "data type not defined (only STRING, INT16 or FLOAT32 valid)" << std::endl;
        break;
    }
  }

  // don't forget to delete client if you don't need it anymore 
  std::cout << "delete datalayer client" << std::endl;
  delete client;
}