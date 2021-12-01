#pragma once

#include <vector>

namespace common { namespace log { namespace diagnosis { namespace distributor { class IRegistration; } } } }
namespace common { namespace log { namespace diagnosis { namespace distributor { struct RegistrationInfo; } } } }

namespace automation
{
  namespace diagnosis
  {
    class DiagnosisReceiver
    {
    public:
      DiagnosisReceiver();
      virtual ~DiagnosisReceiver() = default;

      void registerCallbacks(common::log::diagnosis::distributor::IRegistration* distributorRegistration);
      void unregisterCallbacks(common::log::diagnosis::distributor::IRegistration* distributorRegistration);

    private:
      std::vector<common::log::diagnosis::distributor::RegistrationInfo> m_registrationInfoList;
    };
  }
}