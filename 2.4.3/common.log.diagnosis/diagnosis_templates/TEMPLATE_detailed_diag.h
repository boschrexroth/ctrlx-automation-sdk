#pragma once

#include "common/log/diagnosis/defs.h"


namespace <NAMESPACE>
{
  extern const common::log::diagnosis::DiagnosisElement DETAILED_DIAGNOSIS_LIST[];
  extern const std::uint32_t DETAILED_DIAGNOSIS_LIST_SIZE;

  constexpr std::uint32_t DETAILED_DIAG_<DETAILED_DIAG_1_IDENTIFIER> = 0x<DETAILED_DIAG_1_CODE>;
  constexpr std::uint32_t DETAILED_DIAG_<DETAILED_DIAG_2_IDENTIFIER> = 0x<DETAILED_DIAG_2_CODE>;
}