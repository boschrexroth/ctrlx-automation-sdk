//! @copyright (C) 2024-20xx Bosch Rexroth AG
//!
//! The reproduction, distribution and utilization of this file as well as the communication of its contents to others
//! without express authorization is prohibited. Offenders will be held liable for the payment of damages.
//! All rights reserved in the event of the grant of a patent, utility model or design.


// ---------------------------------------------------------------------------
// Include files
// ---------------------------------------------------------------------------

#include "trace_itf_wrapper.h"


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
      // Implementation of TraceConnect class methods
      // ---------------------------------------------------------------------------

      TraceResult TraceConnect::attachTrace(IRegistrationRealTime3* traceRegistration)
      {
        if (nullptr == traceRegistration)
        {
          return TraceResult::INVALID_ARGUMENT;
        }

        m_traceRegistration = traceRegistration;

        TraceResult returnResult = TraceResult::OKAY;
        for (const auto& traceInstance : m_traceInstances)
        {
          auto result = traceInstance->registerTraceUnits();
          if (statusFailed(result))
          {
            returnResult = result;
          }
        }

        return returnResult;
      }


      TraceResult TraceConnect::detachTrace()
      {
        if (nullptr == m_traceRegistration)
        {
          return TraceResult::INVALID_PARAMETER;
        }

        TraceResult returnResult = TraceResult::OKAY;
        for (const auto& traceInstance : m_traceInstances)
        {
          auto result = traceInstance->unregisterTraceUnits(true);
          if (statusFailed(result))
          {
            returnResult = result;
          }
        }

        m_traceRegistration = nullptr;

        return returnResult;
      }


      void TraceConnect::disableAllBufferedTraceUnits()
      {
        for (const auto& traceInstance : m_traceInstances)
        {
          traceInstance->disableAllBufferedTraceUnits();
        }
      }


      void TraceConnect::setOrigin(const std::string& origin)
      {
        for (const auto& traceInstance : m_traceInstances)
        {
          traceInstance->setOrigin(origin);
        }
      }


      // ---------------------------------------------------------------------------
      // Implementation of TraceInstance class methods
      // ---------------------------------------------------------------------------

      TraceInstance::TraceInstance(TraceConnect& traceConnect, std::uint32_t infoCode, std::uint32_t warningCode, std::uint32_t errorCode, const std::string baseEntity, const std::string origin) : m_infoCode(infoCode), m_warningCode(warningCode), m_errorCode(errorCode), m_baseEntity(baseEntity), m_traceConnect(traceConnect)
      {
        std::string envSnapName = "SNAP_NAME";

        if (origin.empty())
        {
#ifdef __linux__
          auto envVar = std::getenv(envSnapName.c_str());
          if (envVar)
          {
            m_origin = envVar;
          }
#else
          char* envVar;
          size_t requiredSize;
          getenv_s(&requiredSize, NULL, 0, envSnapName.c_str());
          if (requiredSize)
          {
            envVar = new char[requiredSize * sizeof(char)];
            if (envVar)
            {
              // Get the value of the envSnapName environment variable.
              getenv_s(&requiredSize, envVar, requiredSize, envSnapName.c_str());
              m_origin = envVar;
            }
            delete[] envVar;
          }
#endif
        }
        else
        {
          m_origin = origin;
        }

        m_traceConnect.addTraceInstance(this);
      }


      TraceResult TraceInstance::registerTraceUnits()
      {
        if (nullptr == m_traceConnect.getRegistrationPointer())
        {
          return TraceResult::INVALID_PARAMETER;
        }

        TraceResult returnResult = TraceResult::OKAY;

        for (auto& traceUnit : m_traceUnits)
        {
          if (RegistrationState::REGISTRATION_READY != traceUnit->m_registrationState)
          {
            continue;
          }

          if (traceUnit->m_traceContext == &traceUnit->m_traceBuffered)
          {
            ILogRealTime3* rtTraceUnit = nullptr;
            TraceResult traceResult = m_traceConnect.getRegistrationPointer()->registerUnit(&rtTraceUnit, traceUnit->m_unitName.c_str(), traceUnit->m_traceEnablingState, traceUnit->enablingStateCallback());
            if (rtTraceUnit && statusSucceeded(traceResult))
            {
              traceUnit->m_traceContext = rtTraceUnit;
              traceUnit->m_registrationState = RegistrationState::REGISTERED;
            }
            else
            {
              returnResult = traceResult;
              COMMON_LOG_TRACE_REAL_TIME_LOG3(&traceUnit->m_traceBuffered, m_errorCode, m_baseEntity.c_str(), m_origin.c_str(), getFileName(__FILE__), __FUNCTION__, __LINE__, "Failed to register trace unit '%s' with error code 0x%08X", traceUnit->m_unitName.c_str(), traceResult);
            }
          }
        }

        return returnResult;
      }


      TraceResult TraceInstance::unregisterTraceUnits(bool forceUnregistration)
      {
        if (nullptr == m_traceConnect.getRegistrationPointer())
        {
          return TraceResult::INVALID_PARAMETER;
        }

        TraceResult returnResult = TraceResult::OKAY;

        for (auto& traceUnit : m_traceUnits)
        {
          if (forceUnregistration)
          {
            if (RegistrationState::REGISTERED == traceUnit->m_registrationState)
            {
              traceUnit->m_registrationState = RegistrationState::UNREGISTRATION_READY;
            }
          }

          if (RegistrationState::UNREGISTRATION_READY != traceUnit->m_registrationState)
          {
            continue;
          }

          if (traceUnit->m_traceContext != &traceUnit->m_traceBuffered)
          {
            ILogRealTime3* rtTraceUnit = traceUnit->m_traceContext;
            traceUnit->m_traceContext = &traceUnit->m_traceBuffered;

            TraceResult traceResult = m_traceConnect.getRegistrationPointer()->unregisterUnit(&rtTraceUnit);
            if (statusSucceeded(traceResult))
            {
              // Set back to initial state of standard trace units, indexed trace units are handled in TraceUnitIndexed::unregisterTraceUnitIndexed()
              traceUnit->m_registrationState = RegistrationState::REGISTRATION_READY;
            }
            else
            {
              returnResult = traceResult;
              COMMON_LOG_TRACE_REAL_TIME_LOG3(&traceUnit->m_traceBuffered, m_errorCode, m_baseEntity.c_str(), m_origin.c_str(), getFileName(__FILE__), __FUNCTION__, __LINE__, "Failed to unregister trace unit '%s' with error code 0x%08X", traceUnit->m_unitName.c_str(), traceResult);
            }
          }
        }

        return returnResult;
      }


      void TraceInstance::disableAllBufferedTraceUnits()
      {
        for (const auto& traceUnit : m_traceUnits)
        {
          // Only disable all buffered trace units which are not in state "REGISTERED"
          if (RegistrationState::REGISTERED != traceUnit->m_registrationState)
          {
            traceUnit->m_bufferedTraceEnablingState = TRACE_ENABLING_STATE_NONE;
          }
        }
      }


      void TraceInstance::setOrigin(const std::string& origin)
      {
        m_origin = origin;
      }


      // ---------------------------------------------------------------------------
      // Implementation of TraceUnit class methods
      // ---------------------------------------------------------------------------

      TraceUnit::TraceUnit(TraceInstance& traceInstance, const std::string& unitName, const EnablingState& initialEnablingState) : m_traceInstance(traceInstance), m_traceBuffered(LogBuffered3(unitName.c_str(), TRACE_ENABLING_STATE_ALL.errors)), m_traceContext(&m_traceBuffered), m_unitName(unitName), m_traceEnablingState(initialEnablingState), m_bufferedTraceEnablingState(initialEnablingState), m_registrationState(RegistrationState::REGISTRATION_READY)
      {
        // For test in jenkins environment: check if trace units should be activated to print stdouts to console
        static std::string envEnableUnits = "TRACE_ENABLE_UNITS";
#ifdef __linux__
        static auto envEnableTrace = std::getenv(envEnableUnits.c_str());
        if (envEnableTrace && std::string(envEnableTrace).find(unitName) != std::string::npos)
        {
          m_traceEnablingState = TRACE_ENABLING_STATE_ALL;
          m_bufferedTraceEnablingState = TRACE_ENABLING_STATE_ALL;
        }
#else
        char* envEnableTrace;
        size_t requiredSize;
        getenv_s(&requiredSize, NULL, 0, envEnableUnits.c_str());
        if (requiredSize)
        {
          envEnableTrace = new char[requiredSize * sizeof(char)];
          if (envEnableTrace)
          {
            // Get the value of the envEnableUnits environment variable.
            getenv_s(&requiredSize, envEnableTrace, requiredSize, envEnableUnits.c_str());
            if (envEnableTrace && std::string(envEnableTrace).find(unitName) != std::string::npos)
            {
              m_traceEnablingState = TRACE_ENABLING_STATE_ALL;
              m_bufferedTraceEnablingState = TRACE_ENABLING_STATE_ALL;
            }
          }
          delete[] envEnableTrace;
        }
#endif
        m_traceInstance.addTraceUnit(this);
      }


      TraceUnit::TraceUnit(TraceInstance& traceInstance, const std::uint32_t index, const EnablingState& initialEnablingState) : m_traceInstance(traceInstance), m_traceBuffered(LogBuffered3((std::string(UNKNOWN_BUFFERED_TRACE3_UNIT_NAME) + " with index " + std::to_string(index)).c_str(), TRACE_ENABLING_STATE_ALL.errors)), m_traceContext(&m_traceBuffered), m_traceEnablingState(initialEnablingState), m_bufferedTraceEnablingState(initialEnablingState), m_registrationState(RegistrationState::UNREGISTERED)
      {
        m_traceInstance.addTraceUnit(this);
      }


      std::function<void(const EnablingState&)> TraceUnit::enablingStateCallback()
      {
        return [this](const EnablingState& enablingState)
          {
            m_traceEnablingState = enablingState;

            if (m_additionalEnablingStateCallback)
            {
              m_additionalEnablingStateCallback(m_traceEnablingState);
            }
          };
      }


      // ---------------------------------------------------------------------------
      // Implementation of TraceUnitIndexed class methods
      // ---------------------------------------------------------------------------

      TraceUnitIndexed::TraceUnitIndexed(TraceInstance& traceInstance, std::uint32_t amount, const EnablingState& initialEnablingState) : m_traceInstance(traceInstance), m_amount(amount)
      {
        for (std::uint32_t index = 1; index <= m_amount; ++index)
        {
          m_traceUnitsIndexed.push_back(new TraceUnit(m_traceInstance, index, initialEnablingState));
        }
      }


      TraceUnitIndexed::~TraceUnitIndexed()
      {
        for (auto& traceUnit : m_traceUnitsIndexed)
        {
          delete traceUnit;
          traceUnit = nullptr;
        }
      }


      TraceResult TraceUnitIndexed::registerTraceUnitIndexed(std::uint32_t index, const std::string& unitName)
      {
        if (0 == index || m_amount < index)
        {
          return TraceResult::INVALID_ARGUMENT;
        }

        std::string oldUnitName = m_traceUnitsIndexed[index - 1]->m_unitName;
        m_traceUnitsIndexed[index - 1]->setUnitName(unitName);
        m_traceUnitsIndexed[index - 1]->m_registrationState = RegistrationState::REGISTRATION_READY;

        TraceResult returnResult = m_traceInstance.registerTraceUnits();

        if (statusFailed(returnResult))
        {
          // Set back to initial state
          m_traceUnitsIndexed[index - 1]->m_registrationState = RegistrationState::UNREGISTERED;
          m_traceUnitsIndexed[index - 1]->setUnitName(oldUnitName);
        }

        // For test in jenkins environment: check if trace units should be activated to print stdouts to console
        static std::string envEnableUnits = "TRACE_ENABLE_UNITS";
#ifdef __linux__
        static auto envEnableTrace = std::getenv(envEnableUnits.c_str());
        if (envEnableTrace && std::string(envEnableTrace).find(unitName) != std::string::npos)
        {
          m_traceUnitsIndexed[index - 1]->m_traceEnablingState = TRACE_ENABLING_STATE_ALL;
          m_traceUnitsIndexed[index - 1]->m_bufferedTraceEnablingState = TRACE_ENABLING_STATE_ALL;
        }
#else
        char* envEnableTrace;
        size_t requiredSize;
        getenv_s(&requiredSize, NULL, 0, envEnableUnits.c_str());
        if (requiredSize)
        {
          envEnableTrace = new char[requiredSize * sizeof(char)];
          if (envEnableTrace)
          {
            // Get the value of the envEnableUnits environment variable.
            getenv_s(&requiredSize, envEnableTrace, requiredSize, envEnableUnits.c_str());
            if (envEnableTrace && std::string(envEnableTrace).find(unitName) != std::string::npos)
            {
              m_traceUnitsIndexed[index - 1]->m_traceEnablingState = TRACE_ENABLING_STATE_ALL;
              m_traceUnitsIndexed[index - 1]->m_bufferedTraceEnablingState = TRACE_ENABLING_STATE_ALL;
            }
          }
          delete[] envEnableTrace;
        }
#endif

        return returnResult;
      }


      TraceResult TraceUnitIndexed::unregisterTraceUnitIndexed(std::uint32_t index)
      {
        if (0 == index || m_amount < index)
        {
          return TraceResult::INVALID_ARGUMENT;
        }

        m_traceUnitsIndexed[index - 1]->m_registrationState = RegistrationState::UNREGISTRATION_READY;

        TraceResult returnResult =  m_traceInstance.unregisterTraceUnits();

        if (statusSucceeded(returnResult))
        {
          // Set back to initial state
          m_traceUnitsIndexed[index - 1]->m_registrationState = RegistrationState::UNREGISTERED;
          m_traceUnitsIndexed[index - 1]->setUnitName(UNKNOWN_UNIT_NAME);
        }

        return returnResult;
      }


      void TraceUnitIndexed::disableAllBufferedTraceUnitsIndexed()
      {
        for (const auto& traceUnit : m_traceUnitsIndexed)
        {
          // Only disable all indexed buffered trace units which are not in state "REGISTERED"
          if (RegistrationState::REGISTERED != traceUnit->m_registrationState)
          {
            traceUnit->m_bufferedTraceEnablingState = TRACE_ENABLING_STATE_NONE;
          }
        }
      }
    }
  }
}

