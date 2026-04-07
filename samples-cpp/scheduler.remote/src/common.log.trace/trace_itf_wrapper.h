#pragma once

//! @copyright (C) 2024-20xx Bosch Rexroth AG
//!
//! The reproduction, distribution and utilization of this file as well as the communication of its contents to others
//! without express authorization is prohibited. Offenders will be held liable for the payment of damages.
//! All rights reserved in the event of the grant of a patent, utility model or design.

//! @file trace_itf_wrapper.h
//! @brief This public header and the corresponding cpp source file are wrapping up the trace public interfaces to macros for a easy to use trace interface.
//! @details
//! This wrapper provides macros for:
//! - Creating a global extern object for trace connect
//! - Creating global extern objects for trace instances
//! - Creating global extern objects for trace units or indexed trace units
//! - Setting trace logs
//! .
//! @attention Only the macros for setting trace logs does comply to real time requirements, all others not!
//!
//! @section section_WrapperOverview Overview
//! This header gives the possibility for an easy to use interface of the trace by only using the macros defined in this wrapper.\n
//! It is divided into two use cases which can also be used side by side: standard trace units and/or multiple indexed trace units and a common part which is necessary for all use cases:
//! - Common: Defining and creating needed common objects and register trace units at the trace bundle -> see declaration of macros of common part.
//! - Standard trace units: Defining and creating one or more trace units at compile time and using them (set logs via them) at runtime -> see declaration of macros of standard trace units.
//! - Multiple indexed trace units: Defining and creating multiple indexed trace units at compile time, registering them with the at runtime gotten unit name at the trace bundle and using them (set logs via them) at runtime (e.g. multiple object instances like EtherCAT master) -> see declaration of macros of multiple indexed trace units.
//! .
//! For the better understanding here are some definitions and explanations:
//! - The TraceConnect object (created by @ref DECLARATION_TRACE_CONNECT / @ref DEFINITION_TRACE_CONNECT) acts as a singleton and is responsible for the connection of the trace bundle registration pointer and the management of all trace instances (attach/detach) and all trace units (register/unregister). Can be created only once.
//! - A TraceInstance object (created by @ref DECLARATION_TRACE_INSTANCE / @ref DEFINITION_TRACE_INSTANCE) is a collection of one or more trace units with the same main diagnosis codes and is responsible for the management of these trace units (register/unregister). Can be created multiple times with different identifiers and diagnosis codes.
//! - A TraceUnit object (created by @ref DECLARATION_TRACE_UNIT / @ref DEFINITION_TRACE_UNIT) represents one single trace unit including buffered trace and enabling states. Can be created multiple times with different identifiers and unit names. Unit names have to be unique over all units and instances!
//! - A TraceUnitIndexed object (created by @ref DECLARATION_TRACE_UNIT_INDEXED / @ref DEFINITION_TRACE_UNIT_INDEXED) represents multiple indexed trace units including buffered trace and enabling states. Should be created only once with the at runtime expected amount of trace units.
//! .
//! The documentation of the macros has only the absolutely necessary scope. For a detailed documentation of the trace interface which is used here, please have a look at the files defs.h, i_log_real_time3.h, i_registration_real_time3.h and the other ones in public/include/common/log/trace folder.
//!
//! @section section_Usage Usage
//! To use this interface please follow these steps:
//! - Add "common.log.trace/public/src" to your include directories and compile trace_itf_wrapper.cpp within your project by adding it to the source files.
//! - For your personal trace implementation create a header and CPP file like trace_itf_wrapper_template.h/.cpp from "public/src" folder using them as template or just copy and adapt it according to your personal demands. See and follow the comments in these files for an easy to use trace implementation.
//! - For setting traces this header only provides the most common used macros with passing ellipse ("..."). If you miss the macros with variable arguments ("va_list") or those with return value, you can define your own macros in your personal trace implementation header file. See also i_log_real_time3.h.


// ---------------------------------------------------------------------------
// Include files
// ---------------------------------------------------------------------------

#include <cstdint>
#include <string>
#include <vector>
#include "common/log/trace/defs.h"
#include "common/log/trace/i_registration_real_time3.h"
#include "common/log/trace/i_log_real_time3.h"
#include "common/log/trace/log_buffered3.h"


// ---------------------------------------------------------------------------
// Declaration of macros of common part
// ---------------------------------------------------------------------------

//! @brief This macro declares the global extern object of class @ref common::log::trace::TraceConnect.
//!
//! Usage: Header file of the trace implementation of the component. Can be declared only once!
#define DECLARATION_TRACE_CONNECT() \
  namespace TRACE_NAMESPACE \
  { \
    extern common::log::trace::TraceConnect& g_TRACECONNECT; \
  }

//! @brief This macro defines and creates the global extern object of class @ref common::log::trace::TraceConnect.
//!
//! Usage: CPP file of the trace implementation of the component. Can be defined and created only once!
#define DEFINITION_TRACE_CONNECT() \
  namespace TRACE_NAMESPACE \
  { \
    common::log::trace::TraceConnect& g_TRACECONNECT = common::log::trace::TraceConnect::singleton(); \
  }

//! @brief This macro declares a global extern object of class @ref common::log::trace::TraceInstance.
//!
//! @param[in]    instanceIdentifier   Type: LABEL. Instance identifier which acts also as object name.
//!
//! Usage: Header file of the trace implementation of the component. Can be declared multiple times with different identifiers.
#define DECLARATION_TRACE_INSTANCE(instanceIdentifier) \
  namespace TRACE_NAMESPACE \
  { \
    extern common::log::trace::TraceInstance g_TRACEINSTANCE##instanceIdentifier; \
  }

//! @brief This macro declares, defines and creates a global extern object of class @ref common::log::trace::TraceInstance. Use this macro in case of dynamic origin in combination with @ref SET_TRACE_ORIGIN.
//!
//! @param[in]    instanceIdentifier   Type: LABEL. Instance identifier which acts also as object name.
//! @param[in]    infoCode             Type: std::uint32_t. Main diagnosis code defined in the diagnosis database used for trace infos/messages.
//! @param[in]    warningCode          Type: std::uint32_t. Main diagnosis code defined in the diagnosis database used for trace warnings.
//! @param[in]    errorCode            Type: std::uint32_t. Main diagnosis code defined in the diagnosis database used for trace errors.
//! @param[in]    baseEntity           Type: std::string. Base entity used for trace logs if no special entity is given.
//!
//! Usage: CPP file of the trace implementation of the component. Can be defined and created multiple times with different identifiers and diagnosis codes.
#define DEFINITION_TRACE_INSTANCE(instanceIdentifier, infoCode, warningCode, errorCode, baseEntity) \
  namespace TRACE_NAMESPACE \
  { \
    common::log::trace::TraceInstance g_TRACEINSTANCE##instanceIdentifier = common::log::trace::TraceInstance(TRACE_NAMESPACE::g_TRACECONNECT, infoCode, warningCode, errorCode, baseEntity); \
  }

