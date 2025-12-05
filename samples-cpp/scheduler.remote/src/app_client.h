#pragma once
#include <common/scheduler/i_scheduler4.h>
#include <comm/datalayer/datalayer_system.h>
#include "factory.h"

class AppClient
{
public:
  AppClient(const std::string& ipcPath);
  virtual ~AppClient();

protected:
private:
  void browse();
  void registrationfailure(common::scheduler::SchedStatus status);

  bool waitonframeworkstarted(std::shared_ptr<comm::datalayer::DatalayerSystem>& dl, const std::string &ipcPath);

  std::shared_ptr<comm::datalayer::DatalayerSystem> m_dl;
  std::shared_ptr<common::scheduler::IScheduler3> m_sched;
  std::shared_ptr<MyFactory> m_factory;
};
