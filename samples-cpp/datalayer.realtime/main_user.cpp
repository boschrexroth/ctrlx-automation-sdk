/**
 * MIT License
 * 
 * Copyright (c) 2020-2021 Bosch Rexroth AG
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
 

#include "comm/datalayer/datalayer.h"
#include "comm/datalayer/datalayer_system.h"
#include "comm/datalayer/memory_map_generated.h"
#include <stdio.h>
#include <signal.h>
#include <thread>
#include <iostream>

//Creat end process flag witch is set to true if SIGINT is send
bool endProcess = false;
static void hdl(int sig, siginfo_t *siginfo, void *context)
{
  endProcess = true;
}

//Cleanup closes the memory and stop the datalayersystem
void cleanup(comm::datalayer::DatalayerSystem *datalayer,
             std::shared_ptr<comm::datalayer::IMemoryUser> input,
             std::shared_ptr<comm::datalayer::IMemoryUser> output)
{
  comm::datalayer::DlResult result;
  std::cout << "finish my work" <<std::endl;
  result = datalayer->factory()->closeMemory(input);
  if (comm::datalayer::STATUS_FAILED(result))
    std::cout << "Closing input memory failed with: " << result.toString() << std::endl;

  result = datalayer->factory()->closeMemory(output);
  if (comm::datalayer::STATUS_FAILED(result))
    std::cout << "Closing output memory failed with: " << result.toString() << std::endl;

  datalayer->stop();
  printf("\n");
  exit(0);
}

int main(int ac, char* av[])
{
  comm::datalayer::DlResult result;
  comm::datalayer::Variant data;
  comm::datalayer::DatalayerSystem datalayer;
  uint8_t *inData;
  uint8_t *outData;
  
  // Start datalayer without a broker - a broker already exists in automation snap
  datalayer.start(false); 

  // Read the Memory Map to take the revision number, the revision number is necessary to work with the memory
  comm::datalayer::IClient *client = datalayer.factory()->createClient(DL_IPC_AUTO);
  result = client->readSync("example/rt/output/map", &data);
  if (STATUS_FAILED(result))
  {
    std::cout << "Reading Memory Map failed with " << result.toString() << std::endl;
  }
  
  auto memMap = comm::datalayer::GetMemoryMap(data.getData());
  auto revision = memMap->revision();
  std::cout << "Revision Number of Memory Map: " << revision << std::endl;
  
  std::cout << "Opening some realtime memory" << std::endl;
  std::shared_ptr<comm::datalayer::IMemoryUser> input;
  std::shared_ptr<comm::datalayer::IMemoryUser> output;
  result = datalayer.factory()->openMemory(input, "example/rt/input");
  if (comm::datalayer::STATUS_FAILED(result))
    std::cout << "creation of input failed with: " << result.toString() << std::endl;
  result = datalayer.factory()->openMemory(output, "example/rt/output");
  if (comm::datalayer::STATUS_FAILED(result))
    std::cout << "creation of output failed with: " << result.toString() << std::endl;

  // Structure to interrupt the do while loop with SIGINT
  struct sigaction act;
  memset(&act, '\0', sizeof(act));
  act.sa_sigaction = &hdl;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGINT, &act, NULL);

  std::cout << "start producing some output data" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  do
  {
    result = input->beginAccess(inData, revision);
    if (STATUS_SUCCEEDED(result))
    {
      std::cout << "first byte of input: " << inData[0] << std::endl;
      input->endAccess();
    }
    else
    {
      std::cout << "input memory not present at the moment: " << result.toString() << std::endl;
    }
    result = output->beginAccess(outData, revision);
    if (STATUS_SUCCEEDED(result))
    {
      std::cout << "memory present -> increment output" << std::endl;
      size_t size = 0;
      if (comm::datalayer::STATUS_FAILED(output->getSize(size)))
        std::cout << "can't read size" << std::endl;
      for (uint32_t i=0; i < size; i++)
      {
        outData[i] = outData[i] + 1;
      }
      output->endAccess();
    }
    else
    {
      std::cout << "output memory not present at the moment: " << result.toString() << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  } while (!endProcess);

  // Cleanup closes the memory and stop the datalayersystem
  cleanup(&datalayer, input, output);

}