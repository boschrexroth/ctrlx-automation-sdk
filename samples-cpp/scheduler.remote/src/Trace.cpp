/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
#include "Trace.h"

namespace TRACE_NAMESPACE
{
// Main diagnosis code for trace message of component.
constexpr std::uint32_t TRACE_MAIN_DIAGNOSIS_CODE_MESSAGE = (0x0E0A0ABC);

// Main diagnosis code for trace warning of component.
constexpr std::uint32_t TRACE_MAIN_DIAGNOSIS_CODE_WARNING = (0x0E0E0ABC);

// Main diagnosis code for trace error of component.
constexpr std::uint32_t TRACE_MAIN_DIAGNOSIS_CODE_ERROR = (0x0E0F0ABC);
} // namespace TRACE_NAMESPACE

#ifndef NDEBUG
common::log::trace::EnablingState traceEnablingState = common::log::trace::TRACE_ENABLING_STATE_ERROR;
#else
common::log::trace::EnablingState traceEnablingState = common::log::trace::TRACE_ENABLING_STATE_ALL;
#endif

// Defines and creates the global extern object of class common::log::trace::TraceConnect
DEFINITION_TRACE_CONNECT();

// Defines and creates a global extern object of class common::log::trace::TraceInstance.
DEFINITION_TRACE_INSTANCE(TRACE_INSTANCE_ID, TRACE_MAIN_DIAGNOSIS_CODE_MESSAGE, TRACE_MAIN_DIAGNOSIS_CODE_WARNING,
                          TRACE_MAIN_DIAGNOSIS_CODE_ERROR, TRACE_BASE_ENTITY);

// Defines and creates a global extern object of class common::log::trace::TraceUnit.
DEFINITION_TRACE_UNIT(TRACE_INSTANCE_ID, TRACE_UNIT_ID, TRACE_UNIT_NAME, traceEnablingState);
