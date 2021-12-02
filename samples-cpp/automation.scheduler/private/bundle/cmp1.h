#pragma once
// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include "celix_errno.h"
#include "celix/dm/DmActivator.h"
#include "hello_callable_factory.h"
#include "common/scheduler/i_scheduler.h"
#include "comm/datalayer/datalayer.h"
#include "common/log/trace/i_registration.h"
#include "activator.h"

// ---------------------------------------------------------------------------
// Name Spaces
// ---------------------------------------------------------------------------

namespace sdk {
namespace control {

// ---------------------------------------------------------------------------
// Declaration of symbolic constants
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Declaration of macros
// ---------------------------------------------------------------------------
#define _CLASS_NAME_(x)   (           # x )
#define CLASS_NAME(x)     (_CLASS_NAME_(x))


// ---------------------------------------------------------------------------
// Declaration of data types
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Forward references
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Declaration of classes and structures
// ---------------------------------------------------------------------------


//! @class Cmp1
//! Here is some extensive information about what this class is for and
//! how this class is to be used!
class Cmp1
{
private:
  bundle_context_pt m_context;
  std::string m_bundlePath;
  std::string m_bundleName;

  std::shared_ptr<example::HelloCallableFactory> m_factory;

  common::log::trace::IRegistrationRealTime2* m_rtTraceSrv = nullptr;

public:
  Cmp1() = default;
  virtual ~Cmp1() = default;

  
  //! This method is used for the life cycle management of this component.
  //! It is called during start of the bundle, after 'Activator::init'.
  //! @note This function will not be called in context of 'Activator::init'.
  celix_status_t init();

  //! This method is used for the life cycle management of this component.
  //! It is called during start of the bundle, after 'Component::init'.
  //! @note This function will not be called in context of 'Activator::init'.
  celix_status_t start();

  //! This method is used for the life cycle management of this component.
  //! It is called during stop of the bundle, after 'Activator::deinit'.
  //! @note This function will not be called in context of 'Activator::deinit'.
  celix_status_t stop();

  //! This method is used for the life cycle management of this component.
  //! It is called during stop of the bundle, after 'Component::stop'.
  //! @note This function will not be called in context of 'Activator::deinit'.
  celix_status_t deinit();

  //! This method is used to set bundle context from activator, the context is
  //! e.g. needed for determination of bundle path.
  void setDependencyManager(bundle_context_pt context) { m_context = context; };

  //! This method is used to set root path of bundle from activator, the path is 
  //! e.g. needed for finding files, loading additional shared objects etc..
  void setBundlePath(const char* bundlePath) { if (nullptr != bundlePath) { m_bundlePath.assign(bundlePath); } else { m_bundlePath.clear(); } };

  //! This method is used to clear root path of bundle.
  void clrBundlePath(void) { setBundlePath(nullptr); };

  //! This method is used to set name of bundle from activator, the name is
  //! e.g. needed for messages etc..
  void setBundleName(const char* bundleName) { if (nullptr != bundleName) { m_bundleName.assign(bundleName); } else { m_bundleName.clear(); } };

  //! This method is used to clear name of bundle.
  void clrBundleName(void) { setBundleName(nullptr); };

  //
  // Scheduler
  //
  //! This method is called when the state of a cpp service changes
  //!
  //! @param[in]  service  A pointer to the service.
  void cppSchedulerServiceStateChanged(common::scheduler::IScheduler* service);

  //! This method is called when a cpp service is added
  //!
  //! @param[in]  service  A pointer to the service.
  void cppSchedulerServiceAdded(common::scheduler::IScheduler* service);

  //! This method is called when a cpp service is removed
  //!
  //! @param[in]  service  A pointer to the service.
  void cppSchedulerServiceRemoved(common::scheduler::IScheduler* service);

  //
  // Trace
  //
  //! This method is called when the state of a cpp service changes
  //!
  //! @param[in]  service  A pointer to the service.
  void cppRtTraceServiceStateChanged(common::log::trace::IRegistrationRealTime2* service);

  //! This method is called when a cpp service is added
  //!
  //! @param[in]  service  A pointer to the service.
  void cppRtTraceServiceAdded(common::log::trace::IRegistrationRealTime2* service);

  //! This method is called when a cpp service is removed
  //!
  //! @param[in]  service  A pointer to the service.
  void cppRtTraceServiceRemoved(common::log::trace::IRegistrationRealTime2* service);

};

}
}