#pragma once

#include "common/log/trace/i_log_real_time.h"
#include "common/log/trace/i_registration.h"
#include "common/log/trace/log_buffered.h"

namespace common { namespace log { namespace trace { class IRegistration; } } }
namespace common { namespace log { namespace trace { class IRegistrationRealTime2; } } }
namespace common { namespace log { namespace trace { enum class TraceResult; } } }

#define AUTOMATIONTRACE_API

namespace automation
{
  namespace trace
  {
    class BaseTrace;
    class RtTrace;

    AUTOMATIONTRACE_API extern RtTrace g_rt_trace;

#define TRACE_ENTITY "automation.trace"

#define TRACE_MSG(text, ...) { \
  if (automation::trace::g_rt_trace.getTraceEnablingStateMessage()) \
  { \
    ::common::log::trace::LogParametersExtended logParameters{TRACE_ENTITY, automation::trace::g_rt_trace.getSnapName(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, text}; \
    automation::trace::g_rt_trace().logRealTime(COMMON_LOG_TRACE_EXTERNAL_USER_MAIN_DIAGNOSIS_CODE_MESSAGE, logParameters, ##__VA_ARGS__); \
  } \
}
#define TRACE_WARN(text, ...) { \
  if (automation::trace::g_rt_trace.getTraceEnablingStateWarning()) \
  { \
    ::common::log::trace::LogParametersExtended logParameters{TRACE_ENTITY, automation::trace::g_rt_trace.getSnapName(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, text}; \
    automation::trace::g_rt_trace().logRealTime(COMMON_LOG_TRACE_EXTERNAL_USER_MAIN_DIAGNOSIS_CODE_WARNING, logParameters, ##__VA_ARGS__); \
  } \
}
#define TRACE_ERR(text, ...) { \
  if (automation::trace::g_rt_trace.getTraceEnablingStateError()) \
  { \
    ::common::log::trace::LogParametersExtended logParameters{TRACE_ENTITY, automation::trace::g_rt_trace.getSnapName(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, text}; \
    automation::trace::g_rt_trace().logRealTime(COMMON_LOG_TRACE_EXTERNAL_USER_MAIN_DIAGNOSIS_CODE_ERROR, logParameters, ##__VA_ARGS__); \
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
      std::string m_snapName = "snap.automation.trace";
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

      static void getTraceEnablingState(common::log::trace::EnablingState &traceEnablingState) { traceEnablingState = g_rt_trace.m_traceEnablingState; }

      inline bool getTraceEnablingStateMessage(void) const { return m_traceEnablingState.messages; }
      inline bool getTraceEnablingStateWarning(void) const { return m_traceEnablingState.warnings; }
      inline bool getTraceEnablingStateError(void) const { return m_traceEnablingState.errors; }

      void enableTraceStateMessage(void) { if (m_traceRegistration) {m_traceRegistration->setEnablingState(m_rtTraceContext, {true, false, false});} }
      void enableTraceStateWarning(void) { if (m_traceRegistration) {m_traceRegistration->setEnablingState(m_rtTraceContext, {false, true, false});} }
      void enableTraceStateError(void) { if (m_traceRegistration) {m_traceRegistration->setEnablingState(m_rtTraceContext, {false, false, true});} }

    private:
      common::log::trace::IRegistrationRealTime2* m_traceRegistration = nullptr;
      const std::string m_traceName = "automation.trace.rt";
      common::log::trace::ILogRealTime* m_rtTraceContext = &m_traceBuffered;
      common::log::trace::LogBuffered m_traceBuffered{getTraceName(), true};

      common::log::trace::EnablingState m_traceEnablingState = {false, false, true}; // message, warning, error
    };
  }
}