//! @brief This macro declares, defines and creates a global extern object of class @ref common::log::trace::TraceInstance. Use this macro in case of static origin. For a variable origin (e.g. bundle is used in different apps) use @ref DEFINITION_TRACE_INSTANCE in combination with @ref SET_TRACE_ORIGIN.
//!
//! @param[in]    instanceIdentifier   Type: LABEL. Instance identifier which acts also as object name.
//! @param[in]    infoCode             Type: std::uint32_t. Main diagnosis code defined in the diagnosis database used for trace infos/messages.
//! @param[in]    warningCode          Type: std::uint32_t. Main diagnosis code defined in the diagnosis database used for trace warnings.
//! @param[in]    errorCode            Type: std::uint32_t. Main diagnosis code defined in the diagnosis database used for trace errors.
//! @param[in]    baseEntity           Type: std::string. Base entity used for trace logs if no special entity is given.
//! @param[in]    origin               Type: std::string. Origin for the trace logs. Use the app title (e.g. "Automation Core" or "Motion") of your app.
//!
//! Usage: CPP file of the trace implementation of the component. Can be defined and created multiple times with different identifiers and diagnosis codes.
#define DEFINITION_TRACE_INSTANCE_ORIGIN(instanceIdentifier, infoCode, warningCode, errorCode, baseEntity, origin) \
  namespace TRACE_NAMESPACE \
  { \
    common::log::trace::TraceInstance g_TRACEINSTANCE##instanceIdentifier = common::log::trace::TraceInstance(TRACE_NAMESPACE::g_TRACECONNECT, infoCode, warningCode, errorCode, baseEntity, origin); \
  }

//! @brief This macro attaches all trace units to the trace bundle by registering all defined trace units of all defined trace instances.
//!
//! @param[out]   returnValue           Type: common::log::trace::TraceResult. Return value of method @ref common::log::trace::TraceConnect::attachTrace.
//! @param[in]    registrationPointer   Type: common::log::trace::IRegistrationRealTime3*. Pointer to the registration interface which is gotten from the trace bundle.
//! @retval       TraceResult::UNIT_ALREADY_AVAILABLE   If a trace unit with the same name was already registered.
//! @retval       TraceResult::UNIT_NAME_INVALID        If the given trace unit name is invalid.
//!
//! Usage: In the implementation of the initialization process of the component when the trace bundle becomes valid through Celix framework (registered callback in activator).
#define ATTACH_TRACE(returnValue, registrationPointer) \
  returnValue = TRACE_NAMESPACE::g_TRACECONNECT.attachTrace(registrationPointer)

//! @brief This macro detaches all trace units from the trace bundle by unregistering all defined trace units of all defined trace instances.
//!
//! @param[out]   returnValue   Type: common::log::trace::TraceResult. Return value of method @ref common::log::trace::TraceConnect::detachTrace.
//! @retval       TraceResult::UNIT_NOT_AVAILABLE   If a trace unit with the given name was not already registered.
//!
//! Usage: In the implementation of the deinitialization process of the component when the trace bundle becomes invalid through Celix framework (registered callback in activator).
#define DETACH_TRACE(returnValue) \
  returnValue = TRACE_NAMESPACE::g_TRACECONNECT.detachTrace()

//! @brief This macro sets the origin that is used for all trace logs in case of variable origin (e.g. bundle is used in different apps). Use this macro in combination with @ref DEFINITION_TRACE_INSTANCE. If the origin is a static value, use @ref DEFINITION_TRACE_INSTANCE_ORIGIN.
//!
//! @param[in]    origin   Origin as app title (e.g. "Automation Core" or "Motion").
//!
//! Usage: In the implementation of the initialization process of the Celix framework when the config.properties value of the key "app.snap.title" is read.
#define SET_TRACE_ORIGIN(origin) \
  TRACE_NAMESPACE::g_TRACECONNECT.setOrigin(origin);

//! @brief This macro disables all buffered trace units (standard and indexed) of all instances which could not be registered.
//!
//! Usage: In the implementation of the initialization process of the component when ATTACH_TRACE failed to ensure real time behavior.
#define DISABLE_ALL_BUFFERED_TRACES() \
  TRACE_NAMESPACE::g_TRACECONNECT.disableAllBufferedTraceUnits()


// ---------------------------------------------------------------------------
// Declaration of macros of standard trace units
// ---------------------------------------------------------------------------

//! @brief This macro declares a global extern object of class @ref common::log::trace::TraceUnit.
//!
//! @param[in]    unitIdentifier   Type: LABEL. Unit identifier which acts also as object name. Name has to be unique over all units and instances!
//!
//! Usage: Header file of the trace implementation of the component. Can be declared multiple times with different identifiers. Unit names have to be unique over all units and instances!
#define DECLARATION_TRACE_UNIT(unitIdentifier) \
  namespace TRACE_NAMESPACE \
  { \
    extern common::log::trace::TraceUnit g_TRACEUNIT##unitIdentifier; \
  }

//! @brief This macro defines and creates a global extern object of class @ref common::log::trace::TraceUnit.
//!
//! @param[in]    instanceIdentifier     Type: LABEL. Instance identifier and reference to @ref common::log::trace::TraceInstance object to be able to use member variables of the trace instance e.g. main diagnosis code.
//! @param[in]    unitIdentifier         Type: LABEL. Unit identifier which acts also as object name. Name has to be unique over all units and instances!
//! @param[in]    unitName               Type: std::string. Specific name of the trace unit. The maximum length of the name is @ref common::log::trace::MAXIMUM_LENGTH_UNIT_NAME. Longer names are truncated.
//! @param[in]    initialEnablingState   Type: common::log::trace::EnablingState. Initial enabling state for infos/messages, warnings and errors.
//!
//! Usage: CPP file of the trace implementation of the component. Can be defined and created multiple times with different identifiers and unit names. Unit names have to be unique over all units and instances!
#define DEFINITION_TRACE_UNIT(instanceIdentifier, unitIdentifier, unitName, initialEnablingState) \
  namespace TRACE_NAMESPACE \
  { \
    common::log::trace::TraceUnit g_TRACEUNIT##unitIdentifier = common::log::trace::TraceUnit(g_TRACEINSTANCE##instanceIdentifier, unitName, initialEnablingState); \
  }

