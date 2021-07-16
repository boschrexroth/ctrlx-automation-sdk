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
// Include Files
// ---------------------------------------------------------------------------

#include <iostream>
#include "hello_callable.h"
#include "hello_trace.h"
#include "common/scheduler/state_generated.h"
#include "common/scheduler/task_generated.h"
#include "common/log/trace/log_buffered.h"

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
HelloCallable::HelloCallable()
{
}

common::scheduler::SchedEventResponse HelloCallable::execute(const common::scheduler::SchedEventType& eventType, const common::scheduler::SchedEventPhase& eventPhase, comm::datalayer::Variant& param)
{
  switch ( eventType )
  {
  case common::scheduler::SchedEventType::SCHED_EVENT_TICK:
  {
    // Do cyclic stuff
    // Better do not use something like std::cout, use trace instead.
    TRACE_MSG("Scheduler event tick");
    return common::scheduler::SchedEventResponse::SCHED_EVENT_RESP_OKAY;
  }
  case common::scheduler::SchedEventType::SCHED_EVENT_SWITCH_TO_CONFIG:
  {
    TRACE_MSG("Switch state to config");
    return common::scheduler::SchedEventResponse::SCHED_EVENT_RESP_OKAY;
  }
  case common::scheduler::SchedEventType::SCHED_EVENT_SWITCH_TO_RUN:
  {
    TRACE_MSG("Switch state to run");
    return common::scheduler::SchedEventResponse::SCHED_EVENT_RESP_OKAY;
  }
  case common::scheduler::SchedEventType::SCHED_EVENT_SWITCH_TO_EXIT:
  {
    TRACE_MSG("Switch state to exit");
    return common::scheduler::SchedEventResponse::SCHED_EVENT_RESP_OKAY;
  }
  case common::scheduler::SchedEventType::SCHED_EVENT_TASK_PROPERTIES_CHANGE:
  {
    TRACE_MSG("Change task properties");
    return common::scheduler::SchedEventResponse::SCHED_EVENT_RESP_OKAY;
  }
  case common::scheduler::SchedEventType::SCHED_EVENT_GET_CURRENT_STATE:
  {
    TRACE_MSG("Get current state");
    return common::scheduler::SchedEventResponse::SCHED_EVENT_RESP_OKAY;
  }
  
  default:
  {  
    return common::scheduler::SchedEventResponse::SCHED_EVENT_RESP_ERROR;
  }
  
  }
}

common::scheduler::SchedState HelloCallable::getState() const
{
  return m_state;
}

