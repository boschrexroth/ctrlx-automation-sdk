// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include <iostream>
#include "cmp1.h"



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
  m_example = new example::Example();
  m_motion = new example::Motion( );
  return CELIX_SUCCESS;
}

celix_status_t Cmp1::start()
{
  std::cout << "call of '" << m_bundleName.c_str() << ": " << __func__ << "'..." << std::endl;
  m_example->hello_world();
  std::this_thread::sleep_for(std::chrono::seconds(20));
  m_motion->start( m_cmdIf, m_sysIf );
  std::wcout << "Motion state " << m_sysIf->getMotionState(m_sysIf->handle) << std::endl;
  return CELIX_SUCCESS;
}

celix_status_t Cmp1::stop()
{
  std::cout << "call of '" << m_bundleName.c_str() << ": " << __func__ << "'..." << std::endl;
  m_example->goodbye_world();
  m_motion->stop( );
  return CELIX_SUCCESS;
}

celix_status_t Cmp1::deinit()
{
  std::cout << "call of '" << m_bundleName.c_str() << ": " << __func__ << "'..." << std::endl;
  delete m_example;
  delete m_motion;
  return CELIX_SUCCESS;
}

//
// services regarding MotionSystem"
//

void Cmp1::cMotionServiceStateChanged(const MotionCmdIfT* motionCmdIF)
{
  std::cout << "Bundlename: " << m_bundleName.c_str() << std::endl;
}

void Cmp1::cMotionServiceAdded(const MotionCmdIfT* motionCmdIF)
{
  std::cout << "Bundlename: " << m_bundleName.c_str() << std::endl;
  m_cmdIf = motionCmdIF;
}

void Cmp1::cMotionServiceRemoved(const MotionCmdIfT* motionCmdIF)
{
  std::cout << "Bundlename: " << m_bundleName.c_str() << std::endl;
  m_cmdIf = nullptr;
}

void Cmp1::cMotionServiceStateChanged(const MotionSysIfT* motionSysIF)
{
  std::cout << "Bundlename: " << m_bundleName.c_str() << std::endl;
}

void Cmp1::cMotionServiceAdded(const MotionSysIfT* motionSysIF)
{
  std::cout << "Bundlename: " << m_bundleName.c_str() << std::endl;
  m_sysIf = motionSysIF;
}

void Cmp1::cMotionServiceRemoved(const MotionSysIfT* motionSysIF)
{
  std::cout << "Bundlename: " << m_bundleName.c_str() << std::endl;
  m_sysIf = nullptr;
}

}
}

// ---------------------------------------------------------------------------
// C implementation
// ---------------------------------------------------------------------------
