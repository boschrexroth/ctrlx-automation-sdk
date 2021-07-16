
// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include <iostream>
#include "motion/core/interface/motion_cmd_if.h"
#include "motion/core/interface/motion_sys_if.h"
#include "activator.h"
#include "cmp1.h"


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

    {
      // MotionSystem command interface: c ServiceDependency with specifier 'name'
      CServiceDependency<sdk::control::Cmp1, MotionCmdIfT>& svcDepCmd = component.createCServiceDependency<MotionCmdIfT>(MOTION_CMD_CM_C_INTERFACE_NAME);
      svcDepCmd.setVersionRange(MOTION_CMD_CM_C_INTERFACE_CONSUMER_RANGE);
      svcDepCmd.setRequired(true);
      svcDepCmd.setStrategy(DependencyUpdateStrategy::suspend);
      //svcDepCmd.setCallbacks(&sdk::control::Cmp1::cMotionServiceStateChanged);
      svcDepCmd.setCallbacks(&sdk::control::Cmp1::cMotionServiceAdded, &sdk::control::Cmp1::cMotionServiceRemoved);
    }

    {
      // MotionSystem system interface: c ServiceDependency with specifier 'name'
      CServiceDependency<sdk::control::Cmp1, MotionSysIfT>& svcDepSys = component.createCServiceDependency<MotionSysIfT>(MOTION_SYS_CM_C_INTERFACE_NAME);
      svcDepSys.setVersionRange(MOTION_SYS_CM_C_INTERFACE_CONSUMER_RANGE);
      svcDepSys.setRequired(true);
      svcDepSys.setStrategy(DependencyUpdateStrategy::suspend);
      //svcDepSys.setCallbacks(&sdk::control::Cmp1::cMotionServiceStateChanged);
      svcDepSys.setCallbacks(&sdk::control::Cmp1::cMotionServiceAdded, &sdk::control::Cmp1::cMotionServiceRemoved);
    }
  }
}

void Activator::deinit()
{
  std::cout << "call of '" << __func__ << "'..." << std::endl;
}
