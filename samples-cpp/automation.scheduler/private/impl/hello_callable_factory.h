#pragma once

// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include <iostream>

#include "common/scheduler/i_scheduler.h"
#include "hello_callable.h"

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
  class HelloCallableFactory : public common::scheduler::ICallableFactory 
  {
  private:

    std::shared_ptr<HelloCallable> m_helloCallable;

  public:
    //! @brief Constructor
    HelloCallableFactory();
    //! @brief Destructor
    virtual ~HelloCallableFactory() = default;


    //! This method returns the class name.
    //!
    //! @return                      Return class name.
    static const char* getClassName(void) { return (CLASS_NAME(HelloCallableFactory)); }

    std::shared_ptr<common::scheduler::ICallable> createCallable(const comm::datalayer::Variant& param) final;
    common::scheduler::SchedStatus destroyCallable(const std::shared_ptr<common::scheduler::ICallable>& callable) final;
    common::scheduler::SchedStatus getCallableArguments(std::vector<std::string>& arguments) final;

  };
}
