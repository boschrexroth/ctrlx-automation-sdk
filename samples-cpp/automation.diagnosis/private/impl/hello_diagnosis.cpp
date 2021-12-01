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

#include "common/log/diagnosis/i_registration3.h"
#include "hello_diagnosis.h"

namespace automation { namespace diagnosis { AUTOMATIONDIAGNOSIS_API Diagnosis g_diagnosis; } }

automation::diagnosis::Diagnosis::Diagnosis()
{
  std::string envSnapName = "SNAP_NAME";
  
  auto envVar = getenv(envSnapName.c_str());
  if (envVar)
  {
    m_snapName = envVar;
  }
  
  return;
}

void automation::diagnosis::Diagnosis::attachDiagnosis(common::log::diagnosis::IRegistration3* diagnosisRegistration)
{
  if (nullptr != diagnosisRegistration)
  {
    m_registrationFilePath = m_bundleRootPath + "/resources/diagnostics/" + LIBRARY_DIAGNOSTICS_FILE_NAME;
    common::log::diagnosis::DiagnosisResult diagnosisResult = diagnosisRegistration->registerDefaultDiagnoses(m_registrationFilePath, m_diagnosisLog);
    if (common::log::diagnosis::statusFailed(diagnosisResult))
    {
      if (m_diagnosisLog)
      {
        m_diagnosisLog.reset();
      }
      return;
    }
  }
}

void automation::diagnosis::Diagnosis::detachDiagnosis(common::log::diagnosis::IRegistration3* diagnosisRegistration)
{
  if (nullptr != diagnosisRegistration)
  {
    m_diagnosisLog.reset();
    
    common::log::diagnosis::DiagnosisResult diagnosisResult = diagnosisRegistration->unregisterDefaultDiagnoses(m_registrationFilePath);
    if (common::log::diagnosis::statusFailed(diagnosisResult))
    {
      // Error
    }
  }
}