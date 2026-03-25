//! @copyright (C) 2024-20xx Bosch Rexroth AG
//!
//! The reproduction, distribution and utilization of this file as well as the communication of its contents to others
//! without express authorization is prohibited. Offenders will be held liable for the payment of damages.
//! All rights reserved in the event of the grant of a patent, utility model or design.


// This public CPP file is a template for the easy to use interface of the trace implementation. Use it as template or just copy and adapt it according to your personal demands.


// ---------------------------------------------------------------------------
// Include files
// ---------------------------------------------------------------------------

#include "trace_itf_wrapper_template.h"


// ---------------------------------------------------------------------------
// Declaration of symbolic constants
// ---------------------------------------------------------------------------

namespace TRACE_NAMESPACE
{
  constexpr std::uint32_t TRACE_MAIN_DIAGNOSIS_CODE_MESSAGE = (0x080A0FFF);  // Definition of the main diagnosis code for trace message of the component defined in diagnosis database.
  constexpr std::uint32_t TRACE_MAIN_DIAGNOSIS_CODE_WARNING = (0x080E0FFF);  // Definition of the main diagnosis code for trace warning of the component defined in diagnosis database.
  constexpr std::uint32_t TRACE_MAIN_DIAGNOSIS_CODE_ERROR = (0x080F0FFF);    // Definition of the main diagnosis code for trace error of the component defined in diagnosis database.
}


// ---------------------------------------------------------------------------
// Definition of common part - has to be done when using standard or multiple indexed trace units
// ---------------------------------------------------------------------------

DEFINITION_TRACE_CONNECT();  // Definition and creation of the trace connect object.

// Has to be called in the implementation of the initialization process of the component when the trace bundle becomes valid through Celix framework (registered callback in activator).
//common::log::trace::TraceResult result;
//ATTACH_TRACE(result, service);  // service is of type common::log::trace::IRegistrationRealTime3*
//if (common::log::trace::statusFailed(result))
//{
//  // Do something
//}

// Has to be called in the implementation of the deinitialization process of the component when the trace bundle becomes invalid through Celix framework (registered callback in activator).
//common::log::trace::TraceResult result;
//DETACH_TRACE(result);
//if (common::log::trace::statusFailed(result))
//{
//  // Do something
//}

// ---------------------------------------------------------------------------
// Definition of standard trace units - using Systemhandler as example for one instance and one unit
// ---------------------------------------------------------------------------

DEFINITION_TRACE_INSTANCE_ORIGIN(SYSTEMHANDLER, TRACE_MAIN_DIAGNOSIS_CODE_MESSAGE, TRACE_MAIN_DIAGNOSIS_CODE_WARNING, TRACE_MAIN_DIAGNOSIS_CODE_ERROR, "system", "Automation Core");  // Definition and creation of the trace instance object inclusive static origin.

DEFINITION_TRACE_UNIT(SYSTEMHANDLER, COMMON, "wrappertemplate.common.systemhandler", common::log::trace::TRACE_ENABLING_STATE_ERROR);  // Definition and creation of the trace unit object.

// Setting trace logs in code
//TRACE_ERROR(COMMON, "My trace error with unit '%s'", GET_TRACE_UNIT_NAME(COMMON));
//TRACE_ERROR_ENTITY(COMMON, "system/test", "My trace error with another entity.");


// ---------------------------------------------------------------------------
// Definition of standard trace units - using Data Layer as example for multiple instances and multiple units
// ---------------------------------------------------------------------------

DEFINITION_TRACE_INSTANCE(DATALAYER, TRACE_MAIN_DIAGNOSIS_CODE_MESSAGE, TRACE_MAIN_DIAGNOSIS_CODE_WARNING, TRACE_MAIN_DIAGNOSIS_CODE_ERROR, "datalayer");  // Definition and creation of first trace instance object.
DEFINITION_TRACE_INSTANCE(DATALAYERBROKER, 0x080A0FFE, 0x080E0FFE, 0x080F0FFE, "datalayer/broker");                                                        // Definition and creation of second trace instance object.

