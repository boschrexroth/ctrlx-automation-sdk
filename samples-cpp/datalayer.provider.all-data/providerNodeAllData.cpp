#ifndef PROVIDE_NODES_H
#define PROVIDE_NODES_H

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

#include <string>

#include "providerNodeAllData.h"
#include "sampleSchema_generated.h"

DataContainer *ProviderNodeAllData::getDataContainer(const std::string &address)
{
  for (auto dataContainer : _dataContainers)
  {
    if (dataContainer->Address == address)
    {
      return dataContainer;
    }
  }
  return NULL;
}

comm::datalayer::DlResult ProviderNodeAllData::createDataContainer(
    const std::string &address,
    const comm::datalayer::Variant &data)
{
  if (getDataContainer(address) != nullptr)
  {
    std::cout << "ERROR - Node exists: " << address;
    return comm::datalayer::DlResult::DL_CREATION_FAILED;
  }

  auto metadata = createMetadata(data, address);
  auto dataContainer = new DataContainer(address, data, metadata);
  
  _dataContainers.emplace_back(dataContainer);

  return _provider->registerNode(address, this);
}

void ProviderNodeAllData::createDataContainer(
    comm::datalayer::DlResult setValueResult,
    const std::string &addressNode,
    const comm::datalayer::Variant &data)
{
  const std::string address = _addressBase + addressNode;
  if (comm::datalayer::DlResult::DL_OK != setValueResult)
  {
    std::cout << "ERROR - Data could not be set: " << address;
    return;
  }
  if (createDataContainer(address, data) != comm::datalayer::DlResult::DL_OK){
     std::cout << "ERROR - DataContainer could not be created: " << address;
  }
}

