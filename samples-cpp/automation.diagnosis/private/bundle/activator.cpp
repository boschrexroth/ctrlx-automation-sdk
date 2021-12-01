
// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include <iostream>
#include "activator.h"
#include "cmp1.h"
#include "common/log/diagnosis/i_registration.h"
#include "common/log/diagnosis/distributor/i_registration.h"


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
  if (cmp)
  {
    // remember object because after 'move' 'cmp' is not longer valid, usable for register interfaces
    auto ptrObject = cmp.get();

    // save bundle context because needed to detect bundle path etc.
    cmp->setDependencyManager(mng.bundleContext());

    // create dependency manager component; adapt component name if needed
    Component<sdk::control::Cmp1>& component = mng.createComponent(std::move(cmp));

    // set callbacks for life cycle management
    component.setCallbacks(&sdk::control::Cmp1::init, &sdk::control::Cmp1::start, &sdk::control::Cmp1::stop, &sdk::control::Cmp1::deinit);

    // diagnosis: cpp ServiceDependency w/ specifier 'name'
    ServiceDependency<sdk::control::Cmp1, common::log::diagnosis::IRegistration3>& serviceDepDiagnosis = component.createServiceDependency<common::log::diagnosis::IRegistration3>(common::log::diagnosis::CPP_INTERFACE_DIAGNOSIS_REGISTRATION_3_NAME);
    serviceDepDiagnosis.setVersionRange(common::log::diagnosis::CPP_INTERFACE_DIAGNOSIS_REGISTRATION_3_CONSUMER_RANGE);
    serviceDepDiagnosis.setRequired(true);
    serviceDepDiagnosis.setStrategy(DependencyUpdateStrategy::suspend);
    serviceDepDiagnosis.setCallbacks(&sdk::control::Cmp1::cppDiagnosisServiceStateChanged);
    serviceDepDiagnosis.setCallbacks(&sdk::control::Cmp1::cppDiagnosisServiceAdded, &sdk::control::Cmp1::cppDiagnosisServiceRemoved);

    // diagnosis distributor: cpp ServiceDependency w/ specifier 'name'
    ServiceDependency<sdk::control::Cmp1, common::log::diagnosis::distributor::IRegistration>& serviceDepDiagnosisDistributor = component.createServiceDependency<common::log::diagnosis::distributor::IRegistration>(common::log::diagnosis::CPP_INTERFACE_DIAGNOSIS_DISTRIBUTOR_REGISTRATION_NAME);
    serviceDepDiagnosisDistributor.setVersionRange(common::log::diagnosis::CPP_INTERFACE_DIAGNOSIS_DISTRIBUTOR_REGISTRATION_CONSUMER_RANGE);
    serviceDepDiagnosisDistributor.setRequired(true);
    serviceDepDiagnosisDistributor.setStrategy(DependencyUpdateStrategy::suspend);
    serviceDepDiagnosisDistributor.setCallbacks(&sdk::control::Cmp1::cppDiagnosisDistributorServiceStateChanged);
    serviceDepDiagnosisDistributor.setCallbacks(&sdk::control::Cmp1::cppDiagnosisDistributorServiceAdded, &sdk::control::Cmp1::cppDiagnosisDistributorServiceRemoved);
  }
}

void Activator::deinit()
{
  std::cout << "call of '" << __func__ << "'..." << std::endl;
}
