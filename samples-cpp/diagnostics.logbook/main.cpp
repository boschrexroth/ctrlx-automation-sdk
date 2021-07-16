/**
 * MIT License
 * 
 * Copyright (c) 2021 Bosch Rexroth AG
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <iostream>
#include <systemd/sd-journal.h> // lib systemd required --> CMakeLists.txt

int main(int ac, char *av[])
{
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

  return 0;
}