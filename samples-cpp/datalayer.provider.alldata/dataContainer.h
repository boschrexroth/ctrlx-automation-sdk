/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

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
   Under a ProviderNodeAllData instance the whole ctrlX Data Layer sub branch and the seen nodes are 'virtual'.
   Instead of heavy IProviderNode instances a ProviderNodeAllData node creates and manages lightweight DataContainer
   instances.

   Within a DataContainer instance only a comm::datalayer::Variant is stored to represent the node data.
   m_errorCode and m_error are used to 'simulate' an error state.

 */
class DataContainer final
{
public:
  // The ctrlX Data Layer address of the virtual node
  std::string m_address;

  // The Data of the virtual node
  comm::datalayer::Variant m_data;

  // The Metadata of the virtual node
  comm::datalayer::Variant m_metadata;

  // The error code of the virtual node: <0: Error, ==0: OK; >0: Warning
  int m_errorCode = 0;

  // Constructor
  DataContainer(const std::string& address, const comm::datalayer::Variant& data, const comm::datalayer::Variant& metadata)
    : m_address(address)
    , m_data(data)
    , m_metadata(metadata)
  {}
  ~DataContainer() = default;
};

#endif
