How to use, create, add and change diagnostics for the diagnosis system    {#publicdocu}
=======================================================================

This documentation is intended for Bosch developers of ctrlX CORE and in many parts also for OEMs, partners and customers.

@copyright (C) 2019-20xx Bosch Rexroth AG

The reproduction, distribution and utilization of this file as well as the communication of its contents to others  
without express authorization is prohibited. Offenders will be held liable for the payment of damages.  
All rights reserved in the event of the grant of a patent, utility model or design.

# Introduction

## Deprecated interfaces
The interfaced IRegistration, IRegistration2 and IRegistration3 are deprecated and should not be used! Use interface IRegistration4 instead!  
Cause: The deprecated interfaces does not comply with C++20 and VS2022. Variable arguments are no longer transferred. This may lead to faulty texts in journald and logbook.

## Some definitions
For the structure (definition of the individual bits) of the main and detailed diagnostic numbers, please see https://inside-docupedia.bosch.com/confluence/display/EDCA/DIA_Control_Diagnosenummer or the officially provided documentation of the diagnosis system https://docs.automation.boschrexroth.com/welcome/.
* Main diagnostic number:
  * Must always be unambiguous.
  * Whole number = 0 is reserved and cannot be used as a valid number.
  * Valid source types (bits 24-29): 08-10 and 30-37, all others are invalid.
  * Valid diagnostic classes in combination with priorities (bits 11-19): A0, E0, F0, F2, F6, F8, F9, all others are invalid.
  * Disappeared bit (bit 20) is handled by diagnosis system -> always use 0.
* Detailed diagnostic number:
  * May be ambiguous.
  * Whole number = 0 is reserved and cannot be used as a valid number.
* The combination of main and detailed diagnostics must again be unambiguous.

## Prepare a component the first time for the usage of diagnostic numbers

### Get diagnostics bundle interfaces
* Add component "common.log.diagnosis" to your component.

### Handle JSON File for un-/registration of diagnostics
* The file contains all main and detailed diagnostic numbers and texts for the default language (which will also always appear in journald).
* Create a JSON file for the diagnostics you want to use:
  * For Bosch developers:
    * For Bosch projects/components it is mandatory to use English texts for the default language.
    * Use the export mechanism of the common database. Details see https://inside-docupedia.bosch.com/confluence/x/dY4ucw.
    * It is necessary to provide the exported JSON file within your bundle. Therefore adapt your bundle CMakeLists.txt:
          
          celix_bundle_files(${BUNDLEX_NAME}
            ${DIAGNOSTICS_FILES_DEPENDENCY_DIR}/public/src/common_log_diagnosis_diagnostics_en-US.json
            DESTINATION "resources/diagnostics"
          )
    * The path to the JSON file which is needed for un-/registration of the diagnostics is composed of the bundle path (gotten from Celix) and your chosen file location in your provided bundle and your chosen file name, e.g. matching the above definition:
          
          std::string pathToJsonFile = m_bundlePath + "/resources/diagnostics/" + "<YOUR_COMPONENT_NAME>_diagnostics_en-US.json";
  * For OEMs, partners and customers:
    * It is recommended to use English texts for the default language.
    * The common.log.diagnosis component provides the file `public/src/diagnosis_templates/TEMPLATE.diagnostics.en-US.json` which can be used as a template for your diagnostics.
      * The key "product" is optional.
    * Recommendation for file location in your component: ./private/src/impl/resources/diagnostics/
    * It is necessary to provide the JSON file within your bundle. Therefore adapt your bundle CMakeLists.txt:
          
          celix_bundle_files(${BUNDLEX_NAME}
            ${CMAKE_SOURCE_DIR}/private/src/impl/resources
            DESTINATION ""
          )
    * Hints:
      * Keep in mind: special characters in your json text strings (like backslash or double quotes) have to be escaped by an additional backslash in front of the special character.
      * The language identifier has to be part of the file name, e.g. `\<NAME\>.en-US.json`. Definitions see https://en.wikipedia.org/wiki/Language_localisation#Language_tags_and_codes.
      * Recommendation for file name: `\<your_component_name\>_diagnostics_\<language\>.json`, e.g. `your_component.diagnostics.en-US.json`.
    * The path to the JSON file which is needed for un-/registration of the diagnostics is composed of the bundle path (gotten from Celix) and your chosen file location in your provided bundle and your chosen file name, e.g. matching the above definition:
          
          std::string pathToJsonFile = m_bundlePath + "/resources/diagnostics/" + "<YOUR_COMPONENT_NAME>.diagnostics.en-US.json";

### Handle declaration of diagnostics in C++ header file
* For sending logs to the diagnosis system, it is necessary to use the main and detailed diagnostic numbers. Therefore, it is recommended to create a header file with the declaration of symbolic constants of all your numbers in it.
  * For Bosch developers: use the header file of the export mechanism of the common database. Details see https://inside-docupedia.bosch.com/confluence/x/dY4ucw.
  * For OEMs, partners and customers: the common.log.diagnosis component provides the file `public/src/diagnosis_templates/TEMPLATE.diagnostics.h` which can be used as a template for your diagnostics.

# Adding or changing a diagnostics

## For Bosch developers of ctrlX CORE

## General steps when adding or changing a diagnostics
In general, when adding/changing a diagnostics the following steps have always to be done.
1. Think about the diagnosis text and do the review therefor
   * see https://inside-docupedia.bosch.com/confluence/x/cB_GPQ
2. Insert a new diagnostic in the common database
   * see https://inside-docupedia.bosch.com/confluence/x/u7_yZg
3. Use the export mechanism of the database to get the json file for your component
   * https://inside-docupedia.bosch.com/confluence/x/dY4ucw
4. Add/edit the related diagnostics documentation
   * see https://inside-docupedia.bosch.com/confluence/x/br8jSg

## Change a main diagnostics
ToDo

## Change a detailed diagnostics
ToDo

# For OEMs, partners and customers
ToDo
