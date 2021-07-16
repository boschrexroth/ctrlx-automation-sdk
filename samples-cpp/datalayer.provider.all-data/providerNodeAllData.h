#ifndef PROVIDER_NODE_ALL_DATA_H
#define PROVIDER_NODE_ALL_DATA_H

#include <iostream>
#include <vector>
#include <cfloat>

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/variant.h"

#include "comm/datalayer/metadata_generated.h"

#include "sampleSchema_generated.h"

#include "dataContainer.h"

/*
  ProviderNodeAllData
  This class implements the IProviderNode interface and provides an 'all-data' Data Layer sub branch.
  The branch and it's nodes are virtual - Data Layer clients will see normal Data Layer nodes
  but they are all 'simulated' by an instance of this class. For each virtual node a DataContainer instance is used.

  The branch can be dynamic. In this case the branch itself and it's nodes can be manipulated (write, remove,..) by clients.
  If the branch is declared as static now changes can be made. This is useful for integration test and so on.
  
*/
class ProviderNodeAllData : public comm::datalayer::IProviderNode
{

protected:

  // List of DataContainer instances ('virtual nodes')
  std::vector<DataContainer *> _dataContainers;

  // The Data Layer Provider instance
  comm::datalayer::IProvider *_provider;

  // Data Layer root path
  std::string _addressRoot;

  // Flag: True branch is dynamic (can be changed by clients)
  bool _dynamic;

  // Root path combined with 'static' or 'dynamic'
  std::string _addressBase;

  // Search an existing DataContainer instance for this address
  DataContainer *getDataContainer(const std::string &address);

  // Create a DataContainer instance for this address with that data
  bool createDataContainer(const std::string &address, const comm::datalayer::Variant &data);

  // Create a DataContainer instance for this address with that data if setValueResult is DL_OK
  void createDataContainer(comm::datalayer::DlResult setValueResult, const std::string &addressNode, const comm::datalayer::Variant &data);

public:

  // Constructor
  ProviderNodeAllData(comm::datalayer::IProvider *provider, const std::string &addressRoot, bool dynamic);

  // Create virtual nodes and register them in the Data Layer
  void RegisterNodes();

  // Will be called by the Data Layer broker whenever a sub node should be created.
  virtual void onCreate(
      const std::string &address,
      const comm::datalayer::Variant *data,
      const comm::datalayer::IProviderNode::ResponseCallback &callback);

  // Will be called by the Data Layer broker whenever a sub node should be read.
  virtual void onRead(
      const std::string &address,
      const comm::datalayer::Variant *data,
      const comm::datalayer::IProviderNode::ResponseCallback &callback);

  // Will be called by the Data Layer broker whenever a sub node should be written.
  virtual void onWrite(
      const std::string &address,
      const comm::datalayer::Variant *data,
      const comm::datalayer::IProviderNode::ResponseCallback &callback);

  // Will be called by the Data Layer broker whenever a sub node should be removed.
  virtual void onRemove(
      const std::string &address,
      const comm::datalayer::IProviderNode::ResponseCallback &callback);

  // Will be called by the Data Layer broker to list sub nodes.
  // The returned list is merged with the list of nodes known by the broker himself.
  // So normally we return nothing.
  virtual void onBrowse(
      const std::string &address,
      const comm::datalayer::IProviderNode::ResponseCallback &callback);

  // Will be called by the Data Layer broker when he needs the properties (meta data) of the node.
  virtual void onMetadata(
      const std::string &address,
      const comm::datalayer::IProviderNode::ResponseCallback &callback);
};

static const char* snap_path() {
  return std::getenv("SNAP");
}

static bool is_snap()
{
  return snap_path() != nullptr;
}


#endif