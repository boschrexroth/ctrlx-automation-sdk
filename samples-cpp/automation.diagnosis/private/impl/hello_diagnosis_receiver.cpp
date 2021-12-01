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

#include <cstring>
#include "common/log/diagnosis/distributor/i_registration.h"
#include "hello_diagnosis_receiver.h"
#include "hello_diagnosis.h"

automation::diagnosis::DiagnosisReceiver::DiagnosisReceiver()
{
  auto callbackDiagnosisReceiver = [this](const common::log::diagnosis::distributor::CallbackParameters &diagnosisInformation)
  {
    // The distributor callback should be only used for fast event reaction. Long lasting reactions need to be done in another context.
    common::log::diagnosis::distributor::DiagnosisEvent diagnosisEvent = diagnosisInformation.getDiagnosisEvent();
    switch (diagnosisEvent)
    {
      case common::log::diagnosis::distributor::DiagnosisEvent::MESSAGE_SET:
        // Handle message reaction
        break;

      case common::log::diagnosis::distributor::DiagnosisEvent::WARNING_SET:
        // Handle warning reaction
        break;

      case common::log::diagnosis::distributor::DiagnosisEvent::ERROR_SET_F0:
        // Handle F0 error reaction
        break;

      case common::log::diagnosis::distributor::DiagnosisEvent::ERROR_SET_F2:
      {
        // Handle F2 error reaction
        common::log::diagnosis::distributor::CallbackParametersType type = diagnosisInformation.getCallbackParametersType();
        switch (type)
        {
          case common::log::diagnosis::distributor::CallbackParametersType::COMMON:
          {
            // Event was registered with callback parameters type "COMMON", so main and detailed diagnosis codes are available
            const common::log::diagnosis::distributor::CallbackParametersCommon& info = static_cast<const common::log::diagnosis::distributor::CallbackParametersCommon&>(diagnosisInformation);
            if ((automation::diagnosis::MAIN_DIAG_0E0F2001 == info.m_mainDiagnosisCode) && (1 == info.m_detailedDiagnosisCode))
            {
              // Handle reaction for error 0E0F2001 / 1 (General axis command error / Axis for axis command was not found)
            }
            break;
          }

          case common::log::diagnosis::distributor::CallbackParametersType::COMMON_ENTITY:
          {
            // Event was registered with callback paramters type "COMMON_ENTITY", so main and detailed diagnosis codes are available and also the depending entity
            const common::log::diagnosis::distributor::CallbackParametersCommonEntity& info = static_cast<const common::log::diagnosis::distributor::CallbackParametersCommonEntity&>(diagnosisInformation);
            if ((automation::diagnosis::MAIN_DIAG_0E0F2001 == info.m_mainDiagnosisCode) && (1 == info.m_detailedDiagnosisCode) && (0 == std::strcmp(info.m_entity, "axis1")))
            {
              // Handle reaction for error 0E0F2001 / 1 (General axis command error / Axis for axis command was not found) with entity "axis1"
            }
            break;
          }

          default:
            // Handle other Callback parameters types
            break;
        }
        break;
      }

      case common::log::diagnosis::distributor::DiagnosisEvent::ERROR_SET_F6:
        // Handle F6 error reaction
        break;

      case common::log::diagnosis::distributor::DiagnosisEvent::HIGHEST_PENDING_LOG_ERROR:
        // Handle highest pending log error reaction
        break;

      case common::log::diagnosis::distributor::DiagnosisEvent::HIGHEST_PENDING_LOG_WARNING:
        // Handle highest pending log warning reaction
        break;

      case common::log::diagnosis::distributor::DiagnosisEvent::HIGHEST_PENDING_LOG_NONE:
        // Handle highest pending log none reaction
        break;

      case common::log::diagnosis::distributor::DiagnosisEvent::RESET_ALL_PENDING:
      {
        // Handle reset all pending reaction for own diagnostics
        if (diagnosisInformation.getCallbackParametersType() == common::log::diagnosis::distributor::CallbackParametersType::ERROR_AND_WARNING_LIST)
        {
          const common::log::diagnosis::distributor::CallbackParametersErrorAndWarningList& info = static_cast<const common::log::diagnosis::distributor::CallbackParametersErrorAndWarningList&>(diagnosisInformation);
          // Loop through all pending diagnostics (errors and warnings) -> This should be done in another context and not directly in the distributor callback.
          for (const auto& log : info.m_errorAndWarningList)
          {
            switch (log.mainDiagnosisCode)
            {
              case automation::diagnosis::MAIN_DIAG_0E0E0001:
              case automation::diagnosis::MAIN_DIAG_0E0F0001:
              case automation::diagnosis::MAIN_DIAG_0E0F2001:
              case automation::diagnosis::MAIN_DIAG_0E0F6001:
                if (!common::log::diagnosis::distributor::evaluateResetBitInMainDiagnosis(log.mainDiagnosisCode))
                {
                  DIAGNOSIS_LOG_EXTENDED(common::log::diagnosis::LogType::RESET_ACTIVE, log.mainDiagnosisCode, log.detailedDiagnosisCode, log.entity.c_str(), "Reset diagnostic due to diagnosis event RESET_ALL_PENDING");
                }
              break;

              default:
                break;
            }
          }
        }

        break;
      }

      default:
        // Error: unknown event
        break;
    }
  };

  common::log::diagnosis::distributor::RegistrationInfo registrationInfo;

  registrationInfo.diagnosisEvent = common::log::diagnosis::distributor::DiagnosisEvent::MESSAGE_SET;
  registrationInfo.processingPriority = common::log::diagnosis::distributor::ProcessingPriority::LOWEST;
  registrationInfo.callbackParametersType = common::log::diagnosis::distributor::CallbackParametersType::COMMON;
  registrationInfo.callback = callbackDiagnosisReceiver;
  m_registrationInfoList.push_back(registrationInfo);

  registrationInfo.diagnosisEvent = common::log::diagnosis::distributor::DiagnosisEvent::WARNING_SET;
  registrationInfo.processingPriority = common::log::diagnosis::distributor::ProcessingPriority::LOWEST;
  registrationInfo.callbackParametersType = common::log::diagnosis::distributor::CallbackParametersType::COMMON;
  registrationInfo.callback = callbackDiagnosisReceiver;
  m_registrationInfoList.push_back(registrationInfo);

  registrationInfo.diagnosisEvent = common::log::diagnosis::distributor::DiagnosisEvent::ERROR_SET_F0;
  registrationInfo.processingPriority = common::log::diagnosis::distributor::ProcessingPriority::LOWEST;
  registrationInfo.callbackParametersType = common::log::diagnosis::distributor::CallbackParametersType::COMMON;
  registrationInfo.callback = callbackDiagnosisReceiver;
  m_registrationInfoList.push_back(registrationInfo);

  registrationInfo.diagnosisEvent = common::log::diagnosis::distributor::DiagnosisEvent::ERROR_SET_F2;
  registrationInfo.processingPriority = common::log::diagnosis::distributor::ProcessingPriority::LOWEST;
  registrationInfo.callbackParametersType = common::log::diagnosis::distributor::CallbackParametersType::COMMON_ENTITY;
  registrationInfo.callback = callbackDiagnosisReceiver;
  m_registrationInfoList.push_back(registrationInfo);

  registrationInfo.diagnosisEvent = common::log::diagnosis::distributor::DiagnosisEvent::HIGHEST_PENDING_LOG_ERROR;
  registrationInfo.processingPriority = common::log::diagnosis::distributor::ProcessingPriority::LOWEST;
  registrationInfo.callbackParametersType = common::log::diagnosis::distributor::CallbackParametersType::COMMON_ENTITY;
  registrationInfo.callback = callbackDiagnosisReceiver;
  m_registrationInfoList.push_back(registrationInfo);

  registrationInfo.diagnosisEvent = common::log::diagnosis::distributor::DiagnosisEvent::HIGHEST_PENDING_LOG_WARNING;
  registrationInfo.processingPriority = common::log::diagnosis::distributor::ProcessingPriority::LOWEST;
  registrationInfo.callbackParametersType = common::log::diagnosis::distributor::CallbackParametersType::COMMON_ENTITY;
  registrationInfo.callback = callbackDiagnosisReceiver;
  m_registrationInfoList.push_back(registrationInfo);

  registrationInfo.diagnosisEvent = common::log::diagnosis::distributor::DiagnosisEvent::HIGHEST_PENDING_LOG_NONE;
  registrationInfo.processingPriority = common::log::diagnosis::distributor::ProcessingPriority::LOWEST;
  registrationInfo.callbackParametersType = common::log::diagnosis::distributor::CallbackParametersType::BASE;
  registrationInfo.callback = callbackDiagnosisReceiver;
  m_registrationInfoList.push_back(registrationInfo);

  registrationInfo.diagnosisEvent = common::log::diagnosis::distributor::DiagnosisEvent::RESET_ALL_PENDING;
  registrationInfo.processingPriority = common::log::diagnosis::distributor::ProcessingPriority::LOWEST;
  registrationInfo.callbackParametersType = common::log::diagnosis::distributor::CallbackParametersType::ERROR_AND_WARNING_LIST;
  registrationInfo.callback = callbackDiagnosisReceiver;
  m_registrationInfoList.push_back(registrationInfo);
}

void automation::diagnosis::DiagnosisReceiver::registerCallbacks(common::log::diagnosis::distributor::IRegistration* distributorRegistration)
{
  common::log::diagnosis::DiagnosisResult diagnosisResult = distributorRegistration->registerCallbacks(m_registrationInfoList);
  if (common::log::diagnosis::statusFailed(diagnosisResult))
  {
    // Error
  }
}

void automation::diagnosis::DiagnosisReceiver::unregisterCallbacks(common::log::diagnosis::distributor::IRegistration* distributorRegistration)
{
  common::log::diagnosis::DiagnosisResult diagnosisResult = distributorRegistration->unregisterCallbacks(m_registrationInfoList);
  if (common::log::diagnosis::statusFailed(diagnosisResult))
  {
    // Error
  }
}
