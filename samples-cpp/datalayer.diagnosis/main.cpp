/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <csignal>
#include <iostream>
#include <thread>

#include "comm/datalayer/metadata_helper.h"
#include "comm/datalayer/sub_properties_generated.h"
#include "common/log/diagnosis/set_generated.h"
#include "common/log/diagnosis/list_diagnosis_identification_with_timestamp_generated.h"
#include "common/log/diagnosis/get_main_text_generated.h"
#include "common/log/diagnosis/get_detailed_text_generated.h"
#include "common/log/diagnosis/event_type_1_generated.h"
#include "common/log/diagnosis/event_type_2_generated.h"
#include "common/log/diagnosis/pending_diagnostic_state_generated.h"
#include "common/log/diagnosis/i_log2.h"

#include "ctrlx_datalayer_helper.h"

 // Flags to log error messages only once
static bool g_errorConnection = false;
static bool g_errorDiagnosis = false;

// Add signal handling so we are able to abort the program by sending SIGINT
static bool g_endProcess = false;
static void signalHandler(int signal)
{
  std::cout << "signal: " << signal << std::endl;
  g_endProcess = true;
}

static comm::datalayer::PublishCallback eventsCallback()
{
  return [&](comm::datalayer::DlResult status, const std::vector<comm::datalayer::NotifyItem>& items)
    {
      std::cout << "  Got notification" << std::endl;

      if (comm::datalayer::STATUS_FAILED(status))
      {
        std::cerr << "  Status: " << status.toString() << std::endl;
        return;
      }

      for (auto item = items.begin(); item != items.end(); item++)
      {
        auto info = comm::datalayer::GetNotifyInfo(item->info.getData());

        std::cout << "  Got notification '" << comm::datalayer::EnumNameNotifyType(info->notifyType()) << "' for node '" << info->node()->c_str() << "'." << std::endl;
        if (comm::datalayer::NotifyType_Data != info->notifyType())
        {
          continue;
        }
        comm::datalayer::Variant value = item->data;
        std::string nodeAdress = info->node()->c_str();
        if (0 == nodeAdress.compare("diagnosis/events/message-set"))
        {
          if (comm::datalayer::STATUS_FAILED(value.verifyFlatbuffers(common::log::diagnosis::fbs::VerifyEventType1Buffer)))
          {
            std::cerr << "  Invalid argument type for data" << std::endl;
            return;
          }

          auto eventData = common::log::diagnosis::fbs::GetEventType1(value.getData());
          std::cout << "  Event counter (diagnosis/events/message-set): " << eventData->eventCounter() << std::endl;
          continue;
        }

        if (0 == nodeAdress.compare("diagnosis/events/warning-set"))
        {
          if (comm::datalayer::STATUS_FAILED(value.verifyFlatbuffers(common::log::diagnosis::fbs::VerifyEventType1Buffer)))
          {
            std::cerr << "  Invalid argument type for data" << std::endl;
            return;
          }

          auto eventData = common::log::diagnosis::fbs::GetEventType1(value.getData());
          std::cout << "  Event counter (diagnosis/events/warning-set): " << eventData->eventCounter() << std::endl;
          continue;
        }

        if (0 == nodeAdress.compare("diagnosis/events/error-set-F0"))
        {
          if (comm::datalayer::STATUS_FAILED(value.verifyFlatbuffers(common::log::diagnosis::fbs::VerifyEventType1Buffer)))
          {
            std::cerr << "  Invalid argument type for data" << std::endl;
            return;
          }

          auto eventData = common::log::diagnosis::fbs::GetEventType1(value.getData());
          std::cout << "  Event counter (diagnosis/events/error-set-F0): " << eventData->eventCounter() << std::endl;
          continue;
        }

        if (0 == nodeAdress.compare("diagnosis/events/error-set-F2"))
        {
          if (comm::datalayer::STATUS_FAILED(value.verifyFlatbuffers(common::log::diagnosis::fbs::VerifyEventType1Buffer)))
          {
            std::cerr << "  Invalid argument type for data" << std::endl;
            return;
          }

          auto eventData = common::log::diagnosis::fbs::GetEventType1(value.getData());
          std::cout << "  Event counter (diagnosis/events/error-set-F2): " << eventData->eventCounter() << std::endl;
          continue;
        }

        if (0 == nodeAdress.compare("diagnosis/events/error-set-F6"))
        {
          if (comm::datalayer::STATUS_FAILED(value.verifyFlatbuffers(common::log::diagnosis::fbs::VerifyEventType1Buffer)))
          {
            std::cerr << "  Invalid argument type for data" << std::endl;
            return;
          }

          auto eventData = common::log::diagnosis::fbs::GetEventType1(value.getData());
          std::cout << "  Event counter (diagnosis/events/error-set-F6): " << eventData->eventCounter() << std::endl;
          continue;
        }
        if (0 == nodeAdress.compare("diagnosis/events/highest-pending-log-none"))
        {
          if (comm::datalayer::STATUS_FAILED(value.verifyFlatbuffers(common::log::diagnosis::fbs::VerifyEventType1Buffer)))
          {
            std::cerr << "  Invalid argument type for data" << std::endl;
            return;
          }

          auto eventData = common::log::diagnosis::fbs::GetEventType1(value.getData());
          std::cout << "  Event counter (diagnosis/events/highest-pending-log-none): " << eventData->eventCounter() << std::endl;
          continue;
        }
        if (0 == nodeAdress.compare("diagnosis/events/highest-pending-log-warning"))
        {
          if (comm::datalayer::STATUS_FAILED(value.verifyFlatbuffers(common::log::diagnosis::fbs::VerifyEventType1Buffer)))
          {
            std::cerr << "  Invalid argument type for data" << std::endl;
            return;
          }

          auto eventData = common::log::diagnosis::fbs::GetEventType1(value.getData());
          std::cout << "  Event counter (diagnosis/events/highest-pending-log-warning): " << eventData->eventCounter() << std::endl;
          continue;
        }

        if (0 == nodeAdress.compare("diagnosis/events/highest-pending-log-error"))
        {
          if (comm::datalayer::STATUS_FAILED(value.verifyFlatbuffers(common::log::diagnosis::fbs::VerifyEventType1Buffer)))
          {
            std::cerr << "  Invalid argument type for data" << std::endl;
            return;
          }

          auto eventData = common::log::diagnosis::fbs::GetEventType1(value.getData());
          std::cout << "  Event counter (diagnosis/events/highest-pending-log-error): " << eventData->eventCounter() << std::endl;
          continue;
        }

        if (0 == nodeAdress.compare("diagnosis/events/reset-all-pending"))
        {
          if (comm::datalayer::STATUS_FAILED(value.verifyFlatbuffers(common::log::diagnosis::fbs::VerifyEventType2Buffer)))
          {
            std::cerr << "  Invalid argument type for data" << std::endl;
            return;
          }

          auto eventData = common::log::diagnosis::fbs::GetEventType2(value.getData());
          std::cout << "  Event counter (diagnosis/events/reset-all-pending): " << eventData->eventCounter() << std::endl;
        }
      }
    };
}

