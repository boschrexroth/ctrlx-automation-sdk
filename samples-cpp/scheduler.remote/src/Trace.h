/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "trace_itf_wrapper.h"

// Namespace in which the trace implementation is used.
#define TRACE_NAMESPACE sdk_rt::trace

// Instance identifier which acts also as object name.
// Type: LABEL
#define TRACE_INSTANCE_ID SDK_RT_TRACE_ID

// Unit identifier which acts also as object name.
// Name has to be unique over all units and instances
// Type: LABEL
#define TRACE_UNIT_ID SDK_RT_UNIT_ID

// Base entity used for trace logs if no special entity is given.
// Type: string
#define TRACE_BASE_ENTITY "sdk"

// Specific name of the trace unit to set.
// The maximum length of the name is common::log::trace::MAXIMUM_LENGTH_UNIT_NAME.
// Longer names are truncated.
// Type: string
#define TRACE_UNIT_NAME "sdk.common.remote-scheduler"

// Declares the global extern object of class common::log::trace::TraceConnect
DECLARATION_TRACE_CONNECT();

// Declares a global extern object of class common::log::trace::TraceInstance
DECLARATION_TRACE_INSTANCE(TRACE_INSTANCE_ID);

// Declares a global extern object of class common::log::trace::TraceUnit
DECLARATION_TRACE_UNIT(TRACE_UNIT_ID);