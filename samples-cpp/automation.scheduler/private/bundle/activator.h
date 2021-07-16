#pragma once

// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include "celix/dm/DmActivator.h"
#include "common/scheduler/i_scheduler.h"
#include "common/log/trace/i_log_real_time.h"


// ---------------------------------------------------------------------------
// Name Spaces
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Declaration of symbolic constants
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Declaration of macros
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Declaration of data types
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Forward references
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Declaration of classes and structures
// ---------------------------------------------------------------------------


//! @class Activator
//! The activator implements a "normal" celix bundle activator and depends on four functions which needs to be implemented by the user of the Depedency Manager:
class Activator : public DmActivator {
private:

public:
 explicit Activator(DependencyManager& mng) : DmActivator{ mng }{}


  //! This method should be used to interact with the Dependency Manager.
  //! Here a user can  instantiate components, service dependencies and provided services.
  //! This method is called during start of bundle, after 'create'.
  virtual void init() override;

  //! This method should be used to deinitialize and deallocate objects created in the dm_create function.
  //! This method is called during stop of bundle.
  virtual void deinit() override;
};