DEFINITION_TRACE_UNIT(DATALAYER, CORE, "wrappertemplate.comm.datalayer.core", common::log::trace::TRACE_ENABLING_STATE_ALL);                       // Definition and creation of first trace unit object.
DEFINITION_TRACE_UNIT(DATALAYER, CLIENT, "wrappertemplate.comm.datalayer.client", common::log::trace::TRACE_ENABLING_STATE_WARNING_AND_ERROR);     // Definition and creation of second trace unit object.
DEFINITION_TRACE_UNIT(DATALAYERBROKER, BROKERCORE, "wrappertemplate.comm.datalayer.broker.core", common::log::trace::TRACE_ENABLING_STATE_ERROR);  // Definition and creation of third trace unit object.
DEFINITION_TRACE_UNIT(DATALAYERBROKER, BROKERUSER, "wrappertemplate.comm.datalayer.broker.user", common::log::trace::TRACE_ENABLING_STATE_NONE);   // Definition and creation of forth trace unit object.

// Setting trace logs in code
//TRACE_INFO(CORE, "");
//TRACE_ERROR_ENTITY(CLIENT, "datalayer/test", "My trace error with another entity with unit '%s'.", GET_TRACE_UNIT_NAME(CLIENT));
//TRACE_WARNING(BROKERCORE, "My trace warning with another instance.");
//TRACE_INFO_ENTITY(BROKERUSER, "datalayer/broker/test", "My trace info with another instance and entity.");


// ---------------------------------------------------------------------------
// Definition of multiple indexed trace units - using EtherCAT master as example
// ---------------------------------------------------------------------------

DEFINITION_TRACE_INSTANCE_ORIGIN(ETHERCAT, TRACE_MAIN_DIAGNOSIS_CODE_MESSAGE, TRACE_MAIN_DIAGNOSIS_CODE_WARNING, TRACE_MAIN_DIAGNOSIS_CODE_ERROR, "ethercat", "Ethercat Master");  // Definition and creation of the trace instance object inclusive static origin.

DEFINITION_TRACE_UNIT_INDEXED(ETHERCAT, MASTER, 5, common::log::trace::TRACE_ENABLING_STATE_ALL);    // Definition and creation of the indexed trace unit object with 5 units.
DEFINITION_TRACE_UNIT_INDEXED(ETHERCAT, SLAVE, 10, common::log::trace::TRACE_ENABLING_STATE_ALL);    // Definition and creation of another indexed trace unit object with 10 units.
DEFINITION_TRACE_UNIT_INDEXED(ETHERCAT, DEVICE, 10, common::log::trace::TRACE_ENABLING_STATE_NONE);  // Definition and creation of another indexed trace unit object with 10 units.

// Additionally to the common part: has to be called in the implementation of the component when the trace bundle is valid through Celix framework and the specific trace unit name becomes valid.
//common::log::trace::TraceResult result;
//REGISTER_TRACE_UNIT_INDEXED(result, MASTER, 1, "comm.ethercat.master.instance1");
//REGISTER_TRACE_UNIT_INDEXED(result, MASTER, 2, "comm.ethercat.master.instance2");
//REGISTER_TRACE_UNIT_INDEXED(result, MASTER, 3, "comm.ethercat.master.instance3");

// Additionally to the common part: has to be called in the implementation of the component when the trace bundle becomes invalid through Celix framework or when the specific trace unit name becomes invalid.
//common::log::trace::TraceResult result;
//UNREGISTER_TRACE_UNIT_INDEXED(result, MASTER, 1);
//UNREGISTER_TRACE_UNIT_INDEXED(result, MASTER, 3);
//UNREGISTER_TRACE_UNIT_INDEXED(result, MASTER, 5);

// Setting trace logs in code
//TRACE_ERROR_INDEXED(MASTER, 1, "My trace error in EtherCAT instance '%d'.", 1);
//TRACE_ERROR_INDEXED_ENTITY(MASTER, 1, "ethercat/master/instance1", "My trace error with another entity with unit '%s'.", GET_TRACE_UNIT_INDEXED_NAME(MASTER, 1));
//TRACE_WARNING_INDEXED(MASTER, 2, "My trace warning with another indexed trace unit.");
//TRACE_INFO_INDEXED_ENTITY(MASTER, 3, "ethercat/master/instance3", "My trace info with another indexed trace unit and entity.");
