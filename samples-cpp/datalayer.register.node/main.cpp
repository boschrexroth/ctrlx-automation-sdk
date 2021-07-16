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
 

// The example app datalayer.register.node creates a new provider 
// with node 'myData' and different type elements to the ctrlX Data Layer.

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/metadata_generated.h"
#include <iostream>
#include <signal.h>
#include <thread>

#include "sampleSchema_generated.h"

// Add some signal Handling so we are able to abort the program with sending sigint
bool endProcess = false;
static void sigHandler (int sig, siginfo_t *siginfo, void *context)
{
  endProcess = true;
}

using comm::datalayer::IProviderNode;

// Basic class Provider node interface for providing data to the system
class MyProviderNode : public IProviderNode {
public:
  MyProviderNode(comm::datalayer::Variant value)
  {
    m_data = value;
  };
  virtual ~MyProviderNode() override
  {};

  // Create function of an object. Function will be called whenever a object should be created.
  virtual void onCreate(const std::string &address, const comm::datalayer::Variant* data, const comm::datalayer::IProviderNode::ResponseCallback &callback) override
  {
    callback(comm::datalayer::DlResult::DL_FAILED, nullptr);
  }

  // Read function of a node. Function will be called whenever a node should be read.
  virtual void onRead(const std::string &address, const comm::datalayer::Variant* data, const comm::datalayer::IProviderNode::ResponseCallback &callback) override
  {
    comm::datalayer::Variant result;
    result = m_data;
    callback(comm::datalayer::DlResult::DL_OK, &result);
  }

  // Write function of a node. Function will be called whenever a node should be written.
  virtual void onWrite(const std::string &address, const comm::datalayer::Variant* data, const comm::datalayer::IProviderNode::ResponseCallback &callback) override
  {
    if(data->getType() == m_data.getType())
    {
      m_data = *data;
#ifdef MY_DEBUG
// If we get in here the app stopes until SIGCONT
      if (address.compare("myData/myDebug") == 0)
      {
        std::cout << "Raising SIGSTOP" << std::endl;
        raise(SIGSTOP);
        std::cout << "... Continue..." << std::endl;
      }
#endif

      callback(comm::datalayer::DlResult::DL_OK, data);
    }
    else
    {
      callback(comm::datalayer::DlResult::DL_TYPE_MISMATCH, nullptr);
    }
  }


  // Remove function for an object. Function will be called whenever a object should be removed.
  virtual void onRemove(const std::string &address, const comm::datalayer::IProviderNode::ResponseCallback &callback) override
  {
    callback(comm::datalayer::DlResult::DL_FAILED, nullptr);
  }

  // Browse function of a node. Function will be called to determine children of a node.
  virtual void onBrowse(const std::string &address, const comm::datalayer::IProviderNode::ResponseCallback &callback) override
  {
    callback(comm::datalayer::DlResult::DL_FAILED, nullptr);
  }

  // Read function of metadata of an object. Function will be called whenever a node should be written.
  virtual void onMetadata(const std::string &address, const comm::datalayer::IProviderNode::ResponseCallback &callback) override
  {
    flatbuffers::FlatBufferBuilder builder;
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<comm::datalayer::Reference>>> references;
    auto emptyString = builder.CreateString("This is a Description");

    if (address.compare("myData/myFlatbuffer") == 0)
    {
      flatbuffers::Offset<comm::datalayer::Reference> vecReferences[] =
      {
        comm::datalayer::CreateReferenceDirect(builder, "readType", "types/sampleSchema/inertialValue"),
      };

      references = builder.CreateVectorOfSortedTables(vecReferences, 1);
    }

    // Set allowed operations
    comm::datalayer::AllowedOperationsBuilder allowedOperations(builder);
    allowedOperations.add_read(true);
    allowedOperations.add_write(false);
    allowedOperations.add_create(true);
    allowedOperations.add_delete_(false);
    auto operations = allowedOperations.Finish();

    // Create metadata
    comm::datalayer::MetadataBuilder metadata(builder);
    metadata.add_description(emptyString);
    metadata.add_descriptionUrl(emptyString);
    metadata.add_operations(operations);
    if (address.compare("myData/myFlatbuffer") == 0)
    {
      metadata.add_references(references);
    }
    auto metaFinished = metadata.Finish();
    builder.Finish(metaFinished);

    comm::datalayer::Variant variant;
    variant.shareFlatbuffers(builder);
    callback(comm::datalayer::DlResult::DL_OK, &variant);
  }
  
private:
  comm::datalayer::Variant m_data;
};


