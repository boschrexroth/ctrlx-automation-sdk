#pragma once

//! @copyright (C) 2024-20xx Bosch Rexroth AG
//!
//! The reproduction, distribution and utilization of this file as well as the communication of its contents to others
//! without express authorization is prohibited. Offenders will be held liable for the payment of damages.
//! All rights reserved in the event of the grant of a patent, utility model or design.


// This public header is a template for the easy to use interface of the trace implementation. Use it as template or just copy and adapt it according to your personal demands.


// ---------------------------------------------------------------------------
// Include files
// ---------------------------------------------------------------------------

#include "trace_itf_wrapper.h"


// ---------------------------------------------------------------------------
// Declaration of symbolic constants - using Data Layer as example
// ---------------------------------------------------------------------------

#define TRACE_NAMESPACE wrappertemplate::comm::datalayer::trace  // Definition of the namespace in which the trace implementation is used.


// ---------------------------------------------------------------------------
// Declaration of common part - has to be done using whether standard or multiple indexed trace units
// ---------------------------------------------------------------------------

DECLARATION_TRACE_CONNECT();  // Declaration of the trace connect object.


// ---------------------------------------------------------------------------
// Declaration of standard trace units - using Systemhandler as example for one instance and one unit
// ---------------------------------------------------------------------------

DECLARATION_TRACE_INSTANCE(SYSTEMHANDLER);  // Declaration of the trace instance object.

DECLARATION_TRACE_UNIT(COMMON);  // Declaration of the trace unit object.


// ---------------------------------------------------------------------------
// Declaration of standard trace units - using Data Layer as example for multiple instances and multiple units
// ---------------------------------------------------------------------------

DECLARATION_TRACE_INSTANCE(DATALAYER);        // Declaration of first trace instance object.
DECLARATION_TRACE_INSTANCE(DATALAYERBROKER);  // Declaration of second trace instance object.

DECLARATION_TRACE_UNIT(CORE);        // Declaration of first trace unit object.
DECLARATION_TRACE_UNIT(CLIENT);      // Declaration of second trace unit object.
DECLARATION_TRACE_UNIT(BROKERCORE);  // Declaration of third trace unit object.
DECLARATION_TRACE_UNIT(BROKERUSER);  // Declaration of forth trace unit object.


// ---------------------------------------------------------------------------
// Declaration of multiple indexed trace units - using EtherCAT master as example
// ---------------------------------------------------------------------------

// Commented out due to previous definition for Data Layer example.
//#define TRACE_NAMESPACE comm::ethercat

DECLARATION_TRACE_INSTANCE(ETHERCAT);  // Declaration of the trace instance object.

DECLARATION_TRACE_UNIT_INDEXED(MASTER);   // Declaration of the indexed trace unit object.
DECLARATION_TRACE_UNIT_INDEXED(SLAVE);    // Declaration of another indexed trace unit object.
DECLARATION_TRACE_UNIT_INDEXED(DEVICE);   // Declaration of another indexed trace unit object.
