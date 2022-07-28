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

// The example app datalayer.register.node creates a new provider
// with node 'sdk-cpp-registernode' and different type elements to the ctrlX Data Layer.

#include <iostream>
#include <filesystem>
#include <signal.h>
#include <thread>

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/metadata_generated.h"

#include "ctrlx_datalayer_helper.h"

#include "sampleSchema_generated.h"

// Add some signal Handling so we are able to abort the program with sending sigint
bool endProcess = false;

static void sigHandler(int sig, siginfo_t *siginfo, void *context)
{
  endProcess = true;
}

using comm::datalayer::IProviderNode;

// Basic class Provider node interface for providing data to the system
class MyProviderNode : public IProviderNode
{
private:
  comm::datalayer::Variant _data;

  /* Keep this comment section - it can be used as a sample for creating metadata programmatically.

  comm::datalayer::Variant _metaData;

  void createMetadata()
  {
    flatbuffers::FlatBufferBuilder builder;
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<comm::datalayer::Reference>>> references;

    auto emptyString = builder.CreateString("This is a Description");

    auto isFlatbuffers = _data.getType() == comm::datalayer::VariantType::FLATBUFFERS;
    if (isFlatbuffers)
    {
      flatbuffers::Offset<comm::datalayer::Reference> vecReferences[] =
          {
              comm::datalayer::CreateReferenceDirect(builder, "readType", "types/sampleSchema/inertialValue"),
              comm::datalayer::CreateReferenceDirect(builder, "writeType", "types/sampleSchema/inertialValue"),
          };

      references = builder.CreateVectorOfSortedTables(vecReferences, 2);
    }

    // Set allowed operations
    comm::datalayer::AllowedOperationsBuilder allowedOperations(builder);
    allowedOperations.add_read(true);
    allowedOperations.add_write(true);
    allowedOperations.add_create(false);
    allowedOperations.add_delete_(false);
    auto operations = allowedOperations.Finish();

    // Create metadata
    comm::datalayer::MetadataBuilder metadata(builder);
    metadata.add_nodeClass(comm::datalayer::NodeClass_Variable);
    metadata.add_description(emptyString);
    metadata.add_descriptionUrl(emptyString);
    metadata.add_operations(operations);
    if (isFlatbuffers)
    {
      metadata.add_references(references);
    }
    auto metaFinished = metadata.Finish();
    builder.Finish(metaFinished);

    _metaData.shareFlatbuffers(builder);
  }
*/

public:
  MyProviderNode(comm::datalayer::Variant data)
  {
    _data = data;
    //createMetadata();
  };

  virtual ~MyProviderNode() override{};

  // Create function of an object. Function will be called whenever a object should be created.
  virtual void onCreate(const std::string &address, const comm::datalayer::Variant *data, const comm::datalayer::IProviderNode::ResponseCallback &callback) override
  {
    callback(comm::datalayer::DlResult::DL_FAILED, nullptr);
  }

  // Read function of a node. Function will be called whenever a node should be read.
  virtual void onRead(const std::string &address, const comm::datalayer::Variant *data, const comm::datalayer::IProviderNode::ResponseCallback &callback) override
  {
    comm::datalayer::Variant dataRead;
    dataRead = _data;
    callback(comm::datalayer::DlResult::DL_OK, &dataRead);
  }

  // Write function of a node. Function will be called whenever a node should be written.
  virtual void onWrite(const std::string &address, const comm::datalayer::Variant *data, const comm::datalayer::IProviderNode::ResponseCallback &callback) override
  {
    if (data->getType() != _data.getType())
    {
      callback(comm::datalayer::DlResult::DL_TYPE_MISMATCH, nullptr);
    }

    _data = *data;
    callback(comm::datalayer::DlResult::DL_OK, data);
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
    // Keep this comment! Can be used as sample creating metadata programmatically.
    // callback(comm::datalayer::DlResult::DL_OK, &_metaData);

    // Take metadata from metadata.mddb
    callback(comm::datalayer::DlResult::DL_FAILED, nullptr);
  }
};

