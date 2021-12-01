
// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include <iostream>
#include "activator.h"
#include "cmp1.h"
#include "common/log/trace/i_registration.h"


// ---------------------------------------------------------------------------
// Name Spaces
// ---------------------------------------------------------------------------


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


//! This method should be used to allocated and initialize a dm activator structure.
//! If needed this structure can be used to store object during the lifecycle of the bundle.
//! This method is called during start of bundle.
//!
//! @param[in]    mng            A reference to the dependency manager.
//! @return                      A pointer to the created activator class.
DmActivator* DmActivator::create(DependencyManager& mng)
{
  std::cout << "call of 'DmActivator" << "::" << __func__ << "'..." << std::endl;

  // instantiate activator
  return new Activator(mng);
}

void Activator::init()
{
  
  // create component
  auto cmp = std::unique_ptr<sdk::control::Cmp1>(new sdk::control::Cmp1());
  if (cmp) {
    // remember object because after 'move' 'cmp' is not longer valid, usable for register interfaces
    auto ptrObject = cmp.get();

    // save bundle context because needed to detect bundle path etc.
    cmp->setDependencyManager(mng.bundleContext());

    // create dependency manager component; adapt component name if needed
    Component<sdk::control::Cmp1>& component = mng.createComponent(std::move(cmp));

    // set callbacks for life cycle management
    component.setCallbacks(&sdk::control::Cmp1::init, &sdk::control::Cmp1::start, &sdk::control::Cmp1::stop, &sdk::control::Cmp1::deinit);

    // rt trace: cpp ServiceDependency w/ specifier 'name'
    ServiceDependency<sdk::control::Cmp1, common::log::trace::IRegistrationRealTime2>& serviceDepRtTrace = component.createServiceDependency<common::log::trace::IRegistrationRealTime2>(common::log::trace::CPP_INTERFACE_REAL_TIME_2_NAME);
    serviceDepRtTrace.setVersionRange(common::log::trace::CPP_INTERFACE_CONSUMER_RANGE);
    serviceDepRtTrace.setRequired(true);
    serviceDepRtTrace.setStrategy(DependencyUpdateStrategy::locking);
    serviceDepRtTrace.setCallbacks(&sdk::control::Cmp1::cppRtTraceServiceStateChanged);
    serviceDepRtTrace.setCallbacks(&sdk::control::Cmp1::cppRtTraceServiceAdded, &sdk::control::Cmp1::cppRtTraceServiceRemoved);
  }
}

void Activator::deinit()
{
  std::cout << "call of '" << __func__ << "'..." << std::endl;
}
