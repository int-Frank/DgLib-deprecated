
#include <string>
#include <string.h>
#include <iostream>

#define BUILD_DLL

#include "IPC_TCP_common.h"
#include "IPC_TCP_Logger.h"
#include "DgIPC.h"

static void LogDefault(char const * a_message, int a_logLevel)
{
  std::cout << a_logLevel << ": " << a_message << "\n";
}

static ipcLogCallback g_LogConverter(LogDefault);

static void LogDefaultString(std::string const & a_message, int a_logLevel)
{
  g_LogConverter(a_message.c_str(), a_logLevel);
}

bool ipcInit(ipcLogCallback a_Log)
{
  g_LogConverter = a_Log;
  return IPC::TCP::Init(LogDefaultString);
}

bool ipcShutdown()
{
  IPC::TCP::Shutdown();
  return true;
}

char * ipcGetUsage()
{
  return "Usage";
}

char * ipcGetName()
{
  return "ICP_TCP";
}