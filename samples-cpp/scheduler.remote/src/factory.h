#pragma once
#include "common/scheduler/i_scheduler4.h"
#include "common/scheduler/callable_configurations_generated.h"
#include <map>
#include <iostream>
#include "Logger.h"

class MyFactory
  : public common::scheduler::ICallableFactory2
  , public common::scheduler::ICallable
  , public std::enable_shared_from_this<common::scheduler::ICallable>
{
public:
  MyFactory() = default;
  virtual ~MyFactory() = default;

  std::map< common::scheduler::SchedEventType, size_t> m_eventCounts;
protected:
private:

  void getFactoryDescription(std::vector<std::string>& description) const override
  {
    description.push_back("remote");
    description.push_back("factory");
  }

  void getCallableConfigurations(comm::datalayer::Variant& configurations) const override
  {
    flatbuffers::FlatBufferBuilder builder;
    auto config = common::scheduler::fbs::CreateCallableConfigurationDirect(builder, nullptr, 0, nullptr, common::scheduler::fbs::CallableWdgConfig_WDG_NONE, 0);
    auto configs = std::vector<flatbuffers::Offset<common::scheduler::fbs::CallableConfiguration>>({ config });
    builder.Finish(common::scheduler::fbs::CreateCallableConfigurationsDirect(builder, &configs));
    configurations.copyFlatbuffers(builder);
  }

  std::shared_ptr<common::scheduler::ICallable> createCallable(const comm::datalayer::Variant& param) override
  {
    return shared_from_this();
  }

  common::scheduler::SchedStatus destroyCallable(const std::shared_ptr<common::scheduler::ICallable>& callable) override
  {
    return common::scheduler::SchedStatus::SCHED_S_OK;
  }

  common::scheduler::SchedStatus getCallableArguments(std::vector<std::string>& arguments) override
  {
    return common::scheduler::SchedStatus::SCHED_S_OK;
  }

  // Process incoming scheduler events, log changes and count occurrences
  common::scheduler::SchedEventResponse execute(const common::scheduler::SchedEventType& eventType, const common::scheduler::SchedEventPhase& eventPhase, comm::datalayer::Variant& param) override
  {
    if (lastEventType != eventType || lastEventPhase != eventPhase)
    {
      lastEventType = eventType;
      lastEventPhase = eventPhase;
      
      // Please note that the usage of traces here is only a placeholder for demonstration and should be removed for real-time workloads.
      TRACE_INFO(TRACE_UNIT_ID, "%s %s",
                 common::scheduler::getSchedEventTypeAsString(eventType).c_str(),
                 common::scheduler::getSchedEventPhaseAsString(eventPhase).c_str());
    }
    m_eventCounts[eventType]++;
    return common::scheduler::SchedEventResponse::SCHED_EVENT_RESP_OKAY;
  }

  common::scheduler::SchedEventType lastEventType = common::scheduler::SchedEventType::SCHED_EVENT_TICK;
  common::scheduler::SchedEventPhase lastEventPhase = common::scheduler::SchedEventPhase::SCHED_EVENT_PHASE_NONE;
};