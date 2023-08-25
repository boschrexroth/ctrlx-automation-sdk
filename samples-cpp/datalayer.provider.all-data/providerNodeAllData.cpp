/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <string>

#include "providerNodeAllData.h"
#include "sampleSchema_generated.h"
#include "ctrlx_datalayer_helper.h"

DataContainer* ProviderNodeAllData::getDataContainer(const std::string& address)
{
  for (auto dataContainer : m_dataContainers)
  {
    if (dataContainer->m_address == address)
    {
      return dataContainer;
    }
  }
  return NULL;
}

comm::datalayer::DlResult ProviderNodeAllData::createDataContainer(
  const std::string& address,
  const comm::datalayer::Variant& data)
{
  if (getDataContainer(address) != nullptr)
  {
    // We return NO error here
    return comm::datalayer::DlResult::DL_OK;
  }

  auto metadata = createMetadata(data, address);
  auto dataContainer = new DataContainer(address, data, metadata);

  m_dataContainers.emplace_back(dataContainer);

  // We register the address of our datacontainer now so that the ctrlX Data Layer will show it as "virtual node".
  // The "virtual node" has as provider this instance.
  return m_provider->registerNode(address, this);
}

void ProviderNodeAllData::createDataContainer(
  comm::datalayer::DlResult setValueResult,
  const std::string& addressNode,
  const comm::datalayer::Variant& data)
{
  const std::string address = m_addressBase + addressNode;
  if (comm::datalayer::DlResult::DL_OK != setValueResult)
  {
    std::cout << "ERROR - Data could not be set: " << address;
    return;
  }
  if (createDataContainer(address, data) != comm::datalayer::DlResult::DL_OK)
  {
    std::cout << "ERROR - DataContainer could not be created: " << address;
  }
}

comm::datalayer::Variant ProviderNodeAllData::createMetadata(const comm::datalayer::Variant& data, const std::string& address)
{

  flatbuffers::FlatBufferBuilder builder;
  auto description = builder.CreateString("This is a description for " + address);
  comm::datalayer::NodeClass nodeClass = comm::datalayer::NodeClass_Variable;

  bool isBaseFolder = address == m_addressBase;
  if (isBaseFolder)
  {
    nodeClass = comm::datalayer::NodeClass_Folder;
  }

  comm::datalayer::AllowedOperationsBuilder allowedOperations(builder);
  allowedOperations.add_read(!isBaseFolder);
  allowedOperations.add_write(!isBaseFolder && m_dynamic);
  allowedOperations.add_create(!isBaseFolder && m_dynamic);
  allowedOperations.add_delete_(!isBaseFolder && m_dynamic);
  auto operations = allowedOperations.Finish();

  flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<comm::datalayer::Reference>>> references;
  if (data.getType() == comm::datalayer::VariantType::FLATBUFFERS)
  {
    const char* typeAddr = "types/sampleSchema/inertialValue";

    if (m_dynamic)
    {
      flatbuffers::Offset<comm::datalayer::Reference> vecReferences[] =
      {
          comm::datalayer::CreateReferenceDirect(builder, "readType", typeAddr),
          comm::datalayer::CreateReferenceDirect(builder, "writeType", typeAddr),
      };
      references = builder.CreateVectorOfSortedTables(vecReferences, 2);
    }
    else
    {
      flatbuffers::Offset<comm::datalayer::Reference> vecReferences[] =
      {
          comm::datalayer::CreateReferenceDirect(builder, "readType", typeAddr),
      };
      references = builder.CreateVectorOfSortedTables(vecReferences, 1);
    }
  }

  // Create metadata
  comm::datalayer::MetadataBuilder metadata(builder);
  metadata.add_nodeClass(nodeClass);
  metadata.add_description(description);
  metadata.add_descriptionUrl(description);
  metadata.add_operations(operations);
  if (data.getType() == comm::datalayer::VariantType::FLATBUFFERS)
  {
    metadata.add_references(references);
  }

  auto metaFinished = metadata.Finish();
  builder.Finish(metaFinished);

  comm::datalayer::Variant variant;
  variant.shareFlatbuffers(builder);
  return variant;
}

ProviderNodeAllData::ProviderNodeAllData(comm::datalayer::IProvider* provider, const std::string& addressRoot, bool dynamic)
  : m_provider(provider)
  , m_addressRoot(addressRoot)
  , m_dynamic(dynamic)
  , m_addressBase(dynamic ? m_addressRoot + "dynamic/" : m_addressRoot + "static/")
{
  m_metadata = createMetadata(comm::datalayer::Variant(), m_addressBase);
}

