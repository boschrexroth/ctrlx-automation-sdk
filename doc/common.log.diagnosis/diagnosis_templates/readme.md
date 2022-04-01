How to use, create, add and change diagnostics for the diagnosis system    {#publicdocu}
=======================================================================

This documentation is intended for Bosch developers of ctrlX CORE and in many parts also for OEMs, partners and customers.

@copyright (C) 2019-20xx Bosch Rexroth AG

The reproduction, distribution and utilization of this file as well as the communication of its contents to others  
without express authorization is prohibited. Offenders will be held liable for the payment of damages.  
All rights reserved in the event of the grant of a patent, utility model or design.

<br/><br/>

# Introduction

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

#### Get diagnostics bundle interfaces
* Add component "common.log.diagnosis" to your component.
#### Handle JSON File for un-/registration of diagnostics
* The file contains all main and detailed diagnostic numbers and texts for the default language (which will also always appear in journald).
* Create a JSON file for the diagnostics you want to use:
  * For Bosch developers:
    * For Bosch projects/components it is mandatory to use English texts for the default language.
    * Use the export mechanism of the common database. Details see https://inside-docupedia.bosch.com/confluence/x/dY4ucw.
    * It is necessary to provide the exported JSON file within your bundle. Therefore adapt your bundle CMakeLists.txt:
      @code
      celix_bundle_files(${BUNDLEX_NAME}
        ${DIAGNOSTICS_FILES_DEPENDENCY_DIR}/public/src/common_log_diagnosis_diagnostics_en-US.json
        DESTINATION "resources/diagnostics"
      )
      @endcode
    * The path to the JSON file which is needed for un-/registration of the diagnostics is composed of the bundle path (gotten from Celix) and your chosen file location in your provided bundle and your chosen file name, e.g. matching the above definition:
      @code
      std::string pathToJsonFile = m_bundlePath + "/resources/diagnostics/" + "<YOUR_COMPONENT_NAME>_diagnostics_en-US.json";
      @endcode
  * For OEMs, partners and customers:
    * It is recommended to use English texts for the default language.
    * The common.log.diagnosis component provides a file ("public/src/diagnosis_templates/TEMPLATE.diagnostics.en-US.json") which can be used as a template for your diagnostics.
      * The key "product" is optional.
    * Recommendation for file location in your component: ./private/src/impl/resources/diagnostics/
    * It is necessary to provide the JSON file within your bundle. Therefore adapt your bundle CMakeLists.txt:
      @code
      celix_bundle_files(${BUNDLEX_NAME}
        ${CMAKE_SOURCE_DIR}/private/src/impl/resources
        DESTINATION ""
      )
      @endcode
    * Hints:
      * Keep in mind: special characters in your json text strings (like backslash or double quotes) have to be escaped by an additional backslash in front of the special character.
      * The language identifier has to be part of the file name, e.g. \<NAME\>.en-US.json. Definitions see https://en.wikipedia.org/wiki/Language_localisation#Language_tags_and_codes.
      * Recommendation for file name: \<your_component_name\>_diagnostics_\<language\>.json, e.g. "your_component.diagnostics.en-US.json".
    * The path to the JSON file which is needed for un-/registration of the diagnostics is composed of the bundle path (gotten from Celix) and your chosen file location in your provided bundle and your chosen file name, e.g. matching the above definition:
      @code
      std::string pathToJsonFile = m_bundlePath + "/resources/diagnostics/" + "<YOUR_COMPONENT_NAME>.diagnostics.en-US.json";
      @endcode
#### Handle declaration of diagnostics in C++ header file
* For sending logs to the diagnosis system, it is necessary to use the main and detailed diagnostic numbers. Therefore, it is recommended to create a header file with the declaration of symbolic constants of all your numbers in it.
* For Bosch developers: use the header file of the export mechanism of the common database. Details see https://inside-docupedia.bosch.com/confluence/x/dY4ucw.
* For OEMs, partners and customers: the common.log.diagnosis component provides a file ("public/src/diagnosis_templates/TEMPLATE.diagnostics.h") which can be used as a template for your diagnostics.


<br/><br/>

# For Bosch developers of ctrlX CORE

## General steps when adding/changing a diagnostics

In general, when adding/changing a diagnostics the following steps have always to be done.
1. Insert a new diagnostic in the common database
   * see https://inside-docupedia.bosch.com/confluence/x/u7_yZg
2. Use the export mechanism of the database to get the json file for your component
   * https://inside-docupedia.bosch.com/confluence/x/dY4ucw
3. Add/edit the related diagnostics documentation
   * see https://inside-docupedia.bosch.com/confluence/x/br8jSg

# For OEMs, partners and customers

ToDo

<br/><br/>

# Deprecated interfaces IRegistration and IRegistration2

## Prepare a component the first time for the usage of diagnosis numbers

 * Add the component "common.log.diagnosis" in the cdf.xml of your component and do a "reset dependencies".
 * Depending if main and/or detailed diagnosis are needed, copy the related .cpp/.h files to the folder "public/src/diagnosis" of your component.
 * Change 'TEMPLATE' to the name of the repository/component <b>in small letters</b>:
    * in the file names
    * in the cpp-file(s): the name of the included header file
    * e.g. common_mycomponent_main_diag.h
 * Set CMAKE include path for dependency to "common.log.diagnosis/public/include" (recommendation: set include path in file private/src/lib/CMakeLists.txt).
 * Set CMAKE include path for dependency to "public/src/diagnosis" of your component (recommendation: set include path in file private/src/lib/CMakeLists.txt).
 * Add in CMAKE .cpp files to source files (section SOURCE_FILES) with full path (recommendation: add in private/src/lib/CMakeLists.txt).
 * Include one/both header file(s) in the c/cpp-file where diagnosis numbers are used (e.g. \#include "common_mycomponent_main_diag.h").
 * If not done before, continue as usual e.g. with adding the diagnosis library to your component/project and the diagnosis functions to your code.

## <a name="link_headline_general_steps"></a>General steps when adding/changing a diagnosis

In general, when adding/changing a diagnosis the following steps have always to be done.

1. Think about the diagnosis text and do the review therefor
    * see https://inside-docupedia.bosch.com/confluence/x/cB_GPQ

2. Insert a new diagnosis in the common database
    * see https://inside-docupedia.bosch.com/confluence/x/FlmwQg

3. Use the information of the database and add/change them in the related header file(s)
    * [Add a new diagnosis (if diagnoses ARE NOT AVAILABLE in the header file(s))](#link_headline_add_diag_if_not_available)
    * [Add a new diagnosis (if diagnoses ARE AVAILABLE in the header file(s))](#link_headline_add_diag_if_available)
    * [Change an existing diagnosis](#link_headline_change_diag)

4. Add/edit the related diagnosis documentation
    * see https://inside-docupedia.bosch.com/confluence/display/TDKB/ctrlX-Diagnosen

## <a name="link_headline_add_diag_if_not_available"></a>Add a new diagnosis (if diagnoses ARE NOT AVAILABLE in the header file(s))

### Add a new main diagnosis

1. See section [General steps when adding/changing a diagnosis](#link_headline_general_steps).
2. Changes in ..._main_diag.h/.cpp:
   * General: all replaces to be done including \<\>.
   * Replace \<NAMESPACE\> with the full namespace of your component (e.g. "common::mycomponent") and correct the namespaces in header file manually (has to be separated by brackets instead of "::" ).
   * Replace \<MAIN_DIAG_1_CODE\> with the full 8 digit hexadecimal number of the main diagnosis code (e.g. "080F2345").
   * Replace \<MAIN_DIAG_1_IDENTIFIER\> with a text associated with the diagnosis <b>in capital letters</b> (e.g. "SCRIPT_PROCESS" so at the end the constant should look like this: "MAIN_DIAG_SCRIPT_PROCESS").
   * Replace \<MAIN_DIAG_1_TEXT_ENGLISH\> with the English description of the main diagnosis.
   * Replace \<MAIN_DIAG_1_VERSION\> with the version of the main diagnosis (without "").
3. Use or delete the templates for a second diagnosis (see \<MAIN_DIAG_2...\>) and sort it by diagnosis number!
 
### Add a new detailed diagnosis

1. See section [General steps when adding/changing a diagnosis](#link_headline_general_steps).
2. Changes in ..._detailed_diag.h/.cpp:
   * General: all replaces to be done including \<\>.
   * Replace \<NAMESPACE\> with the full namespace of your component (e.g. "common::mycomponent") and correct the namespaces in header file manually (has to be seperated by brackets instead of "::" ).
   * Replace \<DETAILED_DIAG_1_CODE\> with the full 8 digit hexadecimal number of the detailed diagnosis code with the sub system id "C" for ctrlX CORE (e.g. 0C0D0001)
   * Replace \<DETAILED_DIAG_1_IDENTIFIER\> with a text associated with the diagnosis <b>in capital letters</b> (e.g. "PROCESS_CREATE" so at the end the constant should look like this: "DETAILED_DIAG_PROCESS_CREATE").
   * Replace \<DETAILED_DIAG_1_TEXT_ENGLISH\> with the English description of the detailed diagnosis.
   * Replace \<DETAILED_DIAG_1_VERSION\> with the version of the detailed diagnosis (without "").
   * Replace \<DETAILED_DIAG_1_RELATED_MAIN_DIAGS\> with all related main diagnoses separated by ";" as 8 digit values (see \<MAIN_DIAG_1_CODE\>) in one line (e.g. "080F2345;080F2468,").
3. Use or delete the templates for a second diagnosis (see \<DETAILED_DIAG_2...\>) and sort it by diagnosis number!

## <a name="link_headline_add_diag_if_available"></a>Add a new diagnosis (if diagnoses ARE AVAILABLE in the header file(s))

### Add a new main or detailed diagnosis

1. See section [General steps when adding/changing a diagnosis](#link_headline_general_steps).
2. General: add the new diagnosis sorted by diagnosis number!
3. Copy and paste the following lines from the related diagnosis template files "TEMPLATE_main_diag.h" or "TEMPLATE_detailed_diag.h".
   * Note: e.g. use the "mark"-functionality in Notepad++.
   * main diagnosis: all lines including "MAIN_DIAG_1" (including related lines like "{" of the struct).
   * detailed diagnosis: all lines including "DETAILED_DIAG_1" (including related lines like "{" of the struct).
4. Continue replacing the keywords as described in section [Add a new diagnosis (if diagnosis ARE NOT AVAILABLE in the header file(s))](#link_headline_add_diag_if_not_available).

## <a name="link_headline_change_diag"></a>Change an existing diagnosis

1. See section [General steps when adding/changing a diagnosis](#link_headline_general_steps).

### Change a main diagnosis

ToDo

### Change a detailed diagnosis

ToDo
