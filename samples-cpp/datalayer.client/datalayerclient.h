/**
 * MIT License
 *
 * Copyright (c) 2022 Bosch Rexroth AG
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

#include <stdio.h>
#include <iostream>


#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/metadata_generated.h"

#include "sampleSchema_generated.h"

class DataLayerClient
{
private:
  std::string _ip;
  std::string _user;
  std::string _password;
  int _sslPort;
  comm::datalayer::IClient* _client;
  comm::datalayer::DlResult _result;
  comm::datalayer::DlResult _resultAsync;
  comm::datalayer::Variant _data;
  comm::datalayer::Variant _dataAsync;

  comm::datalayer::DatalayerSystem _datalayerSystem;

  comm::datalayer::IClient::ResponseCallback responseCallback();

  void println(const std::string& text, comm::datalayer::DlResult result, comm::datalayer::Variant* data);
  comm::datalayer::DlResult print(comm::datalayer::Variant* data);

  bool waitForResponseCallback(int counter);
  void readSync(const std::string& node);
  void writeSync(const std::string& node);
  void createSync(const std::string& node);

  bool start();
  void ping();
  void read();

  void create();

  void remove();

  void browse();

  void write();

  void metadata();
  void stop();

public:
  DataLayerClient(const std::string& ip = "192.168.1.1", const std::string& user = "boschrexroth", const std::string& password = "boschrexroth", int sslPort = 443);
  void Run();
};
