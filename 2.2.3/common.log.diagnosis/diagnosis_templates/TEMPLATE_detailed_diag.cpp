#include "TEMPLATE_detailed_diag.h"


const common::log::diagnosis::DiagnosisElement <NAMESPACE>::DETAILED_DIAGNOSIS_LIST[] =
{
  {
    <NAMESPACE>::DETAILED_DIAG_<DETAILED_DIAG_1_IDENTIFIER>,
    <DETAILED_DIAG_1_VERSION>,
    u8"<DETAILED_DIAG_1_TEXT_ENGLISH>",
    // MainDiagRelation: <DETAILED_DIAG_1_RELATED_MAIN_DIAGS>,
  },
  {
    <NAMESPACE>::DETAILED_DIAG_<DETAILED_DIAG_2_IDENTIFIER>,
    <DETAILED_DIAG_2_VERSION>,
    u8"<DETAILED_DIAG_2_TEXT_ENGLISH>",
    // MainDiagRelation: <DETAILED_DIAG_2_RELATED_MAIN_DIAGS>,
  },
};

const std::uint32_t <NAMESPACE>::DETAILED_DIAGNOSIS_LIST_SIZE = sizeof(<NAMESPACE>::DETAILED_DIAGNOSIS_LIST) / sizeof(common::log::diagnosis::DiagnosisElement);