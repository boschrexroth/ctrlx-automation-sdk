// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include <iostream>
#include "cmp1.h"
#include "hello_trace.h"
#include "common/log/trace/defs.h"
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

//
// using CPP services regarding "Scheduler"
//

void Cmp1::cppSchedulerServiceStateChanged(common::scheduler::IScheduler* service)
{
  std::cout << "" << std::endl;
}

void Cmp1::cppSchedulerServiceAdded(common::scheduler::IScheduler* service)
{

  m_factory = std::make_shared<example::HelloCallableFactory>();
  if (m_factory)
  {
    service->registerCallableFactory(m_factory, m_factory->getClassName());
  }
}

void Cmp1::cppSchedulerServiceRemoved(common::scheduler::IScheduler* service)
{

  if (m_factory)
  {
    service->unregisterCallableFactory(m_factory);
  }

}

void Cmp1::cppRtTraceServiceStateChanged(common::log::trace::IRegistrationRealTime* service)
{
  TRACE_MSG("Trace Service changed");
}

void Cmp1::cppRtTraceServiceAdded(common::log::trace::IRegistrationRealTime* service)
{
  m_rtTraceSrv = service;
  automation::scheduler::g_rt_trace.attachTrace(service);
  
}

void Cmp1::cppRtTraceServiceRemoved(common::log::trace::IRegistrationRealTime* service)
{
  automation::scheduler::g_rt_trace.detachTrace(service);
  m_rtTraceSrv = nullptr;
}

}
}