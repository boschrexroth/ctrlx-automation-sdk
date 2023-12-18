/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <thread>
#include <systemd/sd-journal.h> // lib systemd required --> CMakeLists.txt

int main()
{
  // Messages will be sent once!!!!!!!!!!!!!!
  std::cout << "This is an EMERGENCY message simply written to std::cout" << std::endl;
  std::cout << "This is an ALERT message simply written to std::cout" << std::endl;
  std::cout << "This is a CRITICAL message simply written to std::cout" << std::endl;
  std::cout << "This is an ERROR message simply written to std::cout" << std::endl;
  std::cout << "This is a WARNING message simply written to std::cout" << std::endl;
  std::cout << "This is a NOTICE message simply written to std::cout" << std::endl;
  std::cout << "This is an INFO message simply written to std::cout" << std::endl;
  std::cout << "This is a DEBUG message simply written to std::cout" << std::endl;

  sd_journal_print(LOG_EMERG, "%s", "This is an EMERGENCY message written with sd_journal_print() - Be careful: Will be sent to ALL consoles!");
  sd_journal_print(LOG_ALERT, "%s", "This is an ALERT message written with sd_journal_print()");
  sd_journal_print(LOG_CRIT, "%s", "This is a CRITICAL message written with sd_journal_print()");
  sd_journal_print(LOG_ERR, "%s", "This is an ERROR message written with sd_journal_print()");
  sd_journal_print(LOG_WARNING, "%s", "This is a WARNING message written with sd_journal_print()");
  sd_journal_print(LOG_NOTICE, "%s", "This is a NOTICE message written with sd_journal_print()");
  sd_journal_print(LOG_INFO, "%s", "This is an INFO message written with sd_journal_print()");
  sd_journal_print(LOG_DEBUG, "%s", "This is a DEBUG message written with sd_journal_print()");

  for(;;)
  {
    std::cout << "INFO Sleeping 30s..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(30));
  }

  return 0;
}