comm::datalayer::Variant ProviderNodeAllData::createMetadata(const comm::datalayer::Variant &data, const std::string &address)
{
  
  flatbuffers::FlatBufferBuilder builder;
  auto description = builder.CreateString("This is a description for " + address);
  comm::datalayer::NodeClass nodeClass = comm::datalayer::NodeClass_Variable;

  bool isBaseFolder = address == _addressBase;
  if (isBaseFolder)
  {
    nodeClass = comm::datalayer::NodeClass_Folder; 
  }

  comm::datalayer::AllowedOperationsBuilder allowedOperations(builder);
  allowedOperations.add_read(!isBaseFolder);
  allowedOperations.add_write(!isBaseFolder && _dynamic);
  allowedOperations.add_create(!isBaseFolder && _dynamic);
  allowedOperations.add_delete_(!isBaseFolder && _dynamic);
  auto operations = allowedOperations.Finish();


  flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<comm::datalayer::Reference>>> references;
  if (data.getType() == comm::datalayer::VariantType::FLATBUFFERS)
  {
    const char *type_addr = "types/sampleSchema/inertialValue";

    if (_dynamic)
    {
      flatbuffers::Offset<comm::datalayer::Reference> vecReferences[] =
          {
              comm::datalayer::CreateReferenceDirect(builder, "readType", type_addr),
              comm::datalayer::CreateReferenceDirect(builder, "writeType", type_addr),
          };
      references = builder.CreateVectorOfSortedTables(vecReferences, 2);
    }
    else
    {
      flatbuffers::Offset<comm::datalayer::Reference> vecReferences[] =
          {
              comm::datalayer::CreateReferenceDirect(builder, "readType", type_addr),
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

ProviderNodeAllData::ProviderNodeAllData(comm::datalayer::IProvider *provider, const std::string &addressRoot, bool dynamic)
{
  _provider = provider;
  _addressRoot = addressRoot;
  _dynamic = dynamic;

  _addressBase = dynamic ? _addressRoot + "dynamic/" : _addressRoot + "static/";

  const comm::datalayer::Variant data;
  _metadata = createMetadata(data, _addressBase);
}

void ProviderNodeAllData::RegisterNodes()
{
  comm::datalayer::DlResult result;
  result = _provider->registerNode(_addressBase + "**", this);
  
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

  // Arrays ----------------------
  bool a_bool[] = {true, false, true};
  result = data.setValue(a_bool);
  createDataContainer(result, "array-of-bool8", data);

  int8_t a_int8_t[] = {INT8_MIN, -1, 0, INT8_MAX};
  result = data.setValue(a_int8_t);
  createDataContainer(result, "array-of-int8", data);

  uint8_t a_uint8_t[] = {0, UINT8_MAX};
  result = data.setValue(a_uint8_t);
  createDataContainer(result, "array-of-uint8", data);

  int16_t a_int16_t[] = {INT16_MIN, -1, 0, INT16_MAX};
  result = data.setValue(a_int16_t);
  createDataContainer(result, "array-of-int16", data);

  uint16_t a_uint16_t[] = {0, 32767, 32768, UINT16_MAX};
  result = data.setValue(a_uint16_t);
  createDataContainer(result, "array-of-uint16", data);

  int32_t a_int32_t[] = {INT32_MIN, -1, 0, INT32_MAX};
  result = data.setValue(a_int32_t);
  createDataContainer(result, "array-of-int32", data);

  uint32_t a_uint32_t[] = {0, UINT32_MAX};
  result = data.setValue(a_uint32_t);
  createDataContainer(result, "array-of-uint32", data);

  int64_t a_int64_t[] = {INT64_MIN, -1, 0, INT64_MAX};
  result = data.setValue(a_int64_t);
  createDataContainer(result, "array-of-int64", data);

  uint64_t a_uint64_t[] = {0, UINT64_MAX};
  result = data.setValue(a_uint64_t);
  createDataContainer(result, "array-of-uint64", data);

  float a_float[] = {FLT_MIN, -1.0, 0.0, FLT_MAX};
  result = data.setValue(a_float);
  createDataContainer(result, "array-of-float32", data);

  double a_double[] = {DBL_MIN, -1.0, 0.0, DBL_MAX};
  result = data.setValue(a_double);
  createDataContainer(result, "array-of-float64", data);

  // std::string a_string[] = {"Blue", "Red", "Orange", "Yellow"};
  // const char *a_string[] = {"Blue", "Red", "Orange", "Yellow"};
  std::vector<std::string> a_string = {"Blue", "Red", "Orange", "Yellow"}; // Only vector is supported 03/2021
  result = data.setValue(a_string);
  createDataContainer(result, "array-of-string", data);

  unsigned char bytes[] = {0x43, 0x4d, 0x30, 0x30, 0x0f, 0x0D};
  result = data.setValue(bytes);
  createDataContainer(result, "array-of-bytes", data);

  // Register the FlatBuffers type
  std::string typeAddress = "types/sampleSchema/inertialValue";
  std::string bfbs = is_snap() ? std::string(snap_path()).append("/sampleSchema.bfbs") : "bfbs/sampleSchema.bfbs";
  result = _provider->registerType(typeAddress, bfbs);
  if (result != comm::datalayer::DlResult::DL_OK)
  {
    std::cout << "ERROR registerType(): " << typeAddress << " code: 0x" << std::hex << result.value << std::endl;
    return;
  }

  // https://chromium.googlesource.com/external/github.com/google/flatbuffers/+/refs/tags/v1.5.0/docs/source/CppUsage.md
  // https://github.com/google/flatbuffers/blob/master/samples/sample_binary.cpp
  flatbuffers::FlatBufferBuilder builder;
  short x = 1;
  short y = 2;
  short z = 3;
  auto inertialValue = sample::schema::CreateInertialValue(builder, x, y, z);
  builder.Finish(inertialValue);
  result = data.shareFlatbuffers(builder);

  createDataContainer(result, "fbs", data);
}

// This function will be called whenever a object should be created.
void ProviderNodeAllData::onCreate(
    const std::string &address,
    const comm::datalayer::Variant *data,
    const comm::datalayer::IProviderNode::ResponseCallback &callback)
{
  comm::datalayer::DlResult result;

  if (_dynamic == false)
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
    const std::string &address,
    const comm::datalayer::Variant *data,
    const comm::datalayer::IProviderNode::ResponseCallback &callback)
{
  comm::datalayer::DlResult result;

  auto dataContainer = getDataContainer(address);
  if (nullptr == dataContainer)
  {
    result = comm::datalayer::DlResult::DL_INVALID_ADDRESS;
    std::cout << "onRead(): " << address << " " << result.toString() << std::endl;
    callback(result, nullptr);
    return;
  }

  comm::datalayer::Variant myData = dataContainer->Data;
  result = dataContainer->ErrorCode >= 0 ? comm::datalayer::DlResult::DL_OK : comm::datalayer::DlResult::DL_INVALID_VALUE;
  std::cout << "onRead(): " << address << " " << result.toString() << std::endl;

  callback(result, &myData);
}

// Write function of a node. Function will be called whenever a node should be written.
void ProviderNodeAllData::onWrite(
    const std::string &address,
    const comm::datalayer::Variant *data,
    const comm::datalayer::IProviderNode::ResponseCallback &callback)
{
  comm::datalayer::DlResult result;

  if (_dynamic == false)
  {
    result = comm::datalayer::DlResult::DL_PERMISSION_DENIED;
    std::cout << "onWrite(): " << address << " ";
    std::cout << data;
    std::cout << " " << result.toString() << std::endl;
    callback(result, nullptr);
    return;
  }

  DataContainer *dataContainer = getDataContainer(address);
  if (nullptr == dataContainer)
  {
    result = comm::datalayer::DlResult::DL_INVALID_ADDRESS;
    std::cout << "onWrite(): " << address << " ";
    std::cout << data;
    std::cout << " " << result.toString() << std::endl;
    callback(result, nullptr);
    return;
  }

  if (data->getType() != dataContainer->Data.getType())
  {
    result = comm::datalayer::DlResult::DL_TYPE_MISMATCH;
    std::cout << "onWrite(): " << address << " ";
    std::cout << data;
    std::cout << " " << result.toString() << std::endl;
    callback(result, nullptr);
    return;
  }

  result = comm::datalayer::DlResult::DL_OK;
  dataContainer->Data = *data;
  std::cout << "onWrite(): " << address << " ";
  std::cout << data;
  std::cout << " " << result.toString() << std::endl;
  callback(result, data);
}

// Remove function for an object. Function will be called whenever a object should be removed.
void ProviderNodeAllData::onRemove(
    const std::string &address,
    const comm::datalayer::IProviderNode::ResponseCallback &callback)
{
  comm::datalayer::DlResult result;

  if (_dynamic == false)
  {
    result = comm::datalayer::DlResult::DL_PERMISSION_DENIED;
    std::cout << "onRemove(): " << address << " " << result.toString() << std::endl;
    callback(result, nullptr);
    return;
  }

  DataContainer *dataContainer = getDataContainer(address);
  if (nullptr == dataContainer)
  {
    result = comm::datalayer::DlResult::DL_INVALID_ADDRESS;
    std::cout << "onRemove(): " << address << " " << result.toString() << std::endl;
    callback(result, nullptr);
    return;
  }

  _provider->unregisterNode(dataContainer->Address);
  remove(_dataContainers.begin(), _dataContainers.end(), dataContainer);

  delete dataContainer;

  result = comm::datalayer::DlResult::DL_OK;
  std::cout << "onRemove(): " << address << " " << result.toString() << std::endl;
  callback(result, nullptr);
}

// Browse function of a node. Function will be called to determine children of a node.
void ProviderNodeAllData::onBrowse(
    const std::string &address,
    const comm::datalayer::IProviderNode::ResponseCallback &callback)
{
  std::cout << "onBrowse(): " << address << std::endl;

  callback(comm::datalayer::DlResult::DL_OK, nullptr); // Data Layer Broker "knows" all sub node, we don't add further nodes here.
}

// Read function of metadata of an object. Function will be called whenever the metadata of a node should be written.
void ProviderNodeAllData::onMetadata(
    const std::string &address,
    const comm::datalayer::IProviderNode::ResponseCallback &callback)
{
  comm::datalayer::DlResult result;
  DataContainer *dataContainer = getDataContainer(address); // For .../dynamic or .../static we get also nullptr 
  if (nullptr == dataContainer)
  {
    if(address +"/" == _addressBase){
      callback(comm::datalayer::DlResult::DL_OK, &_metadata);
      return;
    }
    result = comm::datalayer::DlResult::DL_INVALID_ADDRESS;
    std::cout << "onMetadata(): " << address << " " << result.toString() << std::endl;
    callback(result, nullptr);
    return;
  }
  callback(comm::datalayer::DlResult::DL_OK, &dataContainer->Metadata);
}
#endif