int main()
{
#ifdef MY_DEBUG
  // TODO Remove block comment to enable remote debugging
  // For further information about C++ remote debugging see public/doc/debug.md
  /*
  std::cout << "Raising SIGSTOP - waiting for attaching debugger ... " << std::endl;
  raise(SIGSTOP);  // Process
  std::cout << "... Continue..." << std::endl;
  */
#endif

  comm::datalayer::DatalayerSystem datalayerSystem;
  comm::datalayer::DlResult result;
  // Starts the ctrlX Data Layer system without a new broker because one broker is already running on ctrlX device
  datalayerSystem.start(false);

  std::cout << "INFO Register 'sdk-cpp-registernode' as root element with 4 nodes 'myFlatbuffer', 'myFloat', 'myString' and 'myInt64'" << std::endl;

  comm::datalayer::IProvider *provider = getProvider(datalayerSystem); // ctrlX CORE (virtual)
  //comm::datalayer::IProvider *provider = getProvider(datalayerSystem, "10.0.2.2", "boschrexroth", "boschrexroth", 8443); // ctrlX CORE virtual with port forwarding
  if (provider == nullptr)
  {
    std::cout << "ERROR Getting provider connection failed." << std::endl;
    datalayerSystem.stop(false);
    return 1;
  }

  // Register a node as string value
  comm::datalayer::Variant myString;
  myString.setValue("Hello ctrlX AUTOMATION sample string");
  std::cout << "INFO Register node 'sdk-cpp-registernode/myString'  " << std::endl;
  result = provider->registerNode("sdk-cpp-registernode/myString", new MyProviderNode(myString));
  if (STATUS_FAILED(result))
  {
    std::cout << "WARN Register node 'sdk-cpp-registernode/myString' failed with: " << result.toString() << std::endl;
  }

  // Register a node as float value
  comm::datalayer::Variant myFloat;
  myFloat.setValue(0.815f);
  std::cout << "INFO Register node 'sdk-cpp-registernode/myFloat'  " << std::endl;
  result = provider->registerNode("sdk-cpp-registernode/myFloat", new MyProviderNode(myFloat));
  if (STATUS_FAILED(result))
  {
    std::cout << "WARN Register node 'sdk-cpp-registernode/myFloat' failed with: " << result.toString() << std::endl;
  }

  // Register a node as int64 value
  comm::datalayer::Variant myInt64;
  myInt64.setValue((int64_t)-123456789);
  std::cout << "INFO Register node 'sdk-cpp-registernode/myInt64'  " << std::endl;
  result = provider->registerNode("sdk-cpp-registernode/myInt64", new MyProviderNode(myInt64));
  if (STATUS_FAILED(result))
  {
    std::cout << "WARN Register node 'sdk-cpp-registernode/myInt64' failed with: " << result.toString() << std::endl;
  }

  // Register type of flatbuffer value
  auto snapDir = snapPath();
  std::filesystem::path dir;
  if (snapDir == nullptr)
    dir = "compiled"; // Build environment: Compiled files are stored into that sub directory
  else
    dir = snapDir; // Snap environment: Compiled files are stored into the $SNAP directory

  std::filesystem::path fileBfbs = dir / "sampleSchema.bfbs";

  std::cout << "INFO Register type 'types/sampleSchema/inertialValue' " << fileBfbs << std::endl;
  result = provider->registerType("types/sampleSchema/inertialValue", fileBfbs);
  if (STATUS_FAILED(result))
  {
    std::cout << "WARN Register type 'types/sampleSchema/inertialValue' " << fileBfbs << " failed with: " << result.toString() << std::endl;
  }

  std::filesystem::path fileMddb = dir / "metadata.mddb";
  result = provider->registerType("datalayer", fileMddb);
  if (STATUS_FAILED(result))
  {
    std::cout << "WARN Register " << fileBfbs << " failed with: " << result.toString() << std::endl;
  }

  // Register a node as flatbuffer value
  flatbuffers::FlatBufferBuilder builder;
  auto actInertialValue = sample::schema::CreateInertialValue(builder, 33, -427, 911);
  builder.Finish(actInertialValue);
  comm::datalayer::Variant myFlatbuffer;
  myFlatbuffer.shareFlatbuffers(builder);
  std::cout << "INFO Register node 'sdk-cpp-registernode/myFlatbuffer'  " << std::endl;
  result = provider->registerNode("sdk-cpp-registernode/myFlatbuffer", new MyProviderNode(myFlatbuffer));
  if (STATUS_FAILED(result))
  {
    std::cout << "WARN Register node 'sdk-cpp-registernode/myFlatbuffer'  failed with: " << result.toString() << std::endl;
  }

  // Prepare signal structure to interrupt the endless loop with ctrl + c
  struct sigaction act;
  memset(&act, '\0', sizeof(act));
  act.sa_sigaction = &sigHandler;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGINT, &act, NULL);

  std::cout << "INFO Running endless loop - end with Ctrl+C" << std::endl;
  while (endProcess == false)
  {
    if (provider->isConnected() == false)
    {
      std::cout << "ERROR Datalayer connection broken!" << std::endl;
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  std::cout << "INFO Exiting application" << std::endl;
  if (isSnap())
  {
    std::cout << "INFO Restarting automatically" << std::endl;
  }

  provider->unregisterType("types/sampleSchema/inertialValue");

  provider->unregisterNode("sdk-cpp-registernode/myString");
  provider->unregisterNode("sdk-cpp-registernode/myFloat");
  provider->unregisterNode("sdk-cpp-registernode/myInt64");
  provider->unregisterNode("sdk-cpp-registernode/myFlatbuffer");

  // Clean up datalayer instances so that process ends properly
  provider->stop();
  delete provider;

  datalayerSystem.stop(false); // Attention: Doesn't return if any provider or client instance is still runnning

  return endProcess ? 0 : 1;
}