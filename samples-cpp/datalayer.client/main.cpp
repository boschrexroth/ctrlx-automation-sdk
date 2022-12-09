/**
 * MIT License
 *
 * Copyright (c) 2020-2022 Bosch Rexroth AG
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

#include "datalayerclient.h"

int main()
{
#ifdef MY_DEBUG
  std::cout << "Raising SIGSTOP" << std::endl;
  raise(SIGSTOP);
  std::cout << "... Continue..." << std::endl;
#endif

  int counter = 1;
  for (;;)
  {
    std::cout << "Loop #" << counter++ << std::endl;

    // ctrlX CORE or ctrlX CORE virtual with network adapter:
    auto dataLayerClient = new DataLayerClient("192.168.1.1", "boschrexroth", "boschrexroth", 443);
    // ctrlX CORE virtual with port forwarding:
    // auto dataLayerClient = new DataLayerClient("10.0.2.2", "boschrexroth", "boschrexroth", 8443);

    dataLayerClient->Run();
    delete dataLayerClient; // Callback are no more called

    std::cout << "Sleeping..." << std::endl;
    sleep(2);
  }
}
