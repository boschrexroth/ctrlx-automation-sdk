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

#include <functional>
#include <thread>
#include <limits>

#include "ctrlx_datalayer_helper.h"
#include "datalayerclient.h"

// Function to print out array of all nodes from VariantType on ctrlX Data Layer
void printStringList(comm::datalayer::Variant &data)
{
  if (data.getType() == comm::datalayer::VariantType::ARRAY_OF_STRING)
  {
    std::cout << "Node List: ";
    const char **strArray = data;
    for (uint32_t i = 0; i < data.getCount(); i++)
    {
      std::cout << strArray[i] << " ";
    }
    std::cout << std::endl;
  }
}

void printMetadata(comm::datalayer::Variant &data)
{
  if (STATUS_FAILED(data.verifyFlatbuffers(comm::datalayer::VerifyMetadataBuffer)))
  {
    std::cout << "Invalid Flatbuffer: Not a MetadataBuffer";
    return;
  }

  auto metadata = comm::datalayer::GetMetadata(data.getData());
  std::cout << metadata->displayName()->c_str() << std::endl;
  std::cout << metadata->displayFormat() << std::endl;
  std::cout << metadata->description()->c_str() << std::endl;
  std::cout << metadata->descriptionUrl()->c_str() << std::endl;

  auto operations = metadata->operations();
  std::cout << "AllowedOperations: read=" << operations->read() << " write:" << operations->write() << " create:" << operations->create() << " delete:" << operations->delete_() << std::endl;

  auto references = metadata->references();
  for (flatbuffers::uoffset_t i = 0; i < references->size(); i++)
  {
    std::cout << references->Get(i)->type()->c_str() << std::endl;
    std::cout << references->Get(i)->targetAddress()->c_str() << std::endl;
  }
}

DataLayerClient::DataLayerClient(const std::string &ip, const std::string &user, const std::string &password, int sslPort)
{
  _ip = ip;
  _user = user;
  _password = password;
  _sslPort = sslPort;
}

