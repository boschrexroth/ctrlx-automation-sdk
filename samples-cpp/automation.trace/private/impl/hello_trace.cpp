/**
 * MIT License
 * 
 * Copyright (c) 2021 Bosch Rexroth AG
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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "hello_trace.h"

AUTOMATIONTRACE_API automation::trace::RtTrace automation::trace::g_rt_trace;

automation::trace::BaseTrace::BaseTrace()
{
  std::string envSnapName = "SNAP_NAME";

  auto envVar = getenv(envSnapName.c_str());
  if (envVar)
  {
    m_snapName = envVar;
  }

  return;
}


void automation::trace::RtTrace::attachTrace(common::log::trace::IRegistrationRealTime2* traceRegistration)
{
  if (nullptr != traceRegistration)
  {
    m_traceRegistration = traceRegistration;

    if (m_rtTraceContext != &m_traceBuffered)
    {
      detachTrace(traceRegistration);
    }

    auto enablingStateCallback = [&m_traceEnablingState = this->m_traceEnablingState](const common::log::trace::EnablingState& enablingState)
    {
      m_traceEnablingState = enablingState;
    };

    common::log::trace::ILogRealTime* rtTraceContext = nullptr;
    common::log::trace::TraceResult traceResult = traceRegistration->registerUnit(&rtTraceContext, getTraceName(), m_traceEnablingState, enablingStateCallback);
    if (rtTraceContext && common::log::trace::statusSucceeded(traceResult))
    {
      m_rtTraceContext = rtTraceContext;
      TRACE_MSG("Register rt trace unit '%s' succeeded", getTraceName());
    }
    else
    {
      TRACE_ERR("Register rt trace unit '%s' failed ('0x%08X')", getTraceName(), traceResult);
    }
  }
}

void automation::trace::RtTrace::detachTrace(common::log::trace::IRegistrationRealTime2* traceRegistration)
{
  m_traceRegistration = nullptr;

  if (nullptr != traceRegistration)
  {
    common::log::trace::ILogRealTime* traceContext = m_rtTraceContext;
    if (traceContext != &m_traceBuffered)
    {
      m_rtTraceContext = &m_traceBuffered;
      common::log::trace::TraceResult traceResult = traceRegistration->unregisterUnit(&traceContext);
      if (common::log::trace::statusSucceeded(traceResult))
      {
        TRACE_MSG("Unregister rt trace unit '%s' succeeded", getTraceName());
      }
      else
      {
        TRACE_ERR("Unregister rt trace unit '%s' failed ('0x%08X')", getTraceName(), traceResult);
      }
    }
  }
}