static int run(comm::datalayer::IClient2* client)
{

  std::cout << "Register diagnostics with file path on PC or with file path in own app for interprocess communication (Alternative 2)" << std::endl;

  auto basePath = snapPath();
  if (nullptr == basePath)
  {
    // Not in snap environment
    char workingDirectory[512];
    basePath = getcwd(workingDirectory, 512);
    if (nullptr == basePath)
    {
      std::cerr << "ERROR Failed to get base directory" << std::endl;
      return 1;
    }
  }

  std::string registrationFilePath = std::string(basePath) + "/resources/diagnostics/sdk_diagnosis_datalayer.diagnostics.en-US.json";
  std::cout << "Using type information from file " << registrationFilePath << std::endl;

  comm::datalayer::MetadataHelper metadataHelper;
  comm::datalayer::Variant binaryFileContent;
  comm::datalayer::DlResult dlResult = metadataHelper.getFileContent(registrationFilePath, binaryFileContent);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "Failed to get type information of file " << registrationFilePath << ", error code: " << dlResult.toString() << std::endl;
    return 1;
  }

  dlResult = client->writeSync("diagnosis/registration/register-file-content", &binaryFileContent);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "Register diagnostics failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  //////////////////////////////////////////////////////////////////////////////////
  std::cout << "Subscribe to diagnosis events" << std::endl;
  //////////////////////////////////////////////////////////////////////////////////

  flatbuffers::FlatBufferBuilder builder;
  comm::datalayer::Variant subscriptionProperties;
  std::string subscriptionName = "eventsNotifier";

  std::cout << "Create subscription with name '" << subscriptionName << "'" << std::endl;
  auto id = builder.CreateString(subscriptionName);
  builder.Finish(comm::datalayer::CreateSubscriptionProperties(builder, id, 60000, 100));
  subscriptionProperties.shareFlatbuffers(builder);
  dlResult = client->createSubscriptionSync(subscriptionProperties, eventsCallback());
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "Failed to create subscription with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  std::cout << "Subscribe to all diagnosis event nodes" << std::endl;
  std::set<std::string> addresses = {"diagnosis/events/message-set", "diagnosis/events/warning-set", "diagnosis/events/error-set-F0", "diagnosis/events/error-set-F2", "diagnosis/events/error-set-F6", "diagnosis/events/highest-pending-log-none", "diagnosis/events/highest-pending-log-warning", "diagnosis/events/highest-pending-log-error", "diagnosis/events/reset-all-pending"};
  dlResult = client->subscribeSync(subscriptionName, addresses);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "Failed to subscribe with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  // Wait for the finish of the initialization events
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  //////////////////////////////////////////////////////////////////////////////////
  std::cout << "Set diagnostics" << std::endl;
  //////////////////////////////////////////////////////////////////////////////////

  comm::datalayer::Variant dataSet;

  std::cout << "Set message (0E0A0001, 00000001, -)" << std::endl;
  auto logParameters = common::log::diagnosis::fbs::CreateLogParametersDirect(builder, "0E0A0001", "00000001", nullptr, nullptr, "datalayerdiagnosis", "common.bootup", common::log::diagnosis::getFileName(__FILE__), __FUNCTION__, __LINE__, "");

  builder.Finish(logParameters);
  dataSet.shareFlatbuffers(builder);

  dlResult = client->writeSync("diagnosis/set/set-active", &dataSet);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "set-active failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  std::cout << "Set active warning(0E0E0001, 00000001, diagnosis/temperature)" << std::endl;
  logParameters = common::log::diagnosis::fbs::CreateLogParametersDirect(builder, "0E0E0001", "00000001", nullptr, "diagnosis/temperature", "datalayerdiagnosis", "common.log.diagnosis", common::log::diagnosis::getFileName(__FILE__), __FUNCTION__, __LINE__, "Maximum temperature: 99°C");

  builder.Finish(logParameters);
  dataSet.shareFlatbuffers(builder);

  dlResult = client->writeSync("diagnosis/set/set-active", &dataSet);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "set-active failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  std::cout << "Set active error (0E0F2001, 00000001, motion/axs/z3)" << std::endl;
  logParameters = common::log::diagnosis::fbs::CreateLogParametersDirect(builder, "0E0F2001", "00000001", nullptr, "motion/axs/z3", "datalayerdiagnosis", "motion.core", common::log::diagnosis::getFileName(__FILE__), __FUNCTION__, __LINE__, "");

  builder.Finish(logParameters);
  dataSet.shareFlatbuffers(builder);

  dlResult = client->writeSync("diagnosis/set/set-active", &dataSet);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "set-active failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  std::cout << "Set and reset error (0E0F0001, 00000001, devices/io)" << std::endl;
  logParameters = common::log::diagnosis::fbs::CreateLogParametersDirect(builder, "0E0F0001", "00000001", nullptr, "devices/io/5", "datalayerdiagnosis", "io.manager", common::log::diagnosis::getFileName(__FILE__), __FUNCTION__, __LINE__, "");

  builder.Finish(logParameters);
  dataSet.shareFlatbuffers(builder);

  dlResult = client->writeSync("diagnosis/set/set-and-reset", &dataSet);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "set-and-reset failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  std::cout << "Reset error (0E0F2001, 00000001, motion/axs/z3)" << std::endl;
  logParameters = common::log::diagnosis::fbs::CreateLogParametersDirect(builder, "0E0F2001", "00000001", nullptr, "motion/axs/z3", "datalayerdiagnosis", "motion.core", common::log::diagnosis::getFileName(__FILE__), __FUNCTION__, __LINE__, "");

  builder.Finish(logParameters);
  dataSet.shareFlatbuffers(builder);

  dlResult = client->writeSync("diagnosis/set/reset-active", &dataSet);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "reset-active failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  std::cout << "Reset warning (0E0E0001, 00000001, diagnosis/temperature)" << std::endl;
  logParameters = common::log::diagnosis::fbs::CreateLogParametersDirect(builder, "0E0E0001", "00000001", nullptr, "diagnosis/temperature", "datalayerdiagnosis", "common.log.diagnosis", common::log::diagnosis::getFileName(__FILE__), __FUNCTION__, __LINE__, "");

  builder.Finish(logParameters);
  dataSet.shareFlatbuffers(builder);

  dlResult = client->writeSync("diagnosis/set/reset-active", &dataSet);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "reset-active failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  std::cout << "Set active error (0E0F6001, 00000001, -)" << std::endl;
  logParameters = common::log::diagnosis::fbs::CreateLogParametersDirect(builder, "0E0F6001", "00000001", nullptr, nullptr, "datalayerdiagnosis", "motion.core", common::log::diagnosis::getFileName(__FILE__), __FUNCTION__, __LINE__, "");

  builder.Finish(logParameters);
  dataSet.shareFlatbuffers(builder);

  dlResult = client->writeSync("diagnosis/set/set-active", &dataSet);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "set-active failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  std::cout << "Set active warning (0E0E0003, 00000001, scheduler/watchdog)" << std::endl;
  logParameters = common::log::diagnosis::fbs::CreateLogParametersDirect(builder, "0E0E0003", "00000001", nullptr, "scheduler/watchdog", "datalayerdiagnosis", "common.scheduler", common::log::diagnosis::getFileName(__FILE__), __FUNCTION__, __LINE__, "");

  builder.Finish(logParameters);
  dataSet.shareFlatbuffers(builder);

  dlResult = client->writeSync("diagnosis/set/set-active", &dataSet);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "set-active failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  //////////////////////////////////////////////////////////////////////////////////
  std::cout << "Get highest priority pending diagnostic" << std::endl;
  //////////////////////////////////////////////////////////////////////////////////

  comm::datalayer::Variant logData;

  dlResult = client->readSync("diagnosis/get/actual/log", &logData);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "Get actual log failed with error code '" << dlResult.toString() << "'. Could not highest priority log." << std::endl;
    return 1;
  }

  if (comm::datalayer::STATUS_FAILED(logData.verifyFlatbuffers(common::log::diagnosis::fbs::VerifyDiagnosisIdentificationWithTimestampBuffer)))
  {
    std::cerr << "Invalid data type for logData. Expected flatbuffer type DiagnosisIdentificationWithTimestamp." << std::endl;
    return 1;
  }

  auto fbsLog = common::log::diagnosis::fbs::GetDiagnosisIdentificationWithTimestamp(logData.getData());
  std::string mainDiagnosisString = std::string(fbsLog->diagnosisIdentification()->mainDiagnosisNumber()->c_str());
  std::string detailedDiagnosisString = std::string(fbsLog->diagnosisIdentification()->detailedDiagnosisNumber()->c_str());
  std::string entity = std::string(fbsLog->diagnosisIdentification()->entity()->c_str());
  std::string timestamp = std::string(fbsLog->timestamp()->c_str());

  std::cout << "Content of actual highest priority log: {" << mainDiagnosisString << "," << detailedDiagnosisString << "," << entity << "}(" << timestamp << ")" << std::endl;

  //////////////////////////////////////////////////////////////////////////////////
  std::cout << "Get pending diagnostics list" << std::endl;
  //////////////////////////////////////////////////////////////////////////////////

  comm::datalayer::Variant listData;

  dlResult = client->readSync("diagnosis/get/actual/list", &listData);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "Get actual list failed with error code '" << dlResult.toString() << "'. Could not read error and warning list." << std::endl;
    return 1;
  }

  if (comm::datalayer::STATUS_FAILED(listData.verifyFlatbuffers(common::log::diagnosis::fbs::VerifyListDiagnosisIdentificationWithTimestampBuffer)))
  {
    std::cerr << "Invalid data type for listData. Expected flatbuffer type ListDiagnosisIdentificationWithTimestamp." << std::endl;
    return 1;
  }

  auto fbsList = common::log::diagnosis::fbs::GetListDiagnosisIdentificationWithTimestamp(listData.getData());
  auto fbsListSize = fbsList->listDiagnosisIdentificationWithTimestamp()->size();

  std::cout << "Content of actual list:" << std::endl;
  for (std::uint32_t listElement = 0; listElement < fbsListSize; ++listElement)
  {
    std::string mainDiagnosisString = std::string(fbsList->listDiagnosisIdentificationWithTimestamp()->Get(listElement)->diagnosisIdentification()->mainDiagnosisNumber()->c_str());
    std::string detailedDiagnosisString = std::string(fbsList->listDiagnosisIdentificationWithTimestamp()->Get(listElement)->diagnosisIdentification()->detailedDiagnosisNumber()->c_str());
    std::string entity = std::string(fbsList->listDiagnosisIdentificationWithTimestamp()->Get(listElement)->diagnosisIdentification()->entity()->c_str());
    std::string timestamp = std::string(fbsList->listDiagnosisIdentificationWithTimestamp()->Get(listElement)->timestamp()->c_str());

    std::cout << "{" << mainDiagnosisString << "," << detailedDiagnosisString << "," << entity << "}(" << timestamp << ")" << std::endl;
  }

  //////////////////////////////////////////////////////////////////////////////////
  std::cout << "Verify logs in pending diagnostics list" << std::endl;
  //////////////////////////////////////////////////////////////////////////////////

  struct DiagIdentification
  {
    std::string mainDiag;
    std::string detailedDiag;
    std::string entity;
  };

  std::vector<DiagIdentification> listDiagnosisIdentification =
  {
      {"0E0E0003", "00000001", "scheduler/watchdog"},
      {"0E0F2001", "00000001", "motion/axs/z3"},
      {"0E0F6001", "00000001", "notExisting"},
  };

  for (const auto& listElement : listDiagnosisIdentification)
  {
    std::cout << "Verify state of log {" << listElement.mainDiag << "," << listElement.detailedDiag << "," << listElement.entity << "}: ";

    comm::datalayer::Variant dataVerifyLog;
    auto diagnosisIdentification = common::log::diagnosis::fbs::CreateDiagnosisIdentificationDirect(builder, listElement.mainDiag.c_str(), listElement.detailedDiag.c_str(), listElement.entity.c_str());

    builder.Finish(diagnosisIdentification);
    dataVerifyLog.shareFlatbuffers(builder);

    dlResult = client->readSync("diagnosis/get/actual/list/verify-log", &dataVerifyLog);
    if (comm::datalayer::STATUS_FAILED(dlResult))
    {
      std::cerr << "Verify specific log failed with error code " << dlResult.toString() << std::endl;
      return 1;
    }

    if (comm::datalayer::STATUS_FAILED(dataVerifyLog.verifyFlatbuffers(common::log::diagnosis::fbs::VerifyPendingDiagnosticStateBuffer)))
    {
      std::cerr << "Invalid data type for dataVerifyLog. Expected flatbuffer type PendingDiagnosticState." << std::endl;
      return 1;
    }

    std::cout << common::log::diagnosis::fbs::EnumNameState(common::log::diagnosis::fbs::GetPendingDiagnosticState(dataVerifyLog.getData())->pendingDiagnosticState()) << std::endl;
  }

  //////////////////////////////////////////////////////////////////////////////////
  std::cout << "Confirm errors" << std::endl;
  //////////////////////////////////////////////////////////////////////////////////

  std::cout << "Confirm all errors" << std::endl;
  dlResult = client->writeSync("diagnosis/confirm/all-errors", nullptr);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "Confirm all errors failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  std::cout << "Confirm specific error (but before: reset error (0E0F6001, 00000001, -), so that it is able to confirm)" << std::endl;
  logParameters = common::log::diagnosis::fbs::CreateLogParametersDirect(builder, "0E0F6001", "00000001", nullptr, nullptr, "datalayerdiagnosis", "motion.core", common::log::diagnosis::getFileName(__FILE__), __FUNCTION__, __LINE__, "");

  builder.Finish(logParameters);
  dataSet.shareFlatbuffers(builder);

  dlResult = client->writeSync("diagnosis/set/reset-active", &dataSet);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "reset-active failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  auto diagnosisIdentification = common::log::diagnosis::fbs::CreateDiagnosisIdentificationDirect(builder, "0E0F6001", "00000001", "");

  builder.Finish(diagnosisIdentification);
  dataSet.shareFlatbuffers(builder);

  dlResult = client->writeSync("diagnosis/confirm/error", &dataSet);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "Confirm specific error failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  //////////////////////////////////////////////////////////////////////////////////
  std::cout << "Get diagnostic texts" << std::endl;
  //////////////////////////////////////////////////////////////////////////////////

  std::string mainDiagnosisNumber = "0E0F0001";
  std::string detailedDiagnosisNumber = "00000001";
  comm::datalayer::Variant dataGetText;

  std::cout << "Get main diagnostic text of number " << mainDiagnosisNumber << std::endl;
  auto getMainTextElement = common::log::diagnosis::fbs::CreateGetMainDiagnosisTextDirect(builder, mainDiagnosisNumber.c_str());
  builder.Finish(getMainTextElement);
  dataGetText.shareFlatbuffers(builder);

  dlResult = client->readSync("diagnosis/get/text/main", &dataGetText);

  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "read failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  if (comm::datalayer::VariantType::STRING != dataGetText.getType())
  {
    std::cerr << "Expected '" << comm::datalayer::DATALAYER_TYPE_STRING << "' as result type, but got '" << dataGetText.typeAsCharString() << "'" << std::endl;
    return 1;
  }

  std::string diagnosisText = static_cast<const char*>(dataGetText);
  std::cout << "Diagnosis text of main diagnosis number " << mainDiagnosisNumber << ":" << diagnosisText << std::endl;

  std::cout << "Get detailed diagnostic text of number " << detailedDiagnosisNumber << " with related main diagnostic number " << mainDiagnosisNumber << std::endl;
  auto getDetailedTextElement = common::log::diagnosis::fbs::CreateGetDetailedDiagnosisTextDirect(builder, detailedDiagnosisNumber.c_str(), mainDiagnosisNumber.c_str());
  builder.Finish(getDetailedTextElement);
  dataGetText.shareFlatbuffers(builder);

  dlResult = client->readSync("diagnosis/get/text/detailed", &dataGetText);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "read failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  if (comm::datalayer::VariantType::STRING != dataGetText.getType())
  {
    std::cerr << "Expected '" << comm::datalayer::DATALAYER_TYPE_STRING << "' as result type, but got '" << dataGetText.typeAsCharString() << "'" << std::endl;
    return 1;
  }

  diagnosisText = static_cast<const char*>(dataGetText);
  std::cout << "Diagnosis text of detailed diagnosis number " << detailedDiagnosisNumber << " with related main diagnosis number " << mainDiagnosisNumber << ":" << diagnosisText << std::endl;


  //////////////////////////////////////////////////////////////////////////////////
  std::cout << "Reset remaining warning, and unregister diagnostics" << std::endl;
  //////////////////////////////////////////////////////////////////////////////////

  std::cout << "Reset warning (0E0E0003, 00000001, scheduler/watchdog)" << std::endl;
  logParameters = common::log::diagnosis::fbs::CreateLogParametersDirect(builder, "0E0E0003", "00000001", nullptr, "scheduler/watchdog", "datalayerdiagnosis", "common.scheduler", common::log::diagnosis::getFileName(__FILE__), __FUNCTION__, __LINE__, "");

  builder.Finish(logParameters);
  dataSet.shareFlatbuffers(builder);

  dlResult = client->writeSync("diagnosis/set/reset-active", &dataSet);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "reset-active failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  std::cout << "Unregister diagnostics with file path on PC or with file path in own app for interprocess communication (Alternative 2)" << std::endl;
  dlResult = metadataHelper.getFileContent(registrationFilePath, binaryFileContent);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "Failed to get type information of file " << registrationFilePath << ", error code: " << dlResult.toString() << std::endl;
    return 1;
  }

  dlResult = client->writeSync("diagnosis/registration/unregister-file-content", &binaryFileContent);
  if (comm::datalayer::STATUS_FAILED(dlResult))
  {
    std::cerr << "Unregister diagnostics failed with error code " << dlResult.toString() << std::endl;
    return 1;
  }

  return 0;
}

