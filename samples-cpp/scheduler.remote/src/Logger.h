/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "Trace.h"
#include <filesystem>
#include <iostream>

#define CONSOLE_INFO(message, ...)                                                                                     \
    {                                                                                                                  \
        sdk_rt::ConsoleLogger::log(sdk_rt::LogLevel::Info, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__);  \
    }

#define CONSOLE_WARNING(message, ...)                                                                                  \
    {                                                                                                                  \
        sdk_rt::ConsoleLogger::log(sdk_rt::LogLevel::Warning, __FILE__, __FUNCTION__, __LINE__, message,               \
                                   ##__VA_ARGS__);                                                                     \
    }

#define CONSOLE_ERROR(message, ...)                                                                                    \
    {                                                                                                                  \
        sdk_rt::ConsoleLogger::log(sdk_rt::LogLevel::Error, __FILE__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__); \
    }

#define LOG_INFO(message, ...) TRACE_INFO(TRACE_UNIT_ID, message, ##__VA_ARGS__) CONSOLE_INFO(message, ##__VA_ARGS__)
#define LOG_WARNING(message, ...)                                                                                      \
    TRACE_WARNING(TRACE_UNIT_ID, message, ##__VA_ARGS__) CONSOLE_WARNING(message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...) TRACE_ERROR(TRACE_UNIT_ID, message, ##__VA_ARGS__) CONSOLE_ERROR(message, ##__VA_ARGS__)

namespace sdk_rt
{
enum class LogLevel
{
    Error = 1,
    Warning = 2,
    Info = 3
};

class ConsoleLogger
{
  private:
    // Reset console color
    static constexpr const char *reset = "\033[0m";

    // Green console color
    static constexpr const char *green = "\033[32m";

    // Yellow console color
    static constexpr const char *yellow = "\033[33m";

    // Red console color
    static constexpr const char *red = "\033[31m";

    inline static const bool isDebugEnvironment = [] {
        // Safely get the environment variable
        const char* snapEnvCStr = std::getenv("SNAP_ENV"); 

        // Check for nullptr before constructing std::string
        if (snapEnvCStr != nullptr) {
            return std::string(snapEnvCStr) == "DEBUG";
        }
        return false; // If SNAP_ENV is not set, it's not a debug environment
    }();

  public:
    inline static void log(LogLevel logLevel, const char *file, const char *functionName, std::uint32_t lineNumber,
                           const char *format, ...)
    {
        if (isDebugEnvironment == false)
        {
            return;
        }

        va_list args;
        va_start(args, format);
        char message[1024];
        vsprintf(message, format, args);

        std::ostringstream strStream;
        auto now = std::chrono::system_clock::now();
        auto formattedTime = std::chrono::system_clock::to_time_t(now);
        auto timestamp = std::put_time(std::localtime(&formattedTime), "%F %T");
        auto filePath = std::filesystem::relative(file, std::filesystem::current_path());

        switch (logLevel)
        {
        case LogLevel::Info:
            std::cout << green << timestamp << " [INFO] ";
            break;
        case LogLevel::Warning:
            std::cout << yellow << timestamp << " [WARN] ";
            break;
        case LogLevel::Error:
            std::cout << red << timestamp << " [ERROR] ";
            break;
        }

        std::cout << filePath.c_str() << "(" << lineNumber << "): " << functionName << "() " << message << reset
                  << '\n';
        va_end(args);
    }

    std::string timeStamp()
    {
        std::ostringstream strStream;
        auto now = std::chrono::system_clock::now();
        auto formattedTime = std::chrono::system_clock::to_time_t(now);
        strStream << std::put_time(std::localtime(&formattedTime), "%F %T") << " ";
        return strStream.str();
    }
};
} // namespace sdk_rt
