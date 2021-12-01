// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include <iostream>
#include "cmp1.h"
#include "hello_diagnosis.h"


// ---------------------------------------------------------------------------
// Name Spaces
// ---------------------------------------------------------------------------
namespace sdk {
namespace control {


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
celix_status_t Cmp1::init()
{
  std::cout << "call of '" << m_bundleName.c_str() << ": " << __func__ << "'..." << std::endl;

  m_diagnosisReceiver = std::make_unique<automation::diagnosis::DiagnosisReceiver>();
  if (nullptr != m_distributorRegistration)
  {
    m_diagnosisReceiver->registerCallbacks(m_distributorRegistration);
  }

  return CELIX_SUCCESS;
}

celix_status_t Cmp1::start()
{
  std::cout << "call of '" << m_bundleName.c_str() << ": " << __func__ << "'..." << std::endl;

  DIAGNOSIS_LOG_EXTENDED(common::log::diagnosis::LogType::SET_ACTIVE, automation::diagnosis::MAIN_DIAG_0E0A0001, 0, "system/boot", "");
  std::uint32_t maxTemperature = 99;
  DIAGNOSIS_LOG_EXTENDED(common::log::diagnosis::LogType::SET_ACTIVE, automation::diagnosis::MAIN_DIAG_0E0E0001, 1, "diagnosis/temperature", "Maximum temperature: %d°C", maxTemperature);
  DIAGNOSIS_LOG_EXTENDED(common::log::diagnosis::LogType::RESET_ACTIVE, automation::diagnosis::MAIN_DIAG_0E0E0001, 1, "diagnosis/temperature", "");
  std::string commandoError = "0x80000042";
  DIAGNOSIS_LOG_EXTENDED(common::log::diagnosis::LogType::SET_AND_RESET, automation::diagnosis::MAIN_DIAG_0E0F2001, 1, "diagnosis/temperature", "Commando error: %s", commandoError);

  return CELIX_SUCCESS;
}

celix_status_t Cmp1::stop()
{
  std::cout << "call of '" << m_bundleName.c_str() << ": " << __func__ << "'..." << std::endl;
  return CELIX_SUCCESS;
}

celix_status_t Cmp1::deinit()
{
  std::cout << "call of '" << m_bundleName.c_str() << ": " << __func__ << "'..." << std::endl;

  if (nullptr != m_distributorRegistration)
  {
    m_diagnosisReceiver->unregisterCallbacks(m_distributorRegistration);
  }
  m_diagnosisReceiver.reset();

  return CELIX_SUCCESS;
}

void Cmp1::cppDiagnosisServiceStateChanged(common::log::diagnosis::IRegistration3* service)
{
}

void Cmp1::cppDiagnosisServiceAdded(common::log::diagnosis::IRegistration3* service)
{
  automation::diagnosis::g_diagnosis.setBundleRootPath(m_bundlePath);
  automation::diagnosis::g_diagnosis.attachDiagnosis(service);
}

void Cmp1::cppDiagnosisServiceRemoved(common::log::diagnosis::IRegistration3* service)
{
  automation::diagnosis::g_diagnosis.detachDiagnosis(service);
}

void Cmp1::cppDiagnosisDistributorServiceStateChanged(common::log::diagnosis::distributor::IRegistration* service)
{
}

void Cmp1::cppDiagnosisDistributorServiceAdded(common::log::diagnosis::distributor::IRegistration* service)
{
  m_distributorRegistration = service;
}

void Cmp1::cppDiagnosisDistributorServiceRemoved(common::log::diagnosis::distributor::IRegistration* service)
{
  m_distributorRegistration = nullptr;
}

}
}