int main(void)
{
  // Prepare signal structure to interrupt the endless loop with Ctrl+C (SIGINT)
  std::signal(SIGINT, signalHandler);

  comm::datalayer::DatalayerSystem datalayerSystem;
  datalayerSystem.start(false);

  for (;;)
  {
    if (g_endProcess)
    {
      std::cout << "INFO Terminating process due to user input of Ctrl+C (signal SIGINT)" << std::endl;
      datalayerSystem.stop();
      return 0;
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));

    comm::datalayer::IClient2* client = getClient(datalayerSystem);
    if (client == nullptr)
    {
      client = getClient(datalayerSystem, "10.0.2.2", "boschrexroth", "boschrexroth", 8443);
    }

    if (client == nullptr)
    {
      if (!g_errorConnection)
      {
        g_errorConnection = true;
        std::cout << "ERROR Data Layer client connection failed - retry every 10 seconds" << std::endl;
      }

      continue;
    }

    g_errorConnection = false;

    int result = run(client);
    delete client;

    if (result == 0)
    {
      g_errorDiagnosis = false;
    }
    else
    {
      if (!g_errorDiagnosis)
      {
        g_errorConnection = true;
        std::cout << "ERROR Diagnosis handling failed - retry every 10 seconds" << std::endl;
      }
    }
  }
}
