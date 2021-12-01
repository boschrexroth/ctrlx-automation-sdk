// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include <iostream>
#include "cmp1.h"
#include "hello_trace.h"
#include "common/log/trace/log_buffered.h"


// ---------------------------------------------------------------------------
// Name Spaces
// ---------------------------------------------------------------------------
namespace sdk {
namespace control {


// ---------------------------------------------------------------------------
// Declaration of constants
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Declaration of macros
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Forward function references
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Class implementation
// ---------------------------------------------------------------------------
celix_status_t Cmp1::init()
{
  std::cout << "call of '" << m_bundleName.c_str() << ": " << __func__ << "'..." << std::endl;
  return CELIX_SUCCESS;
}

celix_status_t Cmp1::start()
{
  std::cout << "call of '" << m_bundleName.c_str() << ": " << __func__ << "'..." << std::endl;

  std::string testString = "Hello Trace";
  std::int32_t testInteger = 42;
  TRACE_MSG("Example trace message with string '%s' and integer '%d'.", testString.c_str(), testInteger);
  TRACE_WARN("Example trace warning with string '%s' and integer '%d'.", testString.c_str(), testInteger);
  TRACE_ERR("Example trace error with string '%s' and integer '%d'.", testString.c_str(), testInteger);

  return CELIX_SUCCESS;
}

celix_status_t Cmp1::stop()
{
  std::cout << "call of '" << m_bundleName.c_str() << ": " << __func__ << "'..." << std::endl;
  return CELIX_SUCCESS;
}

celix_status_t Cmp1::deinit()
{
  std::cout << "call of '" << m_bundleName.c_str() << ": " << __func__ << "'..." << std::endl;
  return CELIX_SUCCESS;
}

void Cmp1::cppRtTraceServiceStateChanged(common::log::trace::IRegistrationRealTime2* service)
{
  TRACE_MSG("Trace Service changed");
}

void Cmp1::cppRtTraceServiceAdded(common::log::trace::IRegistrationRealTime2* service)
{
  m_rtTraceSrv = service;
  automation::trace::g_rt_trace.attachTrace(service);
}

void Cmp1::cppRtTraceServiceRemoved(common::log::trace::IRegistrationRealTime2* service)
{
  automation::trace::g_rt_trace.detachTrace(service);
  m_rtTraceSrv = nullptr;
}

}
}