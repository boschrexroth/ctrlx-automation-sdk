/*
* SPDX-FileCopyrightText: Bosch Rexroth AG
*
* SPDX-License-Identifier: MIT
*/

#include <iostream>
#include <filesystem>
#include <csignal>
#include <thread>
#include <mutex>

#include <comm/datalayer/datalayer.h>
#include <comm/datalayer/datalayer_system.h>
#include <comm/datalayer/metadata_generated.h>

#include <ctrlx_datalayer_helper.h>
#include "clientpublisher.h"

 // Remote debug: Uncomment '#define REMOTE_DEBUG_ENABLED', build and install debugable snap
 // Local debug: Set '#define REMOTE_DEBUG_ENABLED' under comment, rebuild and debug
 //#define REMOTE_DEBUG_ENABLED

 // Add some signal Handling so we are able to abort the program with sending sigint
static bool g_endProcess = false;

static void sigIntHandler(int signal)
{
  std::cout << "signal: " << signal << std::endl;
  g_endProcess = true;
}

using comm::datalayer::ISubscriptionNode;

// Basic class Provider node interface for providing data to the system
class MyProviderNode : public ISubscriptionNode
{
private:
  comm::datalayer::Variant m_data;

  /* Keep this comment section - it can be used as a sample for creating metadata programmatically.
  comm::datalayer::Variant _metaData;

  void createMetadata()
  {
    flatbuffers::FlatBufferBuilder builder;
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<comm::datalayer::Reference>>> references;

    auto emptyString = builder.CreateString("This is a Description");

    auto isFlatbuffers = m_data.getType() == comm::datalayer::VariantType::FLATBUFFERS;
    if (isFlatbuffers)
    {
      flatbuffers::Offset<comm::datalayer::Reference> vecReferences[] =
          {
              comm::datalayer::CreateReferenceDirect(builder, "readType", "types/datalayer/int64"),
          };

      references = builder.CreateVectorOfSortedTables(vecReferences, 2);
    }

    // Set allowed operations
    comm::datalayer::AllowedOperationsBuilder allowedOperations(builder);
    allowedOperations.add_read(true);
    allowedOperations.add_write(false);
    allowedOperations.add_create(false);
    allowedOperations.add_delete_(false);
    auto operations = allowedOperations.Finish();

    // Create metadata
    comm::datalayer::MetadataBuilder metadata(builder);
    metadata.add_nodeClass(comm::datalayer::NodeClass_Folder);
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

std::recursive_mutex m_mtx;
std::map<comm::datalayer::ISubscription*, std::shared_ptr<ClientPublisher>> m_mapSubscriptions;

public:
  MyProviderNode()
  {
    m_data.setValue((int64_t)-123456789);
  };

  virtual ~MyProviderNode() override {
    std::cout << __func__ << std::endl;
  };

  // Create function of an object. Function will be called whenever a object should be created.
  virtual void onCreate(const std::string& address, const comm::datalayer::Variant* data, const comm::datalayer::IProviderNode::ResponseCallback& callback) override
  {
    callback(comm::datalayer::DlResult::DL_FAILED, nullptr);
  }

  // Read function of a node. Function will be called whenever a node should be read.
  virtual void onRead(const std::string& address, const comm::datalayer::Variant* data, const comm::datalayer::IProviderNode::ResponseCallback& callback) override
  {
    comm::datalayer::Variant dataRead;
    dataRead = m_data;
    callback(comm::datalayer::DlResult::DL_OK, &dataRead);
  }

  // Write function of a node. Function will be called whenever a node should be written.
  virtual void onWrite(const std::string& address, const comm::datalayer::Variant* data, const comm::datalayer::IProviderNode::ResponseCallback& callback) override
  {
    std::cout << "INFO " << __func__ << " " << address << std::endl;

    if (data->getType() != m_data.getType())
    {
      callback(comm::datalayer::DlResult::DL_TYPE_MISMATCH, nullptr);
    }

    m_data = *data;
    callback(comm::datalayer::DlResult::DL_OK, data);
  }

  // Remove function for an object. Function will be called whenever a object should be removed.
  virtual void onRemove(const std::string& address, const comm::datalayer::IProviderNode::ResponseCallback& callback) override
  {
    callback(comm::datalayer::DlResult::DL_FAILED, nullptr);
  }

  // Browse function of a node. Function will be called to determine children of a node.
  virtual void onBrowse(const std::string& address, const comm::datalayer::IProviderNode::ResponseCallback& callback) override
  {
    callback(comm::datalayer::DlResult::DL_FAILED, nullptr);
  }

  // Read function of metadata of an object. Function will be called whenever a node should be written.
  virtual void onMetadata(const std::string& address, const comm::datalayer::IProviderNode::ResponseCallback& callback) override
  {
    // Keep this comment! Can be used as sample creating metadata programmatically.
    // callback(comm::datalayer::DlResult::DL_OK, &_metaData);
    callback(comm::datalayer::DlResult::DL_FAILED, nullptr);
  }


  // Starts a subscription of a node
  // Object holding detailed information about subscription. This object can be stored till onUnsubscription() will be called for this address.
  // Address to subscribe
  virtual comm::datalayer::DlResult onSubscription(comm::datalayer::ISubscription* subscription, const std::string& address) override
  {
    std::cout << "INFO " << __func__ << " address: " << address << std::endl;
    std::lock_guard<std::recursive_mutex> guard(m_mtx);
    ClientPublisher *cp = new ClientPublisher(subscription, m_mtx);
    m_mapSubscriptions[subscription].reset(cp);
    return comm::datalayer::DlResult::DL_OK;
  }

  // Stops a subscription of a node
  virtual comm::datalayer::DlResult onUnsubscription(comm::datalayer::ISubscription* subscription, const std::string& address) override
  {
    std::cout << "INFO " << __func__ << " address: " << address << std::endl;
    auto nodes = subscription->getNodes();
    if (nodes.getCount() == 0) {
      std::lock_guard<std::recursive_mutex> guard(m_mtx);
      m_mapSubscriptions.erase(subscription);
    }
    return comm::datalayer::DlResult::DL_OK;
  }
};

int main()
{

#ifdef REMOTE_DEBUG_ENABLED
  std::cout << "Starting 'raise(SIGSTOP)', waiting for debugger.." << std::endl;
  raise(SIGSTOP);
  std::cout << "Debugger connected, continuing program..." << std::endl;
#endif
  
  // Prepare signal structure to interrupt the endless loop with ctrl + c
  std::signal(SIGINT, sigIntHandler);
  std::signal(SIGTERM, sigIntHandler);
  std::signal(SIGABRT, sigIntHandler);

  std::string dlBasePath = "sdk/cpp/datalayer/provider/sub/";

  comm::datalayer::DatalayerSystem datalayerSystem;
  // Starts the ctrlX Data Layer system without a new broker because one broker is already running on ctrlX CORE
  datalayerSystem.start(false);

  std::cout << "INFO Register '" << dlBasePath << "' with these sub nodes of type int64" << std::endl;

  {
    std::unique_ptr<comm::datalayer::IProvider3> provider;
    provider.reset(getProvider(datalayerSystem, "10.0.2.2", "boschrexroth", "boschrexroth", 8443)); // ctrlX COREvirtual with port forwarding
    if (provider == nullptr)
    {
      provider.reset(getProvider(datalayerSystem)); // ctrlX CORE (virtual)
    }

    if (provider == nullptr)
    {
      std::cout << "ERROR Getting provider connection failed." << std::endl;
      datalayerSystem.stop(false);
      return 1;
    }

    // Register a node as int64 value
    std::string dlNodesPath = dlBasePath + "**";
    std::cout << "INFO Register node " << dlNodesPath << std::endl;
    
    std::shared_ptr<MyProviderNode> pnode(new MyProviderNode());

    comm::datalayer::DlResult result = provider->registerSubscriptionNode(dlNodesPath, pnode.get());
    if (STATUS_FAILED(result))
    {
      std::cout << "WARN Register node " << dlNodesPath << " failed with: " << result.toString() << std::endl;
    }

    std::cout << "INFO Running endless loop - end with Ctrl+C" << std::endl;
    while (g_endProcess == false)
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

    provider->unregisterSubscriptionNode(dlNodesPath);
    
    // Clean up datalayer instances so that process ends properly
    provider->stop();
  }
  datalayerSystem.stop(false); // Attention: Doesn't return if any provider or client instance is still runnning

  return g_endProcess ? 0 : 1;
}
