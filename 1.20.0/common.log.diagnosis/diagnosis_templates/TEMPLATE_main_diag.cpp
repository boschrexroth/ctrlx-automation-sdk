#include "TEMPLATE_main_diag.h"


const common::log::diagnosis::DiagnosisElement <NAMESPACE>::MAIN_DIAGNOSIS_LIST[] =
{
  {
    <NAMESPACE>::MAIN_DIAG_<MAIN_DIAG_1_IDENTIFIER>,
    <MAIN_DIAG_1_VERSION>,
    u8"<MAIN_DIAG_1_TEXT_ENGLISH>",
  },
  {
    <NAMESPACE>::MAIN_DIAG_<MAIN_DIAG_2_IDENTIFIER>,
    <MAIN_DIAG_2_VERSION>,
    u8"<MAIN_DIAG_2_TEXT_ENGLISH>",
  },
};

const std::uint32_t <NAMESPACE>::MAIN_DIAGNOSIS_LIST_SIZE = sizeof(<NAMESPACE>::MAIN_DIAGNOSIS_LIST) / sizeof(common::log::diagnosis::DiagnosisElement);