int main(int ac, char* av[])
{
#ifdef MY_DEBUG
  raise(SIGSTOP);
#endif

  comm::datalayer::DatalayerSystem datalayer;
  comm::datalayer::DlResult result;
  // Starts the ctrlX Data Layer system without a new broker because one broker is already running on ctrlX device
  datalayer.start(false);
  std::cout << "Register 'myData' as root element with 4 nodes 'myFlatbuffer', 'myFloat', 'myString' and 'myInt64'" << std::endl;
  
  // Creates a provider at Data Layer backend to provide data to Data Layer clients
  comm::datalayer::IProvider *myProvider = datalayer.factory()->createProvider("tcp://boschrexroth:boschrexroth@192.168.1.1:2070"); //or "tcp://boschrexroth:boschrexroth@192.168.1.1:2070

  // Create some simple dummy data
  comm::datalayer::Variant myString;
  myString.setValue("Hello ctrlX AUTOMATION sample string");
  comm::datalayer::Variant myFloat;
  myFloat.setValue(0.815f);
  comm::datalayer::Variant myInt64;
  myInt64.setValue((int64_t)-123456789);

  // Create dummy flatbuffer data
  flatbuffers::FlatBufferBuilder builder;
  auto actInertialValue = sample::schema::CreateInertialValue(builder, 33, -427, 911);
  builder.Finish(actInertialValue);
  comm::datalayer::Variant myFlatbuffer;
  myFlatbuffer.shareFlatbuffers(builder);

  // Register type of flatbuffer value
  // If running on SNAP environment, use $SNAP environment variable for register the bfbs
  auto bfbsPath = getenv("SNAP") != nullptr ? strcat(getenv("SNAP"), "/sampleSchema.bfbs") : "bfbs/sampleSchema.bfbs";
  result = myProvider->registerType("types/sampleSchema/inertialValue", bfbsPath);
  if(STATUS_FAILED(result))
  {
    std::cout << "Register type 'types/sampleSchema/inertialValue' failed with: " << result.toString() << std::endl;
  }
  else
  {
    std::cout << "Register type 'types/sampleSchema/inertialValue' was successful!" << std::endl;
  }

  // Register a node as flatbuffer value
  result = myProvider->registerNode("myData/myFlatbuffer", new MyProviderNode(myFlatbuffer));
  if(STATUS_FAILED(result))
  {
    std::cout << "Register node 'myData/myFlatbuffer' failed with: " << result.toString() << std::endl;
  }
  else
  {
    std::cout << "Register node 'myData/myFlatbuffer' was successful!" << std::endl;
  }

  // Register a node as string value
  result = myProvider->registerNode("myData/myString", new MyProviderNode(myString));
  if(STATUS_FAILED(result))
  {
    std::cout << "Register node 'myData/myString' failed with: " << result.toString() << std::endl;
  }
  else
  {
    std::cout << "Register node 'myData/myString' was successful!" << std::endl;
  }

  // Register a node as float value
  result = myProvider->registerNode("myData/myFloat", new MyProviderNode(myFloat));
  if(STATUS_FAILED(result))
  {
    std::cout << "Register node 'myData/myFloat' failed with: " << result.toString() << std::endl;
  }
  else
  {
    std::cout << "Register node 'myData/myFloat' was successful!" << std::endl;
  }

  // Register a node as int64 value
  result = myProvider->registerNode("myData/myInt64", new MyProviderNode(myInt64));
  if(STATUS_FAILED(result))
  {
    std::cout << "Register node 'myData/myInt64' failed with: " << result.toString() << std::endl;
  }
  else
  {
    std::cout << "Register node 'myData/myInt64' was successful!" << std::endl;
  }

//Stop on Debugbuilds to attach gdb
//Register a new Node to Break.
#ifdef MY_DEBUG
  comm::datalayer::Variant myDebug;
  myDebug.setValue(false);
  result = myProvider->registerNode("myData/myDebug", new MyProviderNode(myDebug));
  if(STATUS_FAILED(result))
  {
    std::cout << "Register node 'myData/myDebug' failed with: " << result.toString() << std::endl;
  }

  std::cout << "Register node 'myData/myDebug' was successful!" << std::endl;
#endif

  // Start to add Code here to register more nodes
  // Start your ctrlX Data Layer provider
  result = myProvider->start();
  if (STATUS_FAILED(result))
  {
    std::cout << "Starting the Data Layer provider failed with: " << result.toString() << std::endl;
    // If the provider could not be started exit the program
    return -1;
  }
  else
  {
    std::cout << "Provider was started!" << std::endl;
  } 

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  if (!myProvider->isConnected())
  {
    std::cout << "Failed to connect to Data Layer broker!" << std::endl;
    return -1;
  }

  // Prepare signal structure to interrupt the endless loop with ctrl + c 
  struct sigaction act;
  memset (&act, '\0', sizeof(act));
  act.sa_sigaction = &sigHandler;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGINT, &act, NULL);
  
  do
  {
    /* code */
    // Time to interrupt this thread for lower cpu utilization
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  } while (!endProcess);

  // Unregister type and nodes
  myProvider->unregisterType("types/sampleSchema/inertialValue");
  myProvider->unregisterNode("myData/myFlatbuffer");
  myProvider->unregisterNode("myData/myString");
  myProvider->unregisterNode("myData/myFloat");
  myProvider->unregisterNode("myData/myInt64");
  std::cout << std::endl << "Provider type and nodes were unregistered!" << std::endl;

  // Stop Datalayer Provider
  myProvider->stop();
  std::cout << "Provider was stopped!" << std::endl;
  // Delete it to free your memory
  delete myProvider;
  std::cout << "Provider was deleted!" << std::endl;
}