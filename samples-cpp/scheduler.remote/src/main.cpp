#include <csignal>
#include <iostream>
#include <thread>
#include "app_client.h"
#include "Logger.h"

// Global variable to safely communicate signal status between main thread & signal handler
volatile std::sig_atomic_t gSignalStatus;

void signalHandler(int signal)
{
  gSignalStatus = signal;
}

// Main function is the program entry point and primary execution loop
int main(int argc, char *argv[])
{
// Needed for remote debugging
#ifdef MY_DEBUG
  std::cout << "Starting 'raise(SIGSTOP)', waiting for debugger.." << std::endl;
  raise(SIGSTOP);
  std::cout << "Debugger connected, continuing program..." << std::endl;
#endif

  TRACE_INFO(TRACE_UNIT_ID, "Starting program");

  int signals[] = {SIGINT, SIGTERM, SIGABRT, SIGKILL};

  for (int x : signals)
  {
    std::signal(x, signalHandler);
  }

  // First parameter sets the Data Layer communication directory,
  // If empty, it defaults to $SNAP_DATA/.datalayer
  std::string path = (argc > 1) ? argv[1] : "";
  {
    // Register the signal handler for multiple signals
    AppClient myApp(path);

    // Main program loop keeps running until a signal is received
    TRACE_INFO(TRACE_UNIT_ID, "Waiting for task termination");
    while (gSignalStatus == 0)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    switch (gSignalStatus)
    {
    case SIGINT:
      std::cout << "Interrupt signal received\n";
      break;
    case SIGTERM:
      std::cout << "Termination signal received\n";
      break;
    case SIGABRT:
      std::cout << "Abort signal received\n";
      break;
    case SIGKILL:
      std::cout << "Kill signal received\n";
      break;
    default:
      std::cout << "Unknown signal received: " << gSignalStatus << "\n";
      break;
    }
  } // Destructor is called when exiting this scope
  TRACE_INFO(TRACE_UNIT_ID, "Exiting program");
  return 0;
}