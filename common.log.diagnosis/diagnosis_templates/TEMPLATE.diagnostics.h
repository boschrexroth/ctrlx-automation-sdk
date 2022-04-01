#pragma once

//! @file your_component_diagnostics.h
//! @brief This C++ header contains the diagnostics declarations of component 'your_component' in case of en-US used as default language.

namespace your
{
  namespace component
  {
    constexpr char LIBRARY_DIAGNOSTICS_FILE_NAME[] = "your_component_diagnostics_en-US.json";  //!< File name with diagnostics used in this library.

    // --------------------
    // Main diagnostics
    // --------------------
    constexpr std::uint32_t MAIN_DIAG_<MAIN_DIAG_1_NUMBER> = 0x<MAIN_DIAG_1_NUMBER>;  //!< <MAIN_DIAG_1_TEXT_ENGLISH>
    constexpr std::uint32_t MAIN_DIAG_<MAIN_DIAG_2_NUMBER> = 0x<MAIN_DIAG_2_NUMBER>;  //!< <MAIN_DIAG_2_TEXT_ENGLISH>

    // --------------------
    // Detailed diagnostics
    // --------------------
    constexpr std::uint32_t DETAILED_DIAG_<DETAILED_DIAG_1_NUMBER> = 0x<DETAILED_DIAG_1_NUMBER>;  //!< <DETAILED_DIAG_1_TEXT_ENGLISH>
    constexpr std::uint32_t DETAILED_DIAG_<DETAILED_DIAG_2_NUMBER> = 0x<DETAILED_DIAG_2_NUMBER>;  //!< <DETAILED_DIAG_2_TEXT_ENGLISH>
  }
}