comm::datalayer::DlResult DataLayerClient::print(comm::datalayer::Variant *data)
{
  std::string valueText;
  comm::datalayer::DlResult result = comm::datalayer::DlResult::DL_OK;
  auto variantType = data->getType();

  int16_t x;
  int16_t y;
  int16_t z;

  if (variantType == comm::datalayer::VariantType::FLATBUFFERS)
  {
    result = data->verifyFlatbuffers(sample::schema::VerifyInertialValueBuffer);
    if (comm::datalayer::STATUS_FAILED(result))
    {
      valueText = "Node is no InertialValue flatbuffers";
      return result;
    }

    // https://google.github.io/flatbuffers/md__cpp_usage.html
    auto buffer = data->getData();
    auto inertialValue = sample::schema::GetInertialValue(buffer);

    x = inertialValue->x();
    y = inertialValue->y();
    z = inertialValue->z();

    valueText = "x=" + std::to_string(x) + " y=" + std::to_string(y) + " z=" + std::to_string(z);
    std::cout << valueText;
    return comm::datalayer::DlResult::DL_OK;
  }

  // ---------------------------------------------------------------------------
  // IMPORTANT: Value must be CONST POINTER
  // ---------------------------------------------------------------------------

  if (variantType == comm::datalayer::VariantType::ARRAY_OF_BOOL8)
  {
    const bool *values = *data;
    for (uint32_t i = 0; i < data->getCount(); i++)
    {
      std::cout << values[i];
      std::cout << " ";
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::ARRAY_OF_FLOAT32)
  {
    const float *values = *data;
    for (uint32_t i = 0; i < data->getCount(); i++)
    {
      std::cout << values[i];
      std::cout << " ";
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::ARRAY_OF_FLOAT64)
  {
    const double *values = *data;
    for (uint32_t i = 0; i < data->getCount(); i++)
    {
      std::cout << values[i];
      std::cout << " ";
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::ARRAY_OF_INT16)
  {
    const int16_t *values = *data;
    for (uint32_t i = 0; i < data->getCount(); i++)
    {
      std::cout << values[i];
      std::cout << " ";
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::ARRAY_OF_INT32)
  {
    const int32_t *values = *data;
    for (uint32_t i = 0; i < data->getCount(); i++)
    {
      std::cout << values[i];
      std::cout << " ";
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::ARRAY_OF_INT64)
  {
    const int64_t *values = *data;
    for (uint32_t i = 0; i < data->getCount(); i++)
    {
      std::cout << values[i];
      std::cout << " ";
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::ARRAY_OF_INT8)
  {
    const int8_t *values = *data;
    for (uint32_t i = 0; i < data->getCount(); i++)
    {
      std::cout << std::to_string(values[i]);
      std::cout << " ";
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::ARRAY_OF_STRING)
  {
    const char **values = *data;
    for (uint32_t i = 0; i < data->getCount(); i++)
    {
      std::cout << values[i] << " ";
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::ARRAY_OF_UINT16)
  {
    const uint16_t *values = *data;
    for (uint32_t i = 0; i < data->getCount(); i++)
    {
      std::cout << values[i];
      std::cout << " ";
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::ARRAY_OF_UINT32)
  {
    const uint32_t *values = *data;
    for (uint32_t i = 0; i < data->getCount(); i++)
    {
      std::cout << values[i];
      std::cout << " ";
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::ARRAY_OF_UINT64)
  {
    const uint64_t *values = *data;
    for (uint32_t i = 0; i < data->getCount(); i++)
    {
      std::cout << values[i];
      std::cout << " ";
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::ARRAY_OF_UINT8)
  {
    const uint8_t *values = *data;
    for (uint32_t i = 0; i < data->getCount(); i++)
    {
      std::cout << std::to_string(values[i]);
      std::cout << " ";
    }
    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::BOOL8)
  {
    auto value = bool(*data);
    std::cout << value;

    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::FLOAT32)
  {
    auto value = float(*data);
    std::cout << value;

    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::FLOAT64)
  {
    auto value = double(*data);
    std::cout << value;

    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::INT16)
  {
    auto value = int16_t(*data);
    std::cout << value;

    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::INT32)
  {
    auto value = int32_t(*data);
    std::cout << value;

    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::INT64)
  {
    auto value = int64_t(*data);
    std::cout << value;

    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::INT8)
  {
    auto value = int8_t(*data);
    std::cout << std::to_string(value);

    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::STRING)
  {
    const char *value = *data;
    std::cout << value;

    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::UINT16)
  {
    auto value = uint16_t(*data);
    std::cout << value;

    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::UINT32)
  {
    auto value = uint32_t(*data);
    std::cout << value;

    return comm::datalayer::DlResult::DL_OK;
  }

  if (variantType == comm::datalayer::VariantType::UINT64)
  {
    auto value = uint64_t(*data);
    std::cout << value;

    return comm::datalayer::DlResult::DL_OK;
  }

  std::cout << "Unsupported VariantType";
  return comm::datalayer::DlResult::DL_UNSUPPORTED;
}

void DataLayerClient::println(const std::string &text, comm::datalayer::DlResult result, comm::datalayer::Variant *data)
{
  std::cout << text;
  std::cout << " " << result.toString() << " ";

  result = print(data);

  std::cout << " " << result.toString() << std::endl;
}

bool DataLayerClient::start()
{
  std::cout << "_datalayer.start(..)" << std::endl;
  _datalayerSystem.start(false);

  _client = getClient(_datalayerSystem, _ip, _user, _password, _sslPort);

  return _client != nullptr && _client->isConnected();
}

// This method returns a comm::datalayer::IClient::ResponseCallback function as lambda expression.
// See:
// https://en.cppreference.com/w/cpp/language/lambda
// https://de.cppreference.com/w/cpp/language/lambda
comm::datalayer::IClient::ResponseCallback DataLayerClient::responseCallback()
{
  // [&]    All needed symbols are provided per reference
  // (...)  Parameter provided by the calling site
  return [&](comm::datalayer::DlResult result, const comm::datalayer::Variant *data)
  {
    _resultAsync = result;
    if (data != nullptr)
    {
      // data must not be nullptr otherwise the '=' operator crashes
      _dataAsync = *data;

      if (_dataAsync.getType() == comm::datalayer::VariantType::ARRAY_OF_STRING)
      {
        printStringList(_dataAsync);
      }
    }

    std::cout << "ResponseCallback: " << std::string(result.toString()) << std::endl;
  };
}

bool DataLayerClient::waitForResponseCallback(int counter)
{
  for (;;)
  {
    if (counter > 0)
    {
      counter--;
      if (counter <= 0)
        return false;
    }

    sleep(1);

    if (_resultAsync != -1)
    {
      std::cout << "ResponseCallback finished: " << _resultAsync.toString() << std::endl;
      return true;
    }
  }
}

void DataLayerClient::ping()
{
  _result = _client->pingSync();
  std::cout << "_client->pingSync() " << _result.toString() << std::endl;

  _resultAsync = -1;
  _result = _client->pingAsync(responseCallback());
  waitForResponseCallback(10);
}

void DataLayerClient::readSync(const std::string node)
{
  std::string address = "sdk-cpp-alldata/static/" + node;
  _result = _client->readSync(address, &_data);
  println("readSync() " + address, _result, &_data);
}

void DataLayerClient::read()
{

  readSync("bool8");

  std::string address = "sdk-cpp-alldata/static/bool8";

  _result = _client->readAsync(address, _data, responseCallback());

  std::cout << "readAsync()" << _result.toString() << std::endl;

  waitForResponseCallback(10);

  readSync("float32");

  readSync("float64");

  readSync("int8");

  readSync("int16");

  readSync("int32");

  readSync("int64");

  readSync("string");

  readSync("uint8");

  readSync("uint16");

  readSync("uint32");

  readSync("uint64");

  readSync("fbs");

  readSync("array-of-bool8");

  readSync("array-of-float32");

  readSync("array-of-float64");

  readSync("array-of-int8");

  readSync("array-of-int16");

  readSync("array-of-int32");

  readSync("array-of-int64");

  readSync("array-of-string");

  readSync("array-of-uint8");

  readSync("array-of-uint16");

  readSync("array-of-uint32");

  readSync("array-of-uint64");
}

void DataLayerClient::createSync(const std::string node)
{
  std::string address = "/sdk-cpp-alldata/dynamic/_cpp/" + node;
  _client->removeSync(address); // First remove - may be node exists
  _result = _client->createSync(address, &_data);
}

void DataLayerClient::create()
{
  _data.setValue(false);
  createSync("bool8");

  _data.setValue(-0.123456789f);
  createSync("float32");

  _data.setValue(-0.987654321);
  createSync("float64");

  _data.setValue((int8_t)-127);
  createSync("int8");

  _data.setValue((int16_t)-32767);
  createSync("int16");

  _data.setValue((int32_t)0x80000001);
  createSync("int32");

  _data.setValue((int64_t)0x8000000000000001);
  createSync("int64");

  _data.setValue("Changed by cpp Data Layer Client");
  createSync("string");

  // Flatbuffers

  bool arrBool[] = {false, true, false};
  _data.setValue(arrBool);
  createSync("array-of-bool8");

  int8_t arrInt8[] = {-127, -1, 0, 127};
  _data.setValue(arrBool);
  createSync("array-of-int8");

  uint8_t arrUint8[] = {0, 127, 128, 255};
  _data.setValue(arrUint8);
  createSync("array-of-uint8");

  int16_t arrInt16[] = {-32767, -1, 0, 32767};
  _data.setValue(arrInt16);
  createSync("array-of-int16");

  uint16_t arrUint16[] = {0, 32767, 32768, 65535};
  _data.setValue(arrUint16);
  createSync("array-of-uint16");

  int32_t arrInt32[] = {-2147483647, -1, 0, 2147483647};
  _data.setValue(arrInt32);
  createSync("array-of-int32");

  uint32_t arrUint32[] = {0, 2147483647, 2147483648, 4294967295};
  _data.setValue(arrUint32);
  createSync("array-of-uint32");

  int64_t arrInt64[] = {-9223372036854775807, -1, 0, 9223372036854775807};
  _data.setValue(arrInt64);
  createSync("array-of-int64");

  // https://en.cppreference.com/w/cpp/types/numeric_limits/max
  uint64_t arrUint64[] = {0, 9223372036854775807, std::numeric_limits<std::size_t>::max() - 1, std::numeric_limits<std::size_t>::max()};
  _data.setValue(arrUint64);
  createSync("array-of-uint64");

  float_t arrFloat32[] = {32.1, 32.2, 32.3, 32.4};
  _data.setValue(arrFloat32);
  createSync("array-of-float32");

  double_t arrFloat64[] = {64.1, 64.2, 64.3, 64.4};
  _data.setValue(arrFloat64);
  createSync("array-of-float64");

  std::string arrString[] = {"Red", "Green", "Yellow", "Blue"};
  _data.setValue(arrString);
  createSync("array-of-string");
}

void DataLayerClient::remove()
{
  _data.setValue("Will be removed");
  createSync("xxx");

  std::string address = "/sdk-cpp-alldata/dynamic/_cpp/xxx";
  _result = _client->removeSync(address);

  createSync("xxx");
  _result = _client->removeAsync(address, responseCallback());
  waitForResponseCallback(10);
}

void DataLayerClient::browse()
{
  _data.setValue("");
  _result = _client->browseSync("", &_data);
  if (_result == comm::datalayer::DlResult::DL_OK)
  {
    printStringList(_data);
  }

  _result = _client->browseAsync("", responseCallback());
  waitForResponseCallback(10);
}

void DataLayerClient::writeSync(const std::string node)
{
  std::string address = "sdk-cpp-alldata/dynamic/" + node;

  _result = _client->writeSync(address, &_data);
  println("writeSync() " + address, _result, &_data);
}

void DataLayerClient::write()
{
  _data.setValue(false);
  writeSync("bool8");

  _data.setValue(-0.123456789f);
  writeSync("float32");

  _data.setValue(-0.987654321);
  writeSync("float64");

  _data.setValue((int8_t)-127);
  writeSync("int8");

  _data.setValue((int16_t)-32767);
  writeSync("int16");

  _data.setValue((int32_t)0x80000001);
  writeSync("int32");

  _data.setValue((int64_t)0x8000000000000001);
  writeSync("int64");

  _data.setValue("Changed by cpp Data Layer Client");
  writeSync("string");
}

void DataLayerClient::metadata()
{
  std::string address = "scheduler/admin/state";
  _result = _client->metadataSync(address, &_data);

  printMetadata(_data);
}

void DataLayerClient::stop()
{
  delete _client;
}

void DataLayerClient::Run()
{
  std::cout << "Simple Snap for ctrlX Datalayer Client in cpp" << std::endl;
  std::cout << "--------------------------------------------" << std::endl;

  if (start())
  {

    ping();
    read();
    create();
    remove();
    browse();
    write();
    metadata();
  }
  stop();
}
