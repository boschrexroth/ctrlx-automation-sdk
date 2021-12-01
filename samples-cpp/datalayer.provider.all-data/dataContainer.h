#ifndef DATA_CONTAINER_H
#define DATA_CONTAINER_H

#include <iostream>
#include <vector>
#include <cfloat>

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/variant.h"

#include "comm/datalayer/metadata_generated.h"

#include "sampleSchema_generated.h"

/*
  
  Class DataContainer:
  
  This class is just a Data Container used by the class ProviderNodeAllData.
  Under a ProviderNodeAllData instance the whole Data Layer sub branch and the seen nodes are 'virtual'.
  Instead of heavy IProviderNode instances a ProviderNodeAllData node creates and manages lightweight DataContainer 
  instances.
  
  Within a DataContainer instance only a comm::datalayer::Variant is stored to represent the node data.
  ErrorCode and Error are used to 'simulate' an error state.
  
*/
class DataContainer
{

public:
  // The Data Layer address of the virtual node
  std::string Address;

  // The Data of the virtual node
  comm::datalayer::Variant Data;

  // The Metadata of the virtual node
  comm::datalayer::Variant Metadata;

  // The error code of the virtual node: <0: Error, ==0: OK; >0: Warning
  int ErrorCode = 0;      
  
  // The error text 
  std::string Error = "";
  
  // Constructor
  DataContainer(const std::string &address, const comm::datalayer::Variant &data, const comm::datalayer::Variant &meatadata)
  {
    Address = address;
    Data = data; // A copy is made
    Metadata = meatadata; // A copy is made
  }
};

#endif