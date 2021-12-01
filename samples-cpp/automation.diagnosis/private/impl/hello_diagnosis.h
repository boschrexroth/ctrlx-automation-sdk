#pragma once

#include <memory>
#include "common/log/diagnosis/i_log2.h"

namespace common { namespace log { namespace diagnosis { class IRegistration3; } } }

#define AUTOMATIONDIAGNOSIS_API

namespace automation
{
  namespace diagnosis
  {
    class Diagnosis;

    AUTOMATIONDIAGNOSIS_API extern Diagnosis g_diagnosis;

#define DIAGNOSIS_UNIT "automation.diagnosis"

#define DIAGNOSIS_LOG_EXTENDED(logType, mainDiagnosisCode, detailedDiagnosisCode, entity, dynamicDescription, ...) { COMMON_LOG_DIAGNOSIS_LOG2_EXTENDED(automation::diagnosis::g_diagnosis(), logType, mainDiagnosisCode, detailedDiagnosisCode, entity, automation::diagnosis::g_diagnosis.getSnapName(), DIAGNOSIS_UNIT, common::log::diagnosis::getFileName(__FILE__), __FUNCTION__, __LINE__, dynamicDescription, ##__VA_ARGS__); }

    constexpr char LIBRARY_DIAGNOSTICS_FILE_NAME[] = "automation_diagnosis_diagnostics_en-US.json";

	  constexpr std::uint32_t MAIN_DIAG_0E0A0001 = 0x0E0A0001;  // Successfully finished boot process
	  constexpr std::uint32_t MAIN_DIAG_0E0E0001 = 0x0E0E0001;  // Temperature problem of control detected
	  constexpr std::uint32_t MAIN_DIAG_0E0F0001 = 0x0E0F0001;  // Connection to I/O is not possible
	  constexpr std::uint32_t MAIN_DIAG_0E0F2001 = 0x0E0F2001;  // General axis command error
	  constexpr std::uint32_t MAIN_DIAG_0E0F6001 = 0x0E0F6001;  // Fatal axis command error

    //! @class Diagnosis
    class Diagnosis
    {
    public:
      Diagnosis();
      virtual ~Diagnosis() = default;

      void attachDiagnosis(common::log::diagnosis::IRegistration3* diagnosisRegistration);
      void detachDiagnosis(common::log::diagnosis::IRegistration3* diagnosisRegistration);

      void setBundleRootPath(const std::string & bundleRootPath) { m_bundleRootPath = bundleRootPath; }

      std::shared_ptr<common::log::diagnosis::ILog2> operator()() const { return m_diagnosisLog; }

      inline const char* getSnapName() { return m_snapName.c_str(); }

    private:
      void setSnapName();
      std::string m_snapName = "snap.automation.diagnosis";

      std::shared_ptr<common::log::diagnosis::ILog2> m_diagnosisLog;

      std::string m_bundleRootPath;

      std::string m_registrationFilePath;
    };
  }
}
