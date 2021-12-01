#pragma once

// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include <thread>
#include "motion/common/defines/dia_defs.h"
#include "motion/core/defines/motionsystem_statusdefs.h"
#include "motion/jobs/defines/cmd_defs.h"
#include "motion/core/interface/motion_cmd_if.h"
#include "motion/core/interface/motion_sys_if.h"

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
  class Motion
  {
  private:
    const MotionCmdIfT *m_cmdIf{};    //!< motion command interface
    const MotionSysIfT *m_sysIf{};    //!< motion system interface
    std::thread *m_thread{};                  //!< main thread for this example
    bool m_abortThread{ false };              //!< abort the main thread? (on stopping the bundle)
    ::motion::core::dia::CmdSourceInfo m_sourceInfo{ ::motion::core::dia::CmdSourceType::BUNDLE_IF };   //!< source info for commands

    //! @brief function of the main thread
    void main( );

    //! @brief Little example, how to create and command an (single) axis
    void axisExample( );

    //! @brief Little example, how to create non-buffered commands for a single axis
    //! @param[in]  axisName   Name of the axis
    //! @return                Return value indicating success or error
    bool nonBufferedAxisMove( const char *axisName );

    //! @brief Little example, how to create buffered commands for a single axis
    //! @param[in]  axisName   Name of the axis
    //! @return                Return value indicating success or error
    bool bufferedAxisMove( const char *axisName );

    //! @brief helper function to switch the global motion state machine
    //! @param[in]  expected   Name of the expected state
    //! @param[in]  command    Name of the requested state (leave it out, when just waiting)
    //! @return                Return value indicating success or error
    bool switchState( const char *expected, const char *command = nullptr );

    //! @brief helper function to switch on/off power of a single axis
    //! @param[in]  axisName   Name of the axis
    //! @param[in]  powerOn    set to true, to power on
    //! @return                Return value indicating success or error
    bool switchAxisPower( const char *axisName, bool powerOn );

    //! @brief helper function to create a PosAbs command for a single axis
    //! @param[in]  axisName   Name of the axis
    //! @param[in]  targetPos  target position of the movement
    //! @param[in]  buffered   create a buffered command?
    //! @return                Return value indicating success or error
    bool moveAxis( const char *axisName, const double targetPos, bool buffered = false );

    //! @brief helper function to wait until a single axis arrives at the defined position
    //! @param[in]  axisName   Name of the axis
    //! @param[in]  targetPos  target position of the movement
    //! @return                Return value indicating success or error
    bool waitForAxisPos( const char *axisName, const double targetPos );
    
    //! @brief check if an axis exists
    //! @param[in]  axisName   Name of the axis
    //! @return                true, when the axis exists
    bool axisExists( const char *axisName );

    //! @brief Little example, how to create and command a kinematic
    void kinematicsExample( );

    //! @brief enable or disable a kinematic
    //! @param[in]  kinName    Name of the kinematic
    //! @param[in]  doEnable   true = enable (group), false = disable (un-group)
    //! @return                Return value indicating success or error
    bool enableKin( const char *kinName, bool doEnable );

    //! @brief kinematic moves in buffered mode
    //! @param[in]  kinName    Name of the kinematic
    //! @return                Return value indicating success or error
    bool bufferedKinMove( const char *kinName );

    //! @brief add one axis to a kinematic
    //! @param[in]  axsName    Name of the axis
    //! @param[in]  kinName    Name of the kinematic
    //! @return                Return value indicating success or error
    bool addAxisToKin( const char *axsName, const char *kinName );

    //! @brief remove one axis from a kinematic
    //! @param[in]  axsName    Name of the axis
    //! @return                Return value indicating success or error
    bool removeAxisFromKin( const char *axsName );

    //! @brief check if a kinematic exists
    //! @param[in]  kinName    Name of the kinematic
    //! @return                Return value indicating success or error
    bool kinExists( const char *kinName );

    //! @brief wait until a kinematic has reached a given state
    //! @param[in]  kinName      Name of the kinematic
    //! @param[in]  plcOpenState State to wait for
    //! @return                  Return value indicating success or error
    bool wait4StateOfKin( const char *kinName, ::motion::core::statemachine::PlcOpenStateGroup plcOpenState);

  public:
    //! @brief Constructor
    Motion() = default;
    //! @brief Destructor
    virtual ~Motion() = default;


    //! This method returns the class name.
    //!
    //! @return                      Return class name.
    static const char* getClassName(void) { return (CLASS_NAME(Motion)); }

    //! @brief starts the motion example
    //! @param[in]  cmdIf   pointer to the motion command interface
    //! @param[in]  sysIf   pointer to the motion system interface
    void start(const MotionCmdIfT *cmdIf, const MotionSysIfT *sysIf);

    //! @brief stops the motion example
    void stop();

  };
}