void ProviderNodeAllData::registerNodes()
{
  comm::datalayer::DlResult result;
  result = m_provider->registerNode(m_addressBase + "**", this);
  std::cout << "registerNodes: " << result.toString() << std::endl;
  comm::datalayer::Variant data;

  result = data.setValue(true);
  createDataContainer(result, "bool8", data);

  result = data.setValue((int8_t)INT8_MAX);
  createDataContainer(result, "int8", data);

  result = data.setValue((uint8_t)UINT8_MAX);
  createDataContainer(result, "uint8", data);

  result = data.setValue((int16_t)INT16_MAX);
  createDataContainer(result, "int16", data);

  result = data.setValue((uint16_t)UINT16_MAX);
  createDataContainer(result, "uint16", data);

  result = data.setValue((int32_t)INT32_MAX);
  createDataContainer(result, "int32", data);

  result = data.setValue((uint32_t)UINT32_MAX);
  createDataContainer(result, "uint32", data);

  result = data.setValue((int64_t)INT64_MAX);
  createDataContainer(result, "int64", data);

  result = data.setValue((uint64_t)UINT64_MAX);
  createDataContainer(result, "uint64", data);

  result = data.setValue(FLT_MAX);
  createDataContainer(result, "float32", data);

  result = data.setValue(DBL_MAX);
  createDataContainer(result, "float64", data);

  result = data.setValue("This is a string");
  createDataContainer(result, "string", data);

  result = data.setTimestamp(116444736000000000);
  createDataContainer(result, "timestamp-1970-01-01-unix", data);

  result = data.setTimestamp(87790176000000000);
  createDataContainer(result, "timestamp-1879-03-14-einstein", data);

  result = data.setTimestamp(13256352000000000);
  createDataContainer(result, "timestamp-1643-01-04-newton", data);

  result = data.setTimestamp(116444736000000000);
  createDataContainer(result, "timestamp-2014-07-13-germany-soccer-world-champion", data);

  // Arrays ----------------------
  bool abool[] = {true, false, true};
  result = data.setValue(abool);
  createDataContainer(result, "array-of-bool8", data);

  int8_t aint8[] = {INT8_MIN, -1, 0, INT8_MAX};
  result = data.setValue(aint8);
  createDataContainer(result, "array-of-int8", data);

  uint8_t auint8[] = {0, UINT8_MAX};
  result = data.setValue(auint8);
  createDataContainer(result, "array-of-uint8", data);

  int16_t aint16[] = {INT16_MIN, -1, 0, INT16_MAX};
  result = data.setValue(aint16);
  createDataContainer(result, "array-of-int16", data);

  uint16_t auint16[] = {0, 32767, 32768, UINT16_MAX};
  result = data.setValue(auint16);
  createDataContainer(result, "array-of-uint16", data);

  int32_t aint32[] = {INT32_MIN, -1, 0, INT32_MAX};
  result = data.setValue(aint32);
  createDataContainer(result, "array-of-int32", data);

  uint32_t auint32[] = {0, UINT32_MAX};
  result = data.setValue(auint32);
  createDataContainer(result, "array-of-uint32", data);

  int64_t aint64[] = {INT64_MIN, -1, 0, INT64_MAX};
  result = data.setValue(aint64);
  createDataContainer(result, "array-of-int64", data);

  uint64_t auint64[] = {0, UINT64_MAX};
  result = data.setValue(auint64);
  createDataContainer(result, "array-of-uint64", data);

  float afloat[] = {FLT_MIN, -1.0, 0.0, FLT_MAX};
  result = data.setValue(afloat);
  createDataContainer(result, "array-of-float32", data);

  double adouble[] = {DBL_MIN, -1.0, 0.0, DBL_MAX};
  result = data.setValue(adouble);
  createDataContainer(result, "array-of-float64", data);

  // std::string astring[] = {"Blue", "Red", "Orange", "Yellow"};
  // const char *astring[] = {"Blue", "Red", "Orange", "Yellow"};
  std::vector<std::string> astring = {"Blue", "Red", "Orange", "Yellow"}; // Only vector is supported 03/2021
  result = data.setValue(astring);
  createDataContainer(result, "array-of-string", data);

  unsigned char bytes[] = {0x43, 0x4d, 0x30, 0x30, 0x0f, 0x0D};
  result = data.setValue(bytes);
  createDataContainer(result, "array-of-bytes", data);

  // Register the FlatBuffers type
  std::string typeAddress = "types/sampleSchema/inertialValue";
  std::string bfbs = isSnap() ? std::string(snapPath()).append("/sampleSchema.bfbs") : "bfbs/sampleSchema.bfbs";
  result = m_provider->registerType(typeAddress, bfbs);
  if (result != comm::datalayer::DlResult::DL_OK)
  {
    std::cout << "ERROR registerType(): " << typeAddress << " code: 0x" << std::hex << result.value << std::endl;
    return;
  }

  // https://chromium.googlesource.com/external/github.com/google/flatbuffers/+/refs/tags/v1.5.0/docs/source/CppUsage.md
  // https://github.com/google/flatbuffers/blob/master/samples/sample_binary.cpp
  flatbuffers::FlatBufferBuilder builder;
  short x = 123;
  short y = 234;
  short z = 345;
  auto inertialValue = sample::schema::CreateInertialValue(builder, x, y, z);
  builder.Finish(inertialValue);
  result = data.shareFlatbuffers(builder);

  createDataContainer(result, "fbs", data);
}

