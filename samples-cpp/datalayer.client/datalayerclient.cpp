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

#include <functional>
#include <thread>
#include <limits>

#include "datalayerclient .h"

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

void DataLayerClient::println(std::string text)
{
  std::cout << text << std::endl;
}

void DataLayerClient::println(std::string text, comm::datalayer::DlResult result)
{
  std::cout << text << result.toString() << std::endl;
}

void DataLayerClient::println(std::string text, comm::datalayer::DlResult result, comm::datalayer::Variant *data)
{
  std::cout << text << " " << result.toString();
  // std::cout << (std::string)*data.getData();
  std::cout << " " << std::endl;
}

void DataLayerClient::start()
{
  std::cout << "_datalayer.start(..)" << std::endl;
  _datalayer.start(false);

  println("_datalayer.factory()->createClient: " + _connectionClient);
  _client = _datalayer.factory()->createClient(_connectionClient);

  // set timeout of all requests to 2 seconds
  _result = _client->setTimeout(comm::datalayer::TimeoutSetting::Ping, 2000);
  println("_client->setTimeout(..) " + std::string(_result.toString()));
}

// This method returns a comm::datalayer::IClient::ResponseCallback function as lambda expression.
// See:
// https://en.cppreference.com/w/cpp/language/lambda
// https://de.cppreference.com/w/cpp/language/lambda
comm::datalayer::IClient::ResponseCallback DataLayerClient::responseCallback()
{
  // [&]    All needed symbols are provided per reference
  // (...)  Parameter provided by the calling site
  return [&](comm::datalayer::DlResult result, const comm::datalayer::Variant *data) {
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
      println("ResponseCallback finished: ", _resultAsync);
      return true;
    }
  }
}

void DataLayerClient::ping()
{
  _result = _client->pingSync();
  println("_client->pingSync() ", _result);

  _resultAsync = -1;
  _result = _client->pingAsync(responseCallback());
  waitForResponseCallback(10);
}

void DataLayerClient::readSync(const std::string node)
{
  std::string address = "all-data/static/" + node;
  _result = _client->readSync(address, &_data);
  println("readSync() " + address, _result, &_data);
}

void DataLayerClient::read()
{

  readSync("bool8");

  std::string address = "all-data/static/bool8";

  _result = _client->readAsync(address, _data, responseCallback());
  println("readAsync()", _result);
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
  std::string address = "/all-data/dynamic/_cpp/" + node;
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

  std::string address = "/all-data/dynamic/_cpp/xxx";
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
  std::string address = "all-data/dynamic/" + node;

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
  std::cout << "Connect to ctrlX CORE: " + _connectionClient << std::endl;
  std::cout << "--------------------------------------------" << std::endl;

  start();

  ping();
  read();
  create();
  remove();
  browse();
  write();
  metadata();

  stop();
}

/*
int main(int ac, char *av[])
{

  // ping Data Layer on ctrlX device
  result = client->pingSync();
  if (STATUS_FAILED(result))
  {
    std::cout << "Ping Failed with: " << result.toString() << std::endl;
    return -1;
  }

  std::cout << "Ping succeeded!" << std::endl;

  // read all nodes of root on ctrlX Data Layer
  result = client->browseSync("", &data);
  if (STATUS_SUCCEEDED(result))
  {
    printStringList(data);
  }

  // Writing new values to 'myData' nodes
  std::cout << std::endl
            << "Writing new values to 'myData' nodes:" << std::endl;

  // set value of myData/myFloat to pi
  data.setValue(3.1415f);
  result = client->writeSync("myData/myFloat", &data);
  if (STATUS_FAILED(result))
  {
    std::cout << "Writing to 'myData/myFloat' failed with: " << result.toString() << std::endl;
  }
  else
  {
    std::cout << "'myData/myFloat' data has expected value" << std::endl;
  }

  // write string into correct data format
  data.setValue((std::string) "samplestring written from datalayerclient");
  result = client->writeSync("myData/myString", &data);
  if (STATUS_FAILED(result))
  {
    std::cout << "Writing to 'myData/myString' failed with: " << result.toString() << std::endl;
  }
  else
  {
    std::cout << "'myData/myString' data has expected value" << std::endl;
  }

  // write string into wrong data format
  result = client->writeSync("myData/myInt64", &data);
  if (STATUS_FAILED(result))
  {
    std::cout << "Expected failure while trying to write string into int64" << std::endl;
    std::cout << "Result of trying to write string into int64: " << result.toString() << std::endl;
  }

  // read myFlatbuffer created with datalayer.register.node example
  // comm::datalayer::Variant myFlatbuffer;
  // result = client->readSync("myData/myFlatbuffer", &myFlatbuffer);
  // if(STATUS_FAILED(result))
  // {
  //   std::cout << "Read flatbuffer failed with: " << result.toString() << std::endl;
  // }
  // else
  // {
  //   std::cout << "Read flatbuffer was successful:" << std::endl;
  // }

  // if (comm::datalayer::VariantType::FLATBUFFERS == myFlatbuffer.getType()) {
  //   std::cout << "  Type is flatbuffer" << std::endl;
  // }

  // auto inertialValue = sample::schema::GetInertialValue(myFlatbuffer.getData());
  // std:: cout << "  Read myFlatbuffer: {\"x\": " << inertialValue->x() << ",\"y\": " << inertialValue->y() << ",\"z\": " << inertialValue->z() << "}" << std::endl;

  // don't forget to delete client if you don't need it anymore
  delete client;
  std::cout << "Client was deleted!" << std::endl;
}
*/