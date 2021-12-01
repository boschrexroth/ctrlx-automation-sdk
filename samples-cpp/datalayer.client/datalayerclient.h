#include <stdio.h>
#include <iostream>


#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/metadata_generated.h"

//#include "sampleSchema_generated.h"

class DataLayerClient
{
private:
  comm::datalayer::IClient *_client;
  comm::datalayer::DlResult _result;
  comm::datalayer::DlResult _resultAsync;
  comm::datalayer::Variant _data;
  comm::datalayer::Variant _dataAsync;

  comm::datalayer::DatalayerSystem _datalayer;

  std::string _connectionClient = "tcp://boschrexroth:boschrexroth@127.0.0.1:2069";

  comm::datalayer::IClient::ResponseCallback responseCallback();

  void println(std::string text);
  void println(std::string text, comm::datalayer::DlResult result);
  void println(std::string text, comm::datalayer::DlResult result, comm::datalayer::Variant *data);

  bool waitForResponseCallback(int counter);
  void readSync(const std::string node);
  void writeSync(const std::string node);
  void createSync(const std::string node);

  void start();
  void ping();
  void read();

  void create();

  void remove();

  void browse();

  void write();

  void metadata();
  void stop();

public:
  void Run();
};
