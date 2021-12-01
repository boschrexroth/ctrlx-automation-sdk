#pragma once

#include "common/log/trace/i_log_real_time.h"
#include "common/log/trace/i_registration.h"
#include "common/log/trace/log_buffered.h"

namespace common { namespace log { namespace trace { class IRegistration; } } }
namespace common { namespace log { namespace trace { class IRegistrationRealTime2; } } }
namespace common { namespace log { namespace trace { enum class TraceResult; } } }

#define AUTOMATIONSCHEDULER_API

namespace automation
{
  namespace scheduler
  {
    class BaseTrace;
    class RtTrace;

    AUTOMATIONSCHEDULER_API extern RtTrace g_rt_trace;

    constexpr std::uint32_t AUTOMATION_TRACE_MAIN_DIAGNOSIS_CODE_MESSAGE = (0x0E0A0ABC); //!< Main diagnosis code for trace message of component common.scheduler defined in diagnosis database.
    constexpr std::uint32_t AUTOMATION_TRACE_MAIN_DIAGNOSIS_CODE_WARNING = (0x0E0E0ABC); //!< Main diagnosis code for trace warning of component common.scheduler defined in diagnosis database.
    constexpr std::uint32_t AUTOMATION_TRACE_MAIN_DIAGNOSIS_CODE_ERROR = (0x0E0F0ABC);   //!< Main diagnosis code for trace error of component common.scheduler defined in diagnosis database.

#define SCHED_TRACE_ENTITY "automation.scheduler"

#define TRACE_MSG RT_TRACE_MSG
#define TRACE_WARN RT_TRACE_WARN
#define TRACE_ERR RT_TRACE_ERR

#define NRT_TRACE_MSG(text, ...) { automation::scheduler::g_nrt_trace().log(automation::scheduler::AUTOMATION_TRACE_MAIN_DIAGNOSIS_CODE_MESSAGE, SCHED_TRACE_ENTITY, automation::scheduler::g_nrt_trace.getSnapName(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, text, ##__VA_ARGS__); }
#define NRT_TRACE_WARN(text, ...) { automation::scheduler::g_nrt_trace().log(automation::scheduler::AUTOMATION_TRACE_MAIN_DIAGNOSIS_CODE_WARNING, SCHED_TRACE_ENTITY, automation::scheduler::g_nrt_trace.getSnapName(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, text, ##__VA_ARGS__); }
#define NRT_TRACE_ERR(text, ...) { automation::scheduler::g_nrt_trace().log(automation::scheduler::AUTOMATION_TRACE_MAIN_DIAGNOSIS_CODE_ERROR, SCHED_TRACE_ENTITY, automation::scheduler::g_nrt_trace.getSnapName(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, text, ##__VA_ARGS__); }

#define RT_TRACE_MSG(text, ...) { \
  if (automation::scheduler::g_rt_trace.getTraceEnablingStateMessage()) \
  { \
    ::common::log::trace::LogParametersExtended logParameters{SCHED_TRACE_ENTITY, automation::scheduler::g_rt_trace.getSnapName(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, text}; \
    automation::scheduler::g_rt_trace().logRealTime(automation::scheduler::AUTOMATION_TRACE_MAIN_DIAGNOSIS_CODE_MESSAGE, logParameters, ##__VA_ARGS__); \
  } \
}
#define RT_TRACE_WARN(text, ...) { \
  if (automation::scheduler::g_rt_trace.getTraceEnablingStateWarning()) \
  { \
    ::common::log::trace::LogParametersExtended logParameters{SCHED_TRACE_ENTITY, automation::scheduler::g_rt_trace.getSnapName(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, text}; \
    automation::scheduler::g_rt_trace().logRealTime(automation::scheduler::AUTOMATION_TRACE_MAIN_DIAGNOSIS_CODE_WARNING, logParameters, ##__VA_ARGS__); \
  } \
}
#define RT_TRACE_ERR(text, ...) { \
  if (automation::scheduler::g_rt_trace.getTraceEnablingStateError()) \
  { \
    ::common::log::trace::LogParametersExtended logParameters{SCHED_TRACE_ENTITY, automation::scheduler::g_rt_trace.getSnapName(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, text}; \
    automation::scheduler::g_rt_trace().logRealTime(automation::scheduler::AUTOMATION_TRACE_MAIN_DIAGNOSIS_CODE_ERROR, logParameters, ##__VA_ARGS__); \
  } \
}

    //! @class Trace
    class BaseTrace
    {
    public:
      BaseTrace();
      virtual ~BaseTrace() = default;

      inline const char* getSnapName() { return m_snapName.c_str(); }
      
    private:
      std::string m_snapName = "snap.automation.scheduler";
    };

    //! @class RealTime Trace
    class RtTrace : public BaseTrace
    {
    public:
      RtTrace() = default;
      virtual ~RtTrace() = default;
      RtTrace(const RtTrace&) = delete;
      RtTrace& operator=(const RtTrace&) = delete;

      common::log::trace::ILogRealTime& operator()() const { return *m_rtTraceContext; }

      void attachTrace(common::log::trace::IRegistrationRealTime2* traceRegistration);
      void detachTrace(common::log::trace::IRegistrationRealTime2* traceRegistration);

      inline const char* getTraceName() { return m_traceName.c_str(); }

      static void getTraceEnablingState(const common::log::trace::EnablingState& traceEnablingState) { g_rt_trace.m_traceEnablingState = traceEnablingState; return; }

      inline bool getTraceEnablingStateMessage(void) const { return m_traceEnablingState.messages; }
      inline bool getTraceEnablingStateWarning(void) const { return m_traceEnablingState.warnings; }
      inline bool getTraceEnablingStateError(void) const { return m_traceEnablingState.errors; }

      void enableTraceStateMessage(void) { m_traceEnablingState.messages = true; return; }
      void enableTraceStateWarning(void) { m_traceEnablingState.warnings = true; return; }
      void enableTraceStateError(void) { m_traceEnablingState.errors = true; return; }

      void disableTraceStateMessage(void) { m_traceEnablingState.messages = false; return; }
      void disableTraceStateWarning(void) { m_traceEnablingState.warnings = false; return; }
      void disableTraceStateError(void) { m_traceEnablingState.errors = false; return; }

    private:
      const std::string m_traceName = "automation.scheduler.rt";
      common::log::trace::ILogRealTime* m_rtTraceContext = &m_traceBuffered;
      common::log::trace::LogBuffered m_traceBuffered{getTraceName(), true};

      common::log::trace::EnablingState m_traceEnablingState = {false, true, true}; // message, warning, error
    };
  }
}