//! @brief This macro gets the log pointer of the given unit object. If the buffered trace is active, then its log pointer is gotten, otherwise the log pointer of the registered trace unit.
//!
//! @param[in]    unitIdentifier   Type: LABEL. Unit identifier the trace unit log pointer is gotten for.
//! @return       Type: ILogRealTime3*. Log pointer of the given unit identifier.
//!
//! Usage: Code location in the component where the log pointer is needed.
#define GET_TRACE_UNIT_LOG_POINTER(unitIdentifier) \
  TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceContext

//! @brief This macro gets the trace unit name of the given unit object.
//!
//! @param[in]    unitIdentifier   Type: LABEL. Unit identifier the trace unit name is gotten for.
//! @return       Type: char*. Unit name of the given unit identifier.
//!
//! Usage: Code location in the component where the trace unit name is needed.
#define GET_TRACE_UNIT_NAME(unitIdentifier) \
  TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_unitName.c_str()

//! @brief This macro gets the enabling state of the given unit object. If the buffered trace is active, then its enabling state is gotten, otherwise the enabling state of the registered trace unit.
//!
//! @param[in]    unitIdentifier   Type: LABEL. Unit identifier the trace enabling state is gotten for.
//! @return       Type: common::log::trace::EnablingState&. Enabling state of the given unit identifier.
//!
//! Usage: Code location in the component where the trace enabling state is needed.
#define GET_TRACE_ENABLING_STATE(unitIdentifier) \
  TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.getEnablingState()

//! @brief This macro sets an additional callback function when the enabling states are updated. Note: This is only necessary if an update of the enabling states of a trace unit needs to be considered outside the wrapper interface. The enabling states of the trace unit itself are handled internally.
//!
//! @param[in]    unitIdentifier   Type: LABEL. Unit identifier the enabling states are relevant for.
//! @param[in]    callback         Type: const std::function<void(const EnablingState&)>. Callback function that is called when the trace unit enabling states are changed.
//!
//! Usage: Code location in the component when the trace unit enabling states need to be considered outside the wrapper interface.
#define SET_TRACE_UNIT_ENABLING_STATE_CALLBACK(unitIdentifier, callback) \
  TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.setAdditionalEnablingStateCallback(callback);

//! @brief This macro sets the buffered trace enabling state for the given unit object.
//!
//! @param[in]    unitIdentifier               Type: LABEL. Unit identifier the buffered trace enabling state is set for.
//! @param[in]    bufferedTraceEnablingState   Type: common::log::trace::EnablingState. Buffered trace enabling state for infos/messages, warnings and errors to set.
//!
//! Usage: In the implementation of the initialization process of the component when the trace bundle becomes valid through Celix framework (registered callback in activator): e.g. for ensuring real time requirements when trace bundle is not required and registration at it failed.
#define SET_BUFFERED_TRACE_ENABLING_STATE(unitIdentifier, bufferedTraceEnablingState) \
  TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_bufferedTraceEnablingState = bufferedTraceEnablingState;

