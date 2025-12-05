#include <iostream>
#include <thread>
#include <chrono>
#include <source_location>

#include <comm/datalayer/datalayer.h>
#include <comm/datalayer/datalayer_system.h>
#include <common/scheduler/scheduler_remote_system.h>
#include "Logger.h"

#include "app_client.h"

// Constructor definition for AppClient
AppClient::AppClient(const std::string &ipcPath) 
  : m_dl(nullptr)
  , m_sched(nullptr)
{
  //std::cout << std::source_location::current().function_name() << std::endl;

  // Create an instance of the DataLayer System and start the DL communication
  TRACE_INFO(TRACE_UNIT_ID, "Initializing DataLayer");
  m_dl = std::make_shared<comm::datalayer::DatalayerSystem>(ipcPath);
  m_dl->start(false);

  if (!waitonframeworkstarted(m_dl, ipcPath))
  {
    registrationfailure(common::scheduler::SchedStatus::SCHED_E_NOT_INITIALIZED);
    return;
  }

  //std::cout << std::source_location::current().function_name() << " after wait" << std::endl;

  TRACE_INFO(TRACE_UNIT_ID, "Initializing Scheduler");
  m_sched = std::make_shared<common::scheduler::SchedulerRemoteSystem>(m_dl->factory5());

  TRACE_INFO(TRACE_UNIT_ID, "Initializing Scheduler Factory");
  m_factory = std::make_shared<MyFactory>();

  // Attempt registration of scheduler factory upto 5 times until the AutomationCore is available
  TRACE_INFO(TRACE_UNIT_ID, "Registering Scheduler Factory");
  for (int attempt = 1; attempt <= 5; ++attempt)
  {
    auto result = m_sched->registerCallableFactory(std::shared_ptr<common::scheduler::ICallableFactory2>(m_factory), "remote-factory");

    if (result == common::scheduler::SchedStatus::SCHED_S_OK)
    {
      TRACE_INFO(TRACE_UNIT_ID, "Registration Successful");
      break;
    }
    if (attempt < 5)
    {
      TRACE_INFO(TRACE_UNIT_ID, "Registration failed, waiting for AutomationCore to be available...");
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    else
    {
      registrationfailure(result);
    }
  }
}

// Destructor definition for AppClient
AppClient::~AppClient()
{
  //std::cout << std::source_location::current().function_name() << std::endl;

  TRACE_INFO(TRACE_UNIT_ID, "Destructor of virtual AppClient::~AppClient() is being executed.");
  m_sched->unregisterCallableFactory(m_factory, true);
  m_factory.reset();
  m_sched.reset();
  m_sched = nullptr;
  m_dl.reset();
  m_dl = nullptr;
}

// Function to handle registration failure
void AppClient::registrationfailure(common::scheduler::SchedStatus status)
{
  //std::cout << std::source_location::current().function_name() << std::endl;

  TRACE_INFO(TRACE_UNIT_ID, "Failed to register Factory with status code: %s", common::scheduler::getSchedStatusAsString(status));
  TRACE_INFO(TRACE_UNIT_ID, "Exiting with a non-zero status code to trigger an automatic restart");
  std::exit(EXIT_FAILURE);
}

bool AppClient::waitonframeworkstarted(std::shared_ptr<comm::datalayer::DatalayerSystem>& dl, const std::string &ipcPath)
{
  //std::cout << std::source_location::current().function_name() << std::endl;

  std::shared_ptr<comm::datalayer::IClient> client;
  client.reset(dl->factory()->createClient(DL_IPC));

  for (int i = 0; i < 10; i++)
  {
    if (!client->isConnected())
    {
      //std::cout << std::source_location::current().function_name() << " Client: is not connected" << std::endl;

      TRACE_INFO(TRACE_UNIT_ID, "Client: is not connected");
      std::this_thread::sleep_for(std::chrono::seconds(2));
      continue;
    }

    comm::datalayer::Variant value;
    auto result = client->readSync("framework/events/framework-started", &value);
    if (result != DL_OK)
    {
      //std::cout << std::source_location::current().function_name() << " Client: 'framework/events/framework-started' error: " << result.toString() << std::endl;
      TRACE_INFO(TRACE_UNIT_ID, "Client: 'framework/events/framework-started' error: ", result);
      std::this_thread::sleep_for(std::chrono::seconds(2));
      continue;
    }

    if ((uint32_t)value == 1)
    {
      return true;
    }

    //std::cout << std::source_location::current().function_name() << " Client: framework is not started " << (uint32_t)value << std::endl;
    TRACE_INFO(TRACE_UNIT_ID, "Client: framework is not started");
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
  return false;
}