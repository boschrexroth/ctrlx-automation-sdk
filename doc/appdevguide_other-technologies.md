# APPENDIX TO THE CTRLX APP DEVELOPMENT GUIDELINES
# 1. Document Overview
This document is part of the Bosch Rexroth ctrlX CORE App Development Guidelines, and gives hints for Components and Apps, which are not provided as Linux Ubuntu Core App.
Please make sure that you know the App Development Guidelines, that you can easily understand the content of this document.

The following deployment technologies are considered here
- Windows Application (usually Engineering Tools for ctrlX CORE and ctrlX CORE Apps)
- PLC Libraries, which are used in the ctrlX CORE PLC App based on Codesys
- Node-RED palettes which are loaded into the ctrlX CORE Node-RED App

For each technology this document povides
- A List of aspects which have to be considered in general for onboarding to ctrlX World. Please refer to the App Development Guide for further information on these aspects.
- A brief description of aspects, which are specific to the used deployment technology

The content of this document may be subject of change in further versions of the Validation Process.

# 2. Windows Applications

## 2.1. General Aspects

These aspects, which are also part of the App Development Guidelines main document, must be supported by Windows Applications:
- **Basic App Information** - App Documentation (see App Development Guide Chapter 4.2)
- **Basic App Information** - FOSS Info Provisioning (see App Development Guide Chapter 4.4)
- **Further App Information** - FOSS Sources (see App Development Guide Chapter 5.1)
- **Further App Information** - Semantic Versioning Scheme (see App Development Guide Chapter 5.2)
- **Working Set Overview** - Test Setup for Typical Usage Scenario (see App Development Guide Chapter 6.4)
- **ctrlX Basic Mechanisms** - License Handling (see App Development Guide Chapter 7.1) - this is applicable to Windows Applications only 
which require licenses.
Please get in touch with your ctrlX World partner manager at Bosch Rexroth for further information about Licensing on Windows
## 2.2 Specific Topics
In addition to general aspects, Windows Applications must support the following topics:
- **Signed installation file** (msi) with accordings certificate - the Application is protected against unwanted / unauthorized modification (like e.g. code injections)
- **System Requirements** - Overview about minimum and recommended environment, like Windows version, Memory, Processor

# 3. PLC Libraries and Function Blocks
## 3.1. General Aspects
These aspects, which are also part of the App Development Guidelines main document, must be supported by PLC Libraries and Function Blocks:

- **Basic App Information** - App Documentation (see App Development Guide Chapter 4.2)
- **Basic App Information** - FOSS Info Provisioning (see App Development Guide Chapter 4.4)
- **Further App Information** - FOSS Sources (see App Development Guide Chapter 5.1)
- **Further App Information** - Semantic Versioning Scheme (see App Development Guide Chapter 5.2)
- **Working Set Overview** - Network Security and Ports (see App Development Guide Chapter 6.1.2)
- **Working Set Overview** - Standard Task Scheduling (see App Development Guide Chapter 6.3.)
- **Working Set Overview** - Test Setup for Typical Usage Scenario (see App Development Guide Chapter 6.4)
- **ctrlX Basic Mechanisms** - License Handling (see App Development Guide Chapter 7.1)
- **ctrlX Basic Mechanisms** - Library Signing (see App Development Guide Chapter 7.2) *Note: signing process currently in development at Rexroth*

## 3.2 Specific Topics
In addition to general aspects, PLC Libraries and Function Blocks must support the following topics:

- **Library Header** with official supplier info (Lib info)
- **Integrated Library Documentation** for direct use in PLC Engineering (based on Codesys)
- **Compiled Library** strongly recommended
- **System Requirements** - PLC App version


# 4. Node-RED Palettes
## 4.1. General Aspects
These aspects, which are also part of the App Development Guidelines main document, must be supported by Node-RED Palettes:

 - **Basic App Information** - App Documentation (see App Development Guide Chapter 4.2)
- **Basic App Information** - FOSS Info Provisioning (see App Development Guide Chapter 4.4)
- **Further App Information** - FOSS Sources (see App Development Guide Chapter 5.1)
- **Further App Information** - Semantic Versioning Scheme (see App Development Guide Chapter 5.2)
- **Working Set Overview** - Network Security and Ports (see App Development Guide Chapter 6.1.2)
- **Working Set Overview** - Standard Task Scheduling (see App Development Guide Chapter 6.3.)
- **Working Set Overview** - Test Setup for Typical Usage Scenario (see App Development Guide Chapter 6.4)
- **ctrlX Basic Mechanisms** - License Handling (see App Development Guide Chapter 7.1)

## 4.2 Specific Topics
In addition to general aspects, Node-RED Palettes must support the following topics:
- **Provisioning / Deployment** via Node-RED palette manager


**Copyright**
SPDX-FileCopyrightText: Bosch Rexroth AG