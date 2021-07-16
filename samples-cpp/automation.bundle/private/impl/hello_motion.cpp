/**
 * MIT License
 * 
 * Copyright (c) 2020-2021 Bosch Rexroth AG
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
// ---------------------------------------------------------------------------
//
//! This file is part of the SDK and shows interactions with the motion system.
//
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include <cmath>
#include <iostream>
#include "hello_motion.h"

// ---------------------------------------------------------------------------
// Name Spaces
// ---------------------------------------------------------------------------
using namespace example;


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
void Motion::start(const MotionCmdIfT *cmdIf, const MotionSysIfT *sysIf )
{
  std::cout << "Hello MotionX" << std::endl;
  m_cmdIf = cmdIf;
  m_sysIf = sysIf;

  m_thread = new std::thread( &Motion::main, this );
}

void Motion::stop()
{
  m_abortThread = true;
  m_thread->join( );
  delete m_thread;
  m_thread = nullptr;
  std::cout << "Goodbye MotionX" << std::endl;
}


bool Motion::axisExists( const char *axisName )
{
  ::motion::core::statemachine::PlcOpenStateAxis state;
  return ::motion::core::MotionResult::STS_OK == m_sysIf->getAxsPlcOpenState(m_sysIf->handle, axisName, state );
}

bool Motion::switchState( const char *expected, const char *command )
{
  std::string expState( expected );
  if ( expState == m_sysIf->getMotionState(m_sysIf->handle) )
  {
    // we are in the expected state already
    return true;
  }

  if ( command )
  {
    const char *oldState = m_sysIf->getMotionState(m_sysIf->handle);
    std::cout << "Request to switch to motion state " << command << " (current state is " << oldState << ")!" << std::endl;
    if ( ::motion::core::MotionResult::STS_OK != m_sysIf->requestMotionState(m_sysIf->handle, command ) )
    {
      std::cout << "Request to switch to motion state" << command << " failed (old state was " << oldState << ", current state is " << m_sysIf->getMotionState(m_sysIf->handle ) << ")!" << std::endl;
      return false;
    }
  }

  uint32_t timeOut = 200;
  while ( ( timeOut > 0 ) && ( expState != m_sysIf->getMotionState(m_sysIf->handle ) ) )
  {
    if ( m_abortThread )
    {
      return false;
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    --timeOut;
  }

  if ( expState != m_sysIf->getMotionState(m_sysIf->handle ) )
  {
    std::cout << "Unexpected state (found=" << m_sysIf->getMotionState(m_sysIf->handle ) << ", expected=" << expState << ")!" << std::endl;
    return false;
  }

  return true;
}


bool Motion::switchAxisPower( const char *axisName, bool powerOn )
{
  // source information should be set to get helpful error diagnosis
  m_sourceInfo.m_sourceName = __FILE__;
  m_sourceInfo.m_sourceLine = __LINE__;

  uint64_t cmdID;
  if ( ::motion::core::MotionResult::STS_OK != m_cmdIf->axsPowerCmd(m_cmdIf->handle, m_sourceInfo, axisName, powerOn, cmdID ) )
  {
    // you should do are more detailled error check here
    std::cout << "Could not create power command!" << std::endl;
    return false;
  }
  std::cout << "Switch power of axis " << axisName << ". powerOn = " << powerOn << std::endl;

  // now we have to wait, until the requested power state is achieved
  // easiest way is to check the PLC Open state of the axis
  ::motion::core::statemachine::PlcOpenStateAxis expectedState = powerOn ?
  ::motion::core::statemachine::PlcOpenStateAxis::PLC_OPEN_STATE_AXIS_STANDSTILL :
  ::motion::core::statemachine::PlcOpenStateAxis::PLC_OPEN_STATE_AXIS_DISABLED;
  uint32_t timeOut = 200;
  while ( timeOut > 0 )
  {
    ::motion::core::statemachine::PlcOpenStateAxis currentState;
    m_sysIf->getAxsPlcOpenState(m_sysIf->handle, axisName, currentState );
    if ( currentState == expectedState )
    {
      return true;
    }
    if ( m_abortThread )
    {
      return false;
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    --timeOut;
  }

  // you should do are more detailled error check here
  std::cout << "Could not power axis!" << std::endl;
  return false;
}

bool Motion::moveAxis( const char *axisName, const double targetPos, bool buffered )
{
  // source information should be set to get helpful error diagnosis
  m_sourceInfo.m_sourceName = __FILE__;
  m_sourceInfo.m_sourceLine = __LINE__;

  // set the dynamic limits of the movement here
  ::motion::core::cmd::DynamicLimitsT dynLim;
  dynLim.m_vel = 0.5;         // velocity limit
  dynLim.m_acc = 2.0;         // acceleration limit
  dynLim.m_dec = 2.5;         // deceleration limit

  uint64_t cmdID;
  if ( ::motion::core::MotionResult::STS_OK != m_cmdIf->axsPosCmd(m_cmdIf->handle, m_sourceInfo, axisName, ::motion::core::cmd::axs::AxsCmdType::AXS_CMD_POS_ABS, targetPos, dynLim, buffered, cmdID ) )
  {
    // you should do are more detailled error check here
    std::cout << "Could not create move command!" << std::endl;
    return false;
  }
  std::cout << "Move axis " << axisName << " to targetPos " << targetPos << std::endl;

  return true;
}

bool Motion::waitForAxisPos( const char *axisName, const double targetPos )
{
  uint32_t timeOut = 2000;
  std::cout << "Waiting for axis " << axisName << " to reach position " << targetPos << std::endl;
  while ( timeOut > 0 )
  {
    double pos;
    double vel;
    double acc;
    double jrk;
    m_sysIf->getAxisIpoValues(m_sysIf->handle, axisName, pos, vel, acc, jrk );
    if ( fabs(pos - targetPos) < 1E-3 )
    {
      return true;
    }
    if ( m_abortThread )
    {
      return false;
    }
    std::cout << "Actual position: " << pos << std::endl;
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    --timeOut;
  }

  // you should do are more detailled error check here
  std::cout << "Axis did not arrive at target position!" << std::endl;
  return false;
}

bool Motion::nonBufferedAxisMove( const char *axisName )
{
  const double targetPosition{ 3.4 };
  if ( !moveAxis( axisName, targetPosition ) )
  {
    return false;
  }

  // let's check, when the axis arrives at the target position
  if ( !waitForAxisPos( axisName, targetPosition ) )
  {
    return false;
  }

  // and move back again
  if ( !moveAxis( axisName, 0.0 ) || !waitForAxisPos( axisName, 0.0 ) )
  {
    return false;
  }

  // when we don't wait until the target position is reached, every non-buffered command will overwrite the last one
  // the next line won't move the axis at all, because the first move command is overwritten by the second one
  if ( !moveAxis( axisName, targetPosition ) || !moveAxis( axisName, 0.0 ) )
  {
    return false;
  }

  std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
  return true;
}

bool Motion::bufferedAxisMove( const char *axisName )
{
  // when you create buffered commands, all commands are executed sequentially
  std::vector<double> targetPositions{ -5.5, -1.2, 3.0, -6.0 };

  for ( const auto &pos : targetPositions )
  {
    if ( !moveAxis( axisName, pos, true ) )
    {
      return false;
    }
  }

  // let's check, when the axis arrives at the last target position
  if ( !waitForAxisPos( axisName, targetPositions.back() ) )
  {
    return false;
  }

  return true;
}

void Motion::axisExample( )
{
  const char *AXIS_NAME = "HelloMotion";

  // check, if the axis already exists (from a prio run)
  if ( !axisExists( AXIS_NAME ) )
  {
    // we want to create a single axis, so we have to switch to configuration state first
    if ( !switchState( "Configuration", "Configuration" ) )
    {
      return;
    }

    // now create a new axis
    if ( ::motion::core::MotionResult::STS_OK != m_sysIf->createAxis(m_sysIf->handle, AXIS_NAME ) )
    {
      std::cout << "Could not create axis!" << std::endl;
      return;
    }

    // the axis is created, now switch to running (via booting) to move it
    if ( !switchState( "Running", "Booting" ) )
    {
      return;
    }

    // we can change the parameters of the axis (except it's name) when in RUNNING state from the DataLayer (not in this example)
    
    // the axis is currently (after booting) disabled and without power
    // so, we have to switch power on first (see PLC Open state machine for single axis)
    // this is done by a command
    if ( !switchAxisPower( AXIS_NAME, true ) )
    {
      return;
    }

    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
  }

  // finally, we can command our new axis to move

  // let's do some non-buffered commands 
  std::cout << "Starting non-buffered commands" << std::endl;
  if ( !nonBufferedAxisMove( AXIS_NAME ) )
  {
    return;
  }
  // and then some buffered commands
  std::cout << "Starting buffered commands" << std::endl;
  if ( !bufferedAxisMove( AXIS_NAME ) )
  {
    return;
  }
  std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

  // now we switch power off before we leave
  // this is done by a command
  if ( !switchAxisPower( AXIS_NAME, false ) )
  {
    return;
  }
  std::cout << "End of MotionExample" << std::endl;
}

void Motion::main( )
{
  std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
  axisExample( );
  std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
  kinematicsExample( );
}

bool Motion::kinExists( const char *kinName )
{
  ::motion::core::statemachine::PlcOpenStateGroup state;
  return ::motion::core::MotionResult::STS_OK == m_sysIf->getKinPlcOpenState(m_sysIf->handle, kinName, state );
}

bool Motion::enableKin( const char *kinName, bool doEnable )
{
  // source information should be set to get helpful error diagnosis
  m_sourceInfo.m_sourceName = __FILE__;
  m_sourceInfo.m_sourceLine = __LINE__;

  uint64_t cmdID;
  ::motion::core::cmd::kin::KinCmdType cmdType = doEnable ? ::motion::core::cmd::kin::KinCmdType::KIN_CMD_GROUP_ENA : ::motion::core::cmd::kin::KinCmdType::KIN_CMD_GROUP_DIS;
  if ( ::motion::core::MotionResult::STS_OK != m_cmdIf->kinGroupCmd(m_cmdIf->handle, m_sourceInfo, kinName, cmdType, cmdID ) )
  {
    // you should do are more detailled error check here
    std::cout << "Could not create group command!" << std::endl;
    return false;
  }
  std::cout << "Group command for kinematic " << kinName << std::endl;

  ::motion::core::statemachine::PlcOpenStateGroup expectedState = doEnable ?
  ::motion::core::statemachine::PlcOpenStateGroup::PLC_OPEN_STATE_GROUP_STANDBY :
  ::motion::core::statemachine::PlcOpenStateGroup::PLC_OPEN_STATE_GROUP_DISABLED;
  uint32_t timeOut = 200;
  while ( timeOut > 0 )
  {
    ::motion::core::statemachine::PlcOpenStateGroup currentState;
    m_sysIf->getKinPlcOpenState(m_sysIf->handle, kinName, currentState );
    if ( currentState == expectedState )
    {
      if (doEnable == true)
      {
        std::cout << "Group command successfull" << std::endl; 
      }
      else
      {
        std::cout << "UnGroup command successfull" << std::endl;        
      }
      return true;
    }
    if ( m_abortThread )
    {
      return false;
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    --timeOut;
  }

  // you should do are more detailled error check here
  std::cout << "Could not enable kinematics!" << std::endl;
  return false;
}

bool Motion::addAxisToKin( const char *axisName, const char *kinName )
{
  // source information should be set to get helpful error diagnosis
  m_sourceInfo.m_sourceName = __FILE__;
  m_sourceInfo.m_sourceLine = __LINE__;

  uint64_t cmdID;
  if ( ::motion::core::MotionResult::STS_OK != m_cmdIf->axsAddToKinCmd(m_cmdIf->handle, m_sourceInfo, axisName, kinName, false, cmdID ) )
  {
    // you should do are more detailled error check here
    std::cout << "Could not create AddToKin command!" << std::endl;
    return false;
  }
  std::cout << "Add axis " << axisName << " to kinematic " << kinName << std::endl;

  ::motion::core::statemachine::PlcOpenStateAxis expectedState = 
   ::motion::core::statemachine::PlcOpenStateAxis::PLC_OPEN_STATE_AXIS_COORDINATED_MOTION;
  uint32_t timeOut = 200;
  while ( timeOut > 0 )
  {
    ::motion::core::statemachine::PlcOpenStateAxis currentState;
    m_sysIf->getAxsPlcOpenState(m_sysIf->handle, axisName, currentState );
    if ( currentState == expectedState )
    {
      std::cout << "Axis " << axisName << " is in CoordinatedMotion " << std::endl;
      return true;
    }
    if ( m_abortThread )
    {
      return false;
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    --timeOut;
  }

  // you should do are more detailled error check here
  std::cout << "Could not add to kinematics!" << std::endl;
  return false;
}

bool Motion::removeAxisFromKin( const char *axisName )
{
  // source information should be set to get helpful error diagnosis
  m_sourceInfo.m_sourceName = __FILE__;
  m_sourceInfo.m_sourceLine = __LINE__;

  uint64_t cmdID;
  if ( ::motion::core::MotionResult::STS_OK != m_cmdIf->axsRemoveFromKinCmd(m_cmdIf->handle, m_sourceInfo, axisName, cmdID ) )
  {
    // you should do are more detailled error check here
    std::cout << "Could not create RemoveFromKin command!" << std::endl;
    return false;
  }
  std::cout << "Remove axis " << axisName << " from kinematic " << std::endl;

  ::motion::core::statemachine::PlcOpenStateAxis expectedState = 
  ::motion::core::statemachine::PlcOpenStateAxis::PLC_OPEN_STATE_AXIS_STANDSTILL;
  uint32_t timeOut = 200;
  while ( timeOut > 0 )
  {
    ::motion::core::statemachine::PlcOpenStateAxis currentState;
    m_sysIf->getAxsPlcOpenState(m_sysIf->handle, axisName, currentState );
    if ( currentState == expectedState )
    {
      std::cout << "Axis " << axisName << " is in StandStill " << std::endl;
      return true;
    }
    if ( m_abortThread )
    {
      return false;
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    --timeOut;
  }

  // you should do are more detailled error check here
  std::cout << "Could not remove from kinematics!" << std::endl;
  return false;
}

bool Motion::wait4StateOfKin( const char *kinName, ::motion::core::statemachine::PlcOpenStateGroup plcOpenState)
{
  // source information should be set to get helpful error diagnosis
  m_sourceInfo.m_sourceName = __FILE__;
  m_sourceInfo.m_sourceLine = __LINE__;

  uint32_t timeOut = 200;
  while ( timeOut > 0 )
  {
    ::motion::core::statemachine::PlcOpenStateGroup currentState;
    m_sysIf->getKinPlcOpenState(m_sysIf->handle, kinName, currentState );
    if ( currentState == plcOpenState )
    {
      std::cout << "PLCopen state successfull reached" << std::endl; 
      return true;
    }
    if ( m_abortThread )
    {
      return false;
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    --timeOut;
    std::cout << "Actual PLCopen state is " << plcOpenStateToString(currentState) << std::endl; 

  }

  // you should do are more detailled error check here
  std::cout << "Could not enable kinematics!" << std::endl;
  return false;
}

void Motion::kinematicsExample( )
{
  const char *KIN_NAME = "Mover";
  std::vector<const char *> AXIS_NAMES{ "X", "Y", "Z" };
    
  if ( !kinExists( KIN_NAME ) )
  {
    if ( !switchState( "Configuration", "Configuration" ) )
    {
      return;
    }

    // create the kinematic
    if ( ::motion::core::MotionResult::STS_OK != m_sysIf->createKinematic(m_sysIf->handle, KIN_NAME ) )
    {
      std::cout << "Could not create kinematics!" << std::endl;
      return;
    }
    std::cout << "Kinematic  " << KIN_NAME << " created" << std::endl;
    // create the 3 axes for this kinematic
    if ( ::motion::core::MotionResult::STS_OK != m_sysIf->createAxes(m_sysIf->handle, AXIS_NAMES ) ) 
    {
      std::cout << "Could not create axes for the kinematics!" << std::endl;
      return;
    }
    std::cout << "Axes for kinematics created" << std::endl;
    
    // attach the 3 axes to this kinematic
    const ::motion::core::KinAxsMeaning AXES_MEANING[] = { ::motion::core::KinAxsMeaning::MAIN_AXIS_X, ::motion::core::KinAxsMeaning::MAIN_AXIS_Y, ::motion::core::KinAxsMeaning::MAIN_AXIS_Z };
    uint32_t i;
    for(i=0;i<sizeof(AXES_MEANING)/sizeof(::motion::core::KinAxsMeaning);i++)
    {
      if ( ::motion::core::MotionResult::STS_OK != m_sysIf->cfgKinematicAxis(m_sysIf->handle, KIN_NAME, AXIS_NAMES[i], AXES_MEANING[i], ::motion::core::KinAxsDir::POSITIVE ) ) 
      {
        std::cout << "Could not configure axis " << AXIS_NAMES[i] << "to the kinematics!" << std::endl;
        return;
      }
    }    
    std::cout << "Configured axes to the kinematics" << std::endl;
  
    if ( !switchState( "Running", "Booting" ) )
    {
      return;
    }
  } 
  // switch power on for all axes
  for ( const auto &axsName : AXIS_NAMES )
  {
    if ( !switchAxisPower( axsName, true ) )
    {
      return;
    }
  }
  // add all axes to the group
  for ( const auto &axsName : AXIS_NAMES )
  {
    if ( !addAxisToKin( axsName, KIN_NAME ) )
    {
      return;
    }
  }

  // enable the kinematic. PLCopen state GroupStandby 
  if ( !enableKin( KIN_NAME, true ) )
  {
    return;
  }

  // start moving the kinematic. PLCopen state GroupMoving
  if ( !bufferedKinMove( KIN_NAME ) )
  {
    return;
  }

  // Wait until all moves are finished and PLCopen state Standby is reached
  if ( !wait4StateOfKin( KIN_NAME, ::motion::core::statemachine::PlcOpenStateGroup::PLC_OPEN_STATE_GROUP_STANDBY ) )
  {
    return;
  }
  
  // disable the kinematic. PLCopen state GroupDisabled
  if ( !enableKin( KIN_NAME, false ) )
  {
    return;
  }
  // remove all axes from the group
  for ( const auto &axsName : AXIS_NAMES )
  {
    if ( !removeAxisFromKin( axsName ) )
    {
      return;
    }
  }
  // switch power off for all axes
  for ( const auto &axsName : AXIS_NAMES )
  {
    if ( !switchAxisPower( axsName, false ) )
    {
      return;
    }
  }
  // finished. All is switched off
  std::cout << "End of kinematicsExample " << std::endl;

}

bool Motion::bufferedKinMove( const char *kinName )
{
  // for kinematics all commands are executed sequentially always
  std::vector< std::array<double, 3> > targetPositions{
    { 0.1, 0.0, 0.5 },
    { 4.9, 0.0, 0.5 },
    { 4.9, 5.0, 0.5 },
    { 0.1, 5.0, 0.5 },
    { 0.1, 0.0, 0.5 },
    { 4.9, 5.0, 0.5 },
    { 2.5, 7.5, 0.5 },
    { 0.1, 5.0, 0.5 },
    { 4.9, 0.0, 0.5 },
    { 4.9, 0.0, 2.5 },
    { 0.1, 0.0, 2.5 },
    { 0.1, 5.0, 2.5 },
    { 4.9, 5.0, 2.5 },
    { 4.9, 0.0, 2.5 },
    { 0.1, 5.0, 2.5 },
    { 2.5, 7.5, 2.5 },
    { 4.9, 5.0, 2.5 },
    { 0.1, 0.0, 2.5 },
    { 5.0, 0.0, 2.5 }
  };

  for ( const auto &pos : targetPositions )
  {
    // source information should be set to get helpful error diagnosis
    m_sourceInfo.m_sourceName = __FILE__;
    m_sourceInfo.m_sourceLine = __LINE__;

    // set the dynamic limits of the movement here
    ::motion::core::cmd::DynamicLimitsT dynLim;
    dynLim.m_vel = 2.5;         // velocity limit
    dynLim.m_acc = 2.0;         // acceleration limit
    dynLim.m_dec = 2.5;         // deceleration limit
    
    double tp[::motion::core::MAX_AXES_PER_KIN]{0.0};
    for (uint32_t i=0; i<3; ++i)
    {
      tp[i] = pos[i];
    }

    uint64_t cmdID;
    if ( ::motion::core::MotionResult::STS_OK != m_cmdIf->kinMoveCmd(m_cmdIf->handle, m_sourceInfo, kinName, ::motion::core::cmd::kin::KinCmdType::KIN_CMD_MOVE_ABS, tp, ::motion::core::cmd::kin::CmdTargetCOS::PCS, dynLim, true, cmdID ) )
    {
      // you should do are more detailled error check here
      std::cout << "Could not create move command!" << std::endl;
      return false;
    }

  }
  std::cout << "Created move commands for kinematic " << kinName << std::endl;

  return true;
}

