#pragma once

// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include <iostream>
#include "common/scheduler/i_scheduler.h"

// ---------------------------------------------------------------------------
// Name Spaces
// ---------------------------------------------------------------------------
namespace example {
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
  class HelloCallable : public common::scheduler::ICallable
  {
  private:

  common::scheduler::SchedState m_state = ::common::scheduler::SchedState::SCHED_STATE_INIT;  //!< state of the callable
  const double m_schedCycleTimeConv = 20000;                                                  //!< constant to convert scheduler tick time (micro seconds) to internal cycle time (seconds)

  public:
    //! @brief Constructor
    HelloCallable();
    //! @brief Destructor
    virtual ~HelloCallable() = default;

    //! This method returns the class name.
    //!
    //! @return                      Return class name.
    static const char* getClassName(void) { return (CLASS_NAME(HelloCallable)); }

    common::scheduler::SchedEventResponse execute(const common::scheduler::SchedEventType& eventType, const common::scheduler::SchedEventPhase& eventPhase, comm::datalayer::Variant& param) final;
    
    //! @brief This method returns state of the callable.
    //! @return Return callable state.
    common::scheduler::SchedState getState() const;

  };
}