// This function will be called whenever a object should be created.
void ProviderNodeAllData::onCreate(
  const std::string& address,
  const comm::datalayer::Variant* data,
  const comm::datalayer::IProviderNode::ResponseCallback& callback)
{
  comm::datalayer::DlResult result;

  if (m_dynamic == false)
  {
    result = comm::datalayer::DlResult::DL_PERMISSION_DENIED;
    std::cout << "onCreate(): " << address << " " << result.toString() << std::endl;
    callback(result, nullptr);
    return;
  }

  static comm::datalayer::Variant empty;
  comm::datalayer::Variant myData = data != nullptr ? *data : empty; // Avoid nullptr assignment

  result = createDataContainer(address, myData);
  std::cout << "onCreate(): " << address << " " << result.toString() << std::endl;

  callback(result, nullptr);
}

// Read function of a node. Function will be called whenever a node should be read.
void ProviderNodeAllData::onRead(
  const std::string& address,
  const comm::datalayer::Variant* data,
  const comm::datalayer::IProviderNode::ResponseCallback& callback)
{
  comm::datalayer::DlResult result;

  auto dataContainer = getDataContainer(address);
  if (nullptr == dataContainer)
  {
    result = comm::datalayer::DlResult::DL_INVALID_ADDRESS;
    //std::cout << "onRead(): " << address << " " << result.toString() << std::endl;
    callback(result, nullptr);
    return;
  }

  comm::datalayer::Variant myData = dataContainer->m_data;
  result = dataContainer->m_errorCode >= 0 ? comm::datalayer::DlResult::DL_OK : comm::datalayer::DlResult::DL_INVALID_VALUE;
  //std::cout << "onRead(): " << address << " " << result.toString() << std::endl;

  callback(result, &myData);
}

// Write function of a node. Function will be called whenever a node should be written.
void ProviderNodeAllData::onWrite(
  const std::string& address,
  const comm::datalayer::Variant* data,
  const comm::datalayer::IProviderNode::ResponseCallback& callback)
{
  comm::datalayer::DlResult result;

  if (m_dynamic == false)
  {
    result = comm::datalayer::DlResult::DL_PERMISSION_DENIED;
    std::cout << "onWrite(): " << address << " ";
    std::cout << data;
    std::cout << " " << result.toString() << std::endl;
    callback(result, nullptr);
    return;
  }

  DataContainer* dataContainer = getDataContainer(address);
  if (nullptr == dataContainer)
  {
    // A virtual node which is not registered should be written.
    // We a creating and registering the virtual node now. This makes ctrlX Data Layer usage easy for a client.
    result = createDataContainer(address, *data);
    std::cout << "onWrite() CREATE: " << address << " " << result.toString() << std::endl;
  }
  else
  {
    // The virtual node is already registered.
    result = comm::datalayer::DlResult::DL_OK;
    // We are take the data without data type check.
    dataContainer->m_data = *data;
    // No trace to improve performance
    // std::cout << "onWrite(): " << address << " OK" << std::endl;
  }

  callback(result, data);
}

// Remove function for an object. Function will be called whenever a object should be removed.
void ProviderNodeAllData::onRemove(
  const std::string& address,
  const comm::datalayer::IProviderNode::ResponseCallback& callback)
{
  comm::datalayer::DlResult result;

  if (m_dynamic == false)
  {
    result = comm::datalayer::DlResult::DL_PERMISSION_DENIED;
    std::cout << "onRemove(): " << address << " " << result.toString() << std::endl;
    callback(result, nullptr);
    return;
  }

  DataContainer* dataContainer = getDataContainer(address);
  if (nullptr != dataContainer)
  {
    m_provider->unregisterNode(dataContainer->m_address);
    std::remove(m_dataContainers.begin(), m_dataContainers.end(), dataContainer);
    delete dataContainer;
  }

  result = comm::datalayer::DlResult::DL_OK;
  std::cout << "onRemove(): " << address << " " << result.toString() << std::endl;
  callback(result, nullptr);
}

// Browse function of a node. Function will be called to determine children of a node.
void ProviderNodeAllData::onBrowse(
  const std::string& address,
  const comm::datalayer::IProviderNode::ResponseCallback& callback)
{
  // No trace to improve performance
  //std::cout << "onBrowse(): " << address << std::endl;

  callback(comm::datalayer::DlResult::DL_OK, nullptr); // ctrlX Data Layer Broker "knows" all sub node, we don't add further nodes here.
}

// Read function of metadata of an object. Function will be called whenever the metadata of a node should be written.
void ProviderNodeAllData::onMetadata(
  const std::string& address,
  const comm::datalayer::IProviderNode::ResponseCallback& callback)
{
  comm::datalayer::DlResult result;
  DataContainer* dataContainer = getDataContainer(address); // For .../dynamic or .../static we get also nullptr
  if (nullptr == dataContainer)
  {
    if (address + "/" == m_addressBase)
    {
      callback(comm::datalayer::DlResult::DL_OK, &m_metadata);
      return;
    }
    result = comm::datalayer::DlResult::DL_INVALID_ADDRESS;
    std::cout << "onMetadata(): " << address << " " << result.toString() << std::endl;
    callback(result, nullptr);
    return;
  }
  callback(comm::datalayer::DlResult::DL_OK, &dataContainer->m_metadata);
}