//! @brief This macro sets a trace log error for the defined trace unit with the defined trace instance main diagnosis code and the base entity of this trace instance.
//!
//! @param[in]    unitIdentifier   Type: LABEL. Unit identifier the trace log is sent for.
//! @param[in]    message          Type: char*. Dynamic trace log inclusive variables to print out. This argument is optional, it can be an empty string if it is not available.
//!
//! Usage: Code location in the component where the trace log should be set.
#define TRACE_ERROR(unitIdentifier, message, ...) \
  if (TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.getEnablingState().errors) \
  { \
    COMMON_LOG_TRACE_REAL_TIME_LOG3(TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceContext, TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_errorCode, TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_baseEntity.c_str(), TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_origin.c_str(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, message, ##__VA_ARGS__); \
  }

//! @brief This macro sets a trace log error for the defined trace unit with the defined trace instance main diagnosis code and the given entity.
//!
//! @param[in]    unitIdentifier   Type: LABEL. Unit identifier the trace log is sent for.
//! @param[in]    entity           Type: char*. Data Layer path if available (recommended). If not, the system URI of the depending object. This argument is optional, it can be an empty string if it is not available. The maximum length is @ref common::log::trace::MAXIMUM_LENGTH_ENTITY. Longer entities are truncated.
//! @param[in]    message          Type: char*. Dynamic trace log inclusive variables to print out. This argument is optional, it can be an empty string if it is not available.
//!
//! Usage: Code location in the component where the trace log should be set.
#define TRACE_ERROR_ENTITY(unitIdentifier, entity, message, ...) \
  if (TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.getEnablingState().errors) \
  { \
    COMMON_LOG_TRACE_REAL_TIME_LOG3(TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceContext, TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_errorCode, entity, TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_origin.c_str(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, message, ##__VA_ARGS__); \
  }

//! @brief This macro sets a trace log warning for the defined trace unit with the defined trace instance main diagnosis code and the base entity of this trace instance.
//!
//! @param[in]    unitIdentifier   Type: LABEL. Unit identifier the trace log is sent for.
//! @param[in]    message          Type: char*. Dynamic trace log inclusive variables to print out. This argument is optional, it can be an empty string if it is not available.
//!
//! Usage: Code location in the component where the trace log should be set.
#define TRACE_WARNING(unitIdentifier, message, ...) \
  if (TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.getEnablingState().warnings) \
  { \
    COMMON_LOG_TRACE_REAL_TIME_LOG3(TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceContext, TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_warningCode, TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_baseEntity.c_str(), TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_origin.c_str(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, message, ##__VA_ARGS__); \
  }

//! @brief This macro sets a trace log warning for the defined trace unit with the defined trace instance main diagnosis code and the given entity.
//!
//! @param[in]    unitIdentifier   Type: LABEL. Unit identifier the trace log is sent for.
//! @param[in]    entity           Type: char*. Data Layer path if available (recommended). If not, the system URI of the depending object. This argument is optional, it can be an empty string if it is not available. The maximum length is @ref common::log::trace::MAXIMUM_LENGTH_ENTITY. Longer entities are truncated.
//! @param[in]    message          Type: char*. Dynamic trace log inclusive variables to print out. This argument is optional, it can be an empty string if it is not available.
//!
//! Usage: Code location in the component where the trace log should be set.
#define TRACE_WARNING_ENTITY(unitIdentifier, entity, message, ...) \
  if (TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.getEnablingState().warnings) \
  { \
    COMMON_LOG_TRACE_REAL_TIME_LOG3(TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceContext, TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_warningCode, entity, TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_origin.c_str(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, message, ##__VA_ARGS__); \
  }

//! @brief This macro sets a trace log info/message for the defined trace unit with the defined trace instance main diagnosis code and the base entity of this trace instance.
//!
//! @param[in]    unitIdentifier   Type: LABEL. Unit identifier the trace log is sent for.
//! @param[in]    message          Type: char*. Dynamic trace log inclusive variables to print out. This argument is optional, it can be an empty string if it is not available.
//!
//! Usage: Code location in the component where the trace log should be set.
#define TRACE_INFO(unitIdentifier, message, ...) \
  if (TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.getEnablingState().messages) \
  { \
    COMMON_LOG_TRACE_REAL_TIME_LOG3(TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceContext, TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_infoCode, TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_baseEntity.c_str(), TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_origin.c_str(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, message, ##__VA_ARGS__); \
  }

//! @brief This macro sets a trace log info/message for the defined trace unit with the defined trace instance main diagnosis code and the given entity.
//!
//! @param[in]    unitIdentifier   Type: LABEL. Unit identifier the trace log is sent for.
//! @param[in]    entity           Type: char*. Data Layer path if available (recommended). If not, the system URI of the depending object. This argument is optional, it can be an empty string if it is not available. The maximum length is @ref common::log::trace::MAXIMUM_LENGTH_ENTITY. Longer entities are truncated.
//! @param[in]    message          Type: char*. Dynamic trace log inclusive variables to print out. This argument is optional, it can be an empty string if it is not available.
//!
//! Usage: Code location in the component where the trace log should be set.
#define TRACE_INFO_ENTITY(unitIdentifier, entity, message, ...) \
  if (TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.getEnablingState().messages) \
  { \
    COMMON_LOG_TRACE_REAL_TIME_LOG3(TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceContext, TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_infoCode, entity, TRACE_NAMESPACE::g_TRACEUNIT##unitIdentifier.m_traceInstance.m_origin.c_str(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, message, ##__VA_ARGS__); \
  }


// ---------------------------------------------------------------------------
// Declaration of macros of multiple indexed trace units
// ---------------------------------------------------------------------------

//! @brief This macro declares a global extern object of class @ref common::log::trace::TraceUnitIndexed.
//!
//! @param[in]    unitIndexedIdentifier   Type: LABEL. Indexed unit identifier which acts also as object name.
//!
//! Usage: Header file of the trace implementation of the component. Should be declared only once.
#define DECLARATION_TRACE_UNIT_INDEXED(unitIndexedIdentifier) \
  namespace TRACE_NAMESPACE \
  { \
    extern common::log::trace::TraceUnitIndexed g_TRACEUNITIDX##unitIndexedIdentifier; \
  }

//! @brief This macro defines and creates a global extern object of class @ref common::log::trace::TraceUnitIndexed which defines and creates the amount of trace units.
//!
//! @param[in]    instanceIdentifier      Type: LABEL. Instance identifier and reference to @ref common::log::trace::TraceInstance object to be able to use member variables of the trace instance e.g. main diagnosis code.
//! @param[in]    unitIndexedIdentifier   Type: LABEL. Indexed unit identifier which acts also as object name.
//! @param[in]    amount                  Type: std::uint32_t. Amount of trace units to create.
//! @param[in]    initialEnablingState    Type: common::log::trace::EnablingState. Initial enabling state for infos/messages, warnings and errors.
//!
//! Usage: CPP file of the trace implementation of the component. Should be defined and created only once with the at runtime expected amount of trace units.
#define DEFINITION_TRACE_UNIT_INDEXED(instanceIdentifier, unitIndexedIdentifier, amount, initialEnablingState) \
  namespace TRACE_NAMESPACE \
  { \
    common::log::trace::TraceUnitIndexed g_TRACEUNITIDX##unitIndexedIdentifier = common::log::trace::TraceUnitIndexed(g_TRACEINSTANCE##instanceIdentifier, amount, initialEnablingState); \
  }

//! @brief This macro sets the unit name of the defined indexed trace unit and registers the trace unit to the trace bundle.
//!
//! @param[out]   returnValue             Type: common::log::trace::TraceResult. Return value of method @ref common::log::trace::TraceUnitIndexed::registerTraceUnitIndexed.
//! @param[in]    unitIndexedIdentifier   Type: LABEL. Indexed unit identifier of the trace unit to set the name and to register.
//! @param[in]    index                   Type: std::uint32_t. Index of the trace unit to set the name and to register. Range: 1 to amout of trace unit indexed.
//! @param[in]    unitName                Type: std::string. Specific name of the trace unit to set. The maximum length of the name is @ref common::log::trace::MAXIMUM_LENGTH_UNIT_NAME. Longer names are truncated.
//! @retval       TraceResult::UNIT_ALREADY_AVAILABLE   If a trace unit with the same name was already registered.
//! @retval       TraceResult::UNIT_NAME_INVALID        If the given trace unit name is invalid.
//!
//! Usage: In the implementation of the component when the trace bundle is valid through Celix framework and the specific trace unit name becomes valid.
#define REGISTER_TRACE_UNIT_INDEXED(returnValue, unitIndexedIdentifier, index, unitName) \
  returnValue = TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.registerTraceUnitIndexed(index, unitName)

//! @brief This macro unregisters the defined indexed trace unit from the trace bundle.
//!
//! @param[out]   returnValue             Type: common::log::trace::TraceResult. Return value of method @ref common::log::trace::TraceUnitIndexed::unregisterTraceUnitIndexed.
//! @param[in]    unitIndexedIdentifier   Type: LABEL. Indexed unit identifier of the trace unit to unregister.
//! @param[in]    index                   Type: std::uint32_t. Index of the trace unit to unregister. Range: 1 to amout of trace unit indexed.
//! @retval       TraceResult::UNIT_NOT_AVAILABLE   If a trace unit with the given name was not already registered.
//!
//! Usage: In the implementation of the component when the trace bundle becomes invalid through Celix framework or when the specific trace unit name becomes invalid.
#define UNREGISTER_TRACE_UNIT_INDEXED(returnValue, unitIndexedIdentifier, index) \
  returnValue = TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.unregisterTraceUnitIndexed(index)

//! @brief This macro gets the trace unit name of the given indexed unit object.
//!
//! @param[in]    unitIndexedIdentifier   Type: LABEL. Indexed unit identifier the trace name is gotten for.
//! @param[in]    index                   Type: std::uint32_t. Index of the indexed unit object the trace name is gotten for. Range: 1 to amout of trace unit indexed.
//! @return       Type: char*. Unit name of the given indexed unit identifier.
//!
//! Usage: Code location in the component where the indexed trace unit name is needed.
#define GET_TRACE_UNIT_INDEXED_NAME(unitIndexedIdentifier, index) \
  (0 < index && TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_amount >= index) ? TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->m_unitName.c_str() : ""

//! @brief This macro gets the enabling state of the given indexed unit object. If the buffered trace is active, then its enabling state is gotten, otherwise the enabling state of the registered trace unit.
//!
//! @param[in]    unitIndexedIdentifier   Type: LABEL. Indexed unit identifier the trace enabling state is gotten for.
//! @param[in]    index                   Type: std::uint32_t. Index of the indexed unit object the trace enabling state is gotten for. Range: 1 to amout of trace unit indexed.
//! @return       Type: common::log::trace::EnablingState&. Enabling state of the given indexed unit identifier.
//!
//! Usage: Code location in the component where the indexed trace enabling state is needed.
#define GET_TRACE_INDEXED_ENABLING_STATE(unitIndexedIdentifier, index) \
  TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->getEnablingState()

//! @brief This macro sets the buffered trace enabling state for the given indexed unit object (e.g. for ensuring real time requirements when trace bundle is not required and registration at it failed).
//!
//! @param[in]    unitIndexedIdentifier        Type: LABEL. Indexed unit identifier the buffered trace enabling state is set for.
//! @param[in]    index                        Type: std::uint32_t. Index of the indexed unit object the buffered trace enabling state is set for. Range: 1 to amout of trace unit indexed.
//! @param[in]    bufferedTraceEnablingState   Type: common::log::trace::EnablingState. Buffered trace enabling state for infos/messages, warnings and errors to set.
//!
//! Usage: In the implementation of the component when the trace bundle is invalid through Celix framework and the specific trace unit name becomes valid: e.g. for ensuring real time requirements when trace bundle is not required and registration at it failed.
#define SET_BUFFERED_TRACE_INDEXED_ENABLING_STATE(unitIndexedIdentifier, index, bufferedTraceEnablingState) \
  if (0 < index && TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_amount >= index) \
  { \
    TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->m_bufferedTraceEnablingState = bufferedTraceEnablingState; \
  }

//! @brief This macro disables all indexed buffered trace units which could not be registered.
//!
//! @param[in]    unitIndexedIdentifier   Type: LABEL. Indexed unit identifier the buffered traces are disabled for.
//!
//! Usage: In the implementation of the initialization process of the component when ATTACH_TRACE failed to ensure real time behavior.
#define DISABLE_ALL_INDEXED_BUFFERED_TRACES(unitIndexedIdentifier) \
  TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.disableAllBufferedTraceUnitsIndexed()

//! @brief This macro sets a trace log error for the defined indexed trace unit with the defined trace instance main diagnosis code and the base entity of this trace instance.
//!
//! @param[in]    unitIndexedIdentifier   Type: LABEL. Indexed unit identifier the trace log is sent for.
//! @param[in]    index                   Type: std::uint32_t. Index of the indexed unit object the trace log is sent for. Range: 1 to amout of trace unit indexed.
//! @param[in]    message                 Type: char*. Dynamic trace log inclusive variables to print out. This argument is optional, it can be an empty string if it is not available.
//!
//! Usage: Code location in the component where the trace log should be set.
#define TRACE_ERROR_INDEXED(unitIndexedIdentifier, index, message, ...) \
  if (0 < index && TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_amount >= index) \
  { \
    if (TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->getEnablingState().errors) \
    { \
      COMMON_LOG_TRACE_REAL_TIME_LOG3(TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->m_traceContext, TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_errorCode, TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_baseEntity.c_str(), TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_origin.c_str(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, message, ##__VA_ARGS__); \
    } \
  }

//! @brief This macro sets a trace log error for the defined indexed trace unit with the defined trace instance main diagnosis code and the given entity.
//!
//! @param[in]    unitIndexedIdentifier   Type: LABEL. Indexed unit identifier the trace log is sent for.
//! @param[in]    index                   Type: std::uint32_t. Index of the indexed unit object the trace log is sent for. Range: 1 to amout of trace unit indexed.
//! @param[in]    entity                  Type: char*. Data Layer path if available (recommended). If not, the system URI of the depending object. This argument is optional, it can be an empty string if it is not available. The maximum length is @ref common::log::trace::MAXIMUM_LENGTH_ENTITY. Longer entities are truncated.
//! @param[in]    message                 Type: char*. Dynamic trace log inclusive variables to print out. This argument is optional, it can be an empty string if it is not available.
//!
//! Usage: Code location in the component where the trace log should be set.
#define TRACE_ERROR_INDEXED_ENTITY(unitIndexedIdentifier, index, entity, message, ...) \
  if (0 < index && TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_amount >= index) \
  { \
    if (TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->getEnablingState().errors) \
    { \
      COMMON_LOG_TRACE_REAL_TIME_LOG3(TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->m_traceContext, TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_errorCode, entity, TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_origin.c_str(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, message, ##__VA_ARGS__); \
    } \
  }

//! @brief This macro sets a trace log warning for the defined indexed trace unit with the defined trace instance main diagnosis code and the base entity of this trace instance.
//!
//! @param[in]    unitIndexedIdentifier   Type: LABEL. Indexed unit identifier the trace log is sent for.
//! @param[in]    index                   Type: std::uint32_t. Index of the indexed unit object the trace log is sent for. Range: 1 to amout of trace unit indexed.
//! @param[in]    message                 Type: char*. Dynamic trace log inclusive variables to print out. This argument is optional, it can be an empty string if it is not available.
//!
//! Usage: Code location in the component where the trace log should be set.
#define TRACE_WARNING_INDEXED(unitIndexedIdentifier, index, message, ...) \
  if (0 < index && TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_amount >= index) \
  { \
    if (TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->getEnablingState().warnings) \
    { \
      COMMON_LOG_TRACE_REAL_TIME_LOG3(TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->m_traceContext, TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_warningCode, TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_baseEntity.c_str(), TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_origin.c_str(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, message, ##__VA_ARGS__); \
    } \
  }

//! @brief This macro sets a trace log warning for the defined indexed trace unit with the defined trace instance main diagnosis code and the given entity.
//!
//! @param[in]    unitIndexedIdentifier   Type: LABEL. Indexed unit identifier the trace log is sent for.
//! @param[in]    index                   Type: std::uint32_t. Index of the indexed unit object the trace log is sent for. Range: 1 to amout of trace unit indexed.
//! @param[in]    entity                  Type: char*. Data Layer path if available (recommended). If not, the system URI of the depending object. This argument is optional, it can be an empty string if it is not available. The maximum length is @ref common::log::trace::MAXIMUM_LENGTH_ENTITY. Longer entities are truncated.
//! @param[in]    message                 Type: char*. Dynamic trace log inclusive variables to print out. This argument is optional, it can be an empty string if it is not available.
//!
//! Usage: Code location in the component where the trace log should be set.
#define TRACE_WARNING_INDEXED_ENTITY(unitIndexedIdentifier, index, entity, message, ...) \
  if (0 < index && TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_amount >= index) \
  { \
    if (TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->getEnablingState().warnings) \
    { \
      COMMON_LOG_TRACE_REAL_TIME_LOG3(TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->m_traceContext, TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_warningCode, entity, TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_origin.c_str(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, message, ##__VA_ARGS__); \
    } \
  }

//! @brief This macro sets a trace log info/message for the defined indexed trace unit with the defined trace instance main diagnosis code and the base entity of this trace instance.
//!
//! @param[in]    unitIndexedIdentifier   Type: LABEL. Indexed unit identifier the trace log is sent for.
//! @param[in]    index                   Type: std::uint32_t. Index of the indexed unit object the trace log is sent for. Range: 1 to amout of trace unit indexed.
//! @param[in]    message                 Type: char*. Dynamic trace log inclusive variables to print out. This argument is optional, it can be an empty string if it is not available.
//!
//! Usage: Code location in the component where the trace log should be set.
#define TRACE_INFO_INDEXED(unitIndexedIdentifier, index, message, ...) \
  if (0 < index && TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_amount >= index) \
  { \
    if (TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->getEnablingState().messages) \
    { \
      COMMON_LOG_TRACE_REAL_TIME_LOG3(TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->m_traceContext, TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_infoCode, TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_baseEntity.c_str(), TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_origin.c_str(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, message, ##__VA_ARGS__); \
    } \
  }

//! @brief This macro sets a trace log info/message for the defined indexed trace unit with the defined trace instance main diagnosis code and the given entity.
//!
//! @param[in]    unitIndexedIdentifier   Type: LABEL. Indexed unit identifier the trace log is sent for.
//! @param[in]    index                   Type: std::uint32_t. Index of the indexed unit object the trace log is sent for. Range: 1 to amout of trace unit indexed.
//! @param[in]    entity                  Type: char*. Data Layer path if available (recommended). If not, the system URI of the depending object. This argument is optional, it can be an empty string if it is not available. The maximum length is @ref common::log::trace::MAXIMUM_LENGTH_ENTITY. Longer entities are truncated.
//! @param[in]    message                 Type: char*. Dynamic trace log inclusive variables to print out. This argument is optional, it can be an empty string if it is not available.
//!
//! Usage: Code location in the component where the trace log should be set.
#define TRACE_INFO_INDEXED_ENTITY(unitIndexedIdentifier, index, entity, message, ...) \
  if (0 < index && TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_amount >= index) \
  { \
    if (TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->getEnablingState().messages) \
    { \
      COMMON_LOG_TRACE_REAL_TIME_LOG3(TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceUnitsIndexed[index - 1]->m_traceContext, TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_infoCode, entity, TRACE_NAMESPACE::g_TRACEUNITIDX##unitIndexedIdentifier.m_traceInstance.m_origin.c_str(), common::log::trace::getFileName(__FILE__), __FUNCTION__, __LINE__, message, ##__VA_ARGS__); \
    } \
  }


// ---------------------------------------------------------------------------
// Declaration of namespaces
// ---------------------------------------------------------------------------

namespace common
{
  namespace log
  {
    namespace trace
    {
      // ---------------------------------------------------------------------------
      // Forward declarations
      // ---------------------------------------------------------------------------

      class TraceInstance;
      class TraceUnit;


      // ---------------------------------------------------------------------------
      // Declaration of enum classes
      // ---------------------------------------------------------------------------

      //! This enum class defines the registration state of a trace unit.
      enum class RegistrationState
      {
        UNREGISTERED,          //!< Trace unit is not registered and not ready for registration.
        REGISTRATION_READY,    //!< Trace unit is not registered but ready for registration.
        REGISTERED,            //!< Trace unit is registered and not ready for unregistration.
        UNREGISTRATION_READY,  //!< Trace unit is registered and ready for unregistration.
      };


      // ---------------------------------------------------------------------------
      // Declaration of classes
      // ---------------------------------------------------------------------------

      //! This class is responsible for the connection of the trace bundle registration pointer and the management of all trace instances (attach/detach) and all trace units (register/unregister).
      class TraceConnect
      {
      public:
        //! @brief This method gets the singleton instance of class TraceConnect.
        //!
        //! @return       Singleton instance of class TraceConnect.
        static TraceConnect& singleton()
        {
          static TraceConnect singletonInstance;
          return singletonInstance;
        }

        //! @brief Destructor of class TraceConnect.
        virtual ~TraceConnect() = default;

        //! @brief This method attaches all trace units to the trace bundle by registering all defined trace units of all defined trace instances.
        //!
        //! @param[in]    traceRegistration   Pointer to the registration interface which is gotten from the trace bundle.
        //! @return       Status of execution indicating success or error referred to @ref TraceResult.
        //! @retval       TraceResult::UNIT_ALREADY_AVAILABLE   If a trace unit with the same name was already registered.
        //! @retval       TraceResult::UNIT_NAME_INVALID        If the given trace unit name is invalid.
        TraceResult attachTrace(IRegistrationRealTime3* traceRegistration);

        //! @brief This method detaches all trace units from the trace bundle by unregistering all defined trace units of all defined trace instances.
        //!
        //! @return       Status of execution indicating success or error referred to @ref TraceResult.
        //! @retval       TraceResult::UNIT_NOT_AVAILABLE   If a trace unit with the given name was not already registered.
        TraceResult detachTrace();

        //! @brief This method adds a trace instance.
        //!
        //! @param[in]    traceInstance   Trace instance to be added.
        inline void addTraceInstance(TraceInstance* traceInstance)
        {
          m_traceInstances.push_back(traceInstance);
        }

        //! @brief This method gets the registration pointer.
        //!
        //! @return       Registration pointer.
        inline IRegistrationRealTime3* getRegistrationPointer()
        {
          return m_traceRegistration;
        }

        //! @brief This method disables all buffered trace units (standard and indexed) of all instances which could not be registered.
        void disableAllBufferedTraceUnits();

        //! @brief This method sets the origin for all trace instances.
        //!
        //! @param[in]    origin   Origin as app title.
        void setOrigin(const std::string& origin);

      private:
        //! @brief Constructor of class TraceConnect.
        TraceConnect() = default;

        //! @brief Deactivated copy constructor of class TraceConnect.
        TraceConnect(const TraceConnect&) = delete;

        //! @brief Deactivated move constructor of class TraceConnect.
        TraceConnect(TraceConnect&&) = delete;

        //! @brief Deactivated assignment operator of class TraceConnect.
        TraceConnect& operator=(const TraceConnect&) = delete;

        //! Pointer to the registration interface.
        IRegistrationRealTime3* m_traceRegistration = nullptr;

        //! Vector which holds all added trace instances.
        std::vector<TraceInstance*> m_traceInstances;
      };


      //! This class is responsible for the management of all trace units (register/unregister) of one trace instance.
      class TraceInstance
      {
      public:
        //! @brief Constructor of class TraceInstance.
        //!
        //! @param[in]    traceConnect   Reference to @ref TraceConnect object to be able to add all defined trace instances.
        //! @param[in]    infoCode       Main diagnosis code defined in the diagnosis database used for trace infos/messages.
        //! @param[in]    warningCode    Main diagnosis code defined in the diagnosis database used for trace warnings.
        //! @param[in]    errorCode      Main diagnosis code defined in the diagnosis database used for trace errors.
        //! @param[in]    baseEntity     Base entity used for trace logs if no special entity is given.
        //! @param[in]    origin         Origin as app name (e.g. "Automation Core" or "Motion"). Optional.
        TraceInstance(TraceConnect& traceConnect, std::uint32_t infoCode, std::uint32_t warningCode, std::uint32_t errorCode, const std::string baseEntity, const std::string origin = "");

        //! @brief Deactivated copy constructor of class TraceInstance.
        TraceInstance(const TraceInstance&) = delete;

        //! @brief Deactivated move constructor of class TraceInstance.
        TraceInstance(TraceInstance&&) = delete;

        //! @brief Deactivated assignment operator of class TraceInstance.
        TraceInstance& operator=(const TraceInstance&) = delete;

        //! @brief Destructor of class TraceInstance.
        virtual ~TraceInstance() = default;

        //! @brief This method adds a trace unit.
        //!
        //! @param[in]    traceUnit   Trace unit to be added.
        inline void addTraceUnit(TraceUnit* traceUnit)
        {
          m_traceUnits.push_back(traceUnit);
        }

        //! @brief This method registers all defined trace units to the trace bundle.
        //!
        //! @return       Status of execution indicating success or error referred to @ref TraceResult.
        //! @retval       TraceResult::UNIT_ALREADY_AVAILABLE   If a trace unit with the same name was already registered.
        //! @retval       TraceResult::UNIT_NAME_INVALID        If the given trace unit name is invalid.
        TraceResult registerTraceUnits();

        //! @brief This method unregisters all defined trace units from the trace bundle.
        //!
        //! @param[in]    forceUnregistration   Set to 'true' if all trace units of the instance must be unregistered.
        //! @return       Status of execution indicating success or error referred to @ref TraceResult.
        //! @retval       TraceResult::UNIT_NOT_AVAILABLE   If a trace unit with the given name was not already registered.
        TraceResult unregisterTraceUnits(bool forceUnregistration = false);

        //! @brief This method disables all buffered trace units (standard and indexed) which could not be registered.
        void disableAllBufferedTraceUnits();

        //! @brief This method sets the origin for the trace instance.
        //!
        //! @param[in]    origin   Origin as app title.
        void setOrigin(const std::string& origin);

        //! Main diagnosis code defined in the diagnosis database used for trace infos/messages.
        std::uint32_t m_infoCode;

        //! Main diagnosis code defined in the diagnosis database used for trace warnings.
        std::uint32_t m_warningCode;

        //! Main diagnosis code defined in the diagnosis database used for trace errors.
        std::uint32_t m_errorCode;

        //! Base entity used for trace logs if no special entity is given.
        std::string m_baseEntity;

        //! Origin which corresponds to the app title the trace instance is running in.
        std::string m_origin = "unknownOrigin";

      private:
        //! Reference to @ref TraceConnect object to be able to add all defined trace instances.
        TraceConnect& m_traceConnect;

        //! Vector which holds all added trace units.
        std::vector<TraceUnit*> m_traceUnits;
      };


      //! This class represents one single trace unit including buffered trace and enabling states.
      class TraceUnit
      {
      public:
        friend class TraceUnitIndexed;

        //! @brief Constructor of class TraceUnit used by standard trace unit.
        //!
        //! @param[in]    traceInstance          Reference to @ref TraceInstance object to be able to use member variables of the trace instance e.g. main diagnosis code.
        //! @param[in]    unitName               Specific name of the trace unit. The maximum length of the name is @ref MAXIMUM_LENGTH_UNIT_NAME. Longer names are truncated.
        //! @param[in]    initialEnablingState   Initial enabling state for infos/messages, warnings and errors.
        //!
        //! @par Detailed description:
        //! This constructor is used for static created trace units at compile time when already all information is valid (e.g. unit name). It sets @ref m_registrationState to RegistrationState::REGISTRATION_READY.\n
        //! Registration of the unit to the trace bundle can be done immediately when the trace bundle gets valid.
        TraceUnit(TraceInstance& traceInstance, const std::string& unitName, const EnablingState& initialEnablingState);

        //! @brief Constructor of class TraceUnit used by indexed trace unit.
        //!
        //! @param[in]    traceInstance          Reference to @ref TraceInstance object to be able to use member variables of the trace instance e.g. main diagnosis code.
        //! @param[in]    index                  Index of the trace unit to be added to the name of the buffered trace. Range: 1 to amount of trace unit indexed.
        //! @param[in]    initialEnablingState   Initial enabling state for infos/messages, warnings and errors.
        //!
        //! @par Detailed description:
        //! This constructor is used for dynamic created trace units at runtime when all necessary information get valid (e.g. unit name). It sets @ref m_registrationState to RegistrationState::UNREGISTERED.\n
        //! Registration of the unit to the trace bundle can be done after the unit name gets valid by calling the registration process again.
        TraceUnit(TraceInstance& traceInstance, const std::uint32_t index, const EnablingState& initialEnablingState);

        //! @brief Deactivated copy constructor of class TraceUnit.
        TraceUnit(const TraceUnit&) = delete;

        //! @brief Deactivated move constructor of class TraceUnit.
        TraceUnit(TraceUnit&&) = delete;

        //! @brief Deactivated assignment operator of class TraceUnit.
        TraceUnit& operator=(const TraceUnit&) = delete;

        //! @brief Destructor of class TraceUnit.
        virtual ~TraceUnit() = default;

        //! @brief This method gets the enabling state.
        //!
        //! @return       Actual defined enabling state depending on trace context (buffered trace or trace bundle).
        inline EnablingState& getEnablingState()
        {
          if (m_traceContext != &m_traceBuffered)
          {
            return m_traceEnablingState;
          }
          else
          {
            return m_bufferedTraceEnablingState;
          }
        }

        //! @brief This method sets the additional callback function which is called when at least one enabling state for message, warning or error has changed.
        //!
        //! @param[in]    callback   Callback function.
        inline void setAdditionalEnablingStateCallback(const std::function<void(const EnablingState&)> callback)
        {
          m_additionalEnablingStateCallback = callback;
        }

        //! Enabling state callback function which is called when at least one enabling state of infos/messages, warnings and errors has changed.
        std::function<void(const EnablingState&)> enablingStateCallback();

        //! Reference to @ref TraceInstance object to be able to use member variables of the trace instance e.g. main diagnosis code.
        TraceInstance& m_traceInstance;

        //! Buffered trace object used for logging while trace bundle is not present.
        LogBuffered3 m_traceBuffered;

        //! Trace context, either buffered trace or trace bundle.
        ILogRealTime3* m_traceContext;

        //! Name of the trace unit.
        std::string m_unitName = UNKNOWN_UNIT_NAME;

        //! Enabling state of the unit registered in the trace bundle.
        EnablingState m_traceEnablingState = TRACE_ENABLING_STATE_NONE;

        //! Enabling state of the buffered trace.
        EnablingState m_bufferedTraceEnablingState = TRACE_ENABLING_STATE_ALL;

        //! Registration state of the trace unit.
        RegistrationState m_registrationState = RegistrationState::UNREGISTERED;

      protected:
        //! @brief This method sets the unit name for dynamic created unit at runtime (not able to be set by the constructor).
        //!
        //! @param[in]    unitName   Specific name of the trace unit.
        inline void setUnitName(const std::string& unitName)
        {
          m_unitName = unitName;
        }

      private:
        //! Additional callback function which is called when at least one enabling state for message, warning or error has changed.
        std::function<void(const EnablingState&)> m_additionalEnablingStateCallback = nullptr;
      };


      //! This class represents multiple indexed trace units including buffered trace and enabling states.
      class TraceUnitIndexed
      {
      public:
        //! @brief Constructor of class TraceUnitIndexed.
        //!
        //! @param[in]    traceInstance          Reference to @ref TraceInstance object to be able to use member variables of the trace instance e.g. main diagnosis code.
        //! @param[in]    amount                 Amount of trace units to create.
        //! @param[in]    initialEnablingState   Initial enabling state for infos/messages, warnings and errors.
        TraceUnitIndexed(TraceInstance& traceInstance, std::uint32_t amount, const EnablingState& initialEnablingState);

        //! @brief Deactivated copy constructor of class TraceUnitIndexed.
        TraceUnitIndexed(const TraceUnitIndexed&) = delete;

        //! @brief Deactivated move constructor of class TraceUnitIndexed.
        TraceUnitIndexed(TraceUnitIndexed&&) = delete;

        //! @brief Deactivated assignment operator of class TraceUnitIndexed.
        TraceUnitIndexed& operator=(const TraceUnitIndexed&) = delete;

        //! @brief Destructor of class TraceUnitIndexed.
        virtual ~TraceUnitIndexed();

        //! @brief This method sets the unit name of the defined indexed trace unit and registers the trace unit to the trace bundle.
        //!
        //! @param[in]    index      Index of the trace unit to set the name and to register. Range: 1 to amount of trace unit indexed.
        //! @param[in]    unitName   Specific name of the trace unit to set. The maximum length of the name is @ref MAXIMUM_LENGTH_UNIT_NAME. Longer names are truncated.
        //! @return       Status of execution indicating success or error referred to @ref TraceResult.
        //! @retval       TraceResult::UNIT_ALREADY_AVAILABLE   If a trace unit with the same name was already registered.
        //! @retval       TraceResult::UNIT_NAME_INVALID        If the given trace unit name is invalid.
        TraceResult registerTraceUnitIndexed(std::uint32_t index, const std::string& unitName);

        //! @brief This method unregisters the defined indexed trace unit from the trace bundle.
        //!
        //! @param[in]    index   Index of the trace unit to unregister. Range: 1 to amount of trace unit indexed.
        //! @return       Status of execution indicating success or error referred to @ref TraceResult.
        //! @retval       TraceResult::UNIT_NOT_AVAILABLE   If a trace unit with the given name was not already registered.
        TraceResult unregisterTraceUnitIndexed(std::uint32_t index);

        //! @brief This method disables all indexed buffered trace units which are not yet registered.
        void disableAllBufferedTraceUnitsIndexed();

        //! Reference to @ref TraceInstance object to be able to use member variables of the trace instance e.g. main diagnosis code.
        TraceInstance& m_traceInstance;

        //! Amount of trace units.
        std::uint32_t m_amount;

        //! Vector which holds all trace units of indexed trace unit.
        std::vector<TraceUnit*> m_traceUnitsIndexed;
      };
    }
  }
}
