/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */


#include <iostream>
#include <thread>

int main()
{
  for(;;)
  {
    std::cout << "Hello World" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
  }

  return 0;
}
