
#include <sstream>
#include <iostream>

#include "IPC_TCP_common.h"
#include "IPC_TCP_Plugin_common.h"
#include "DgIPC.h"

static void LogDefault(std::string const & a_str, int a_logLevel)
{
  std::cout << a_str << ": " << a_logLevel << "\n";
}

void(*g_Log)(std::string const &, int)(LogDefault);

bool ipcInit(void(*a_Log)(std::string const &, int))
{
  g_Log = a_Log;
  IPC::SetLoggingFunction(a_Log);
  return IPC::InitWinsock();
}

bool ipcShutdown()
{
  IPC::ShutdownWinsock();
  return true;
}

std::string ipcGetUsage()
{
  std::stringstream ss;
  ss << "Uses TCP protocol to dispatch data a server, and then on to registered clients."
    << " Payloads will always contain a header. The header will have 2 fields:\n"
    << "  ID   : int, 2\n"
    << "  Size : int, 4\n"
    << "Possible values for ID are:\n"
    << "  " << IPC::E_IPAddressRequest << " : IP address query\n"
    << "  " << IPC::E_IPAddressResponse << " : IP address query response\n"
    << "  " << IPC::E_RegisterClient << " : Register a client\n"
    << "  " << IPC::E_ClientExiting << " : Notification that a client is closing\n"
    << "  " << IPC::E_Dispatch << " : Dispatch payload to all registered clients\n"
    << "Payload formats:\n"
    << "  ID : " << IPC::E_IPAddressRequest << "\n"
    << "    int, 2 - port to send response to.\n"
    << "And so on..."
    ;
  return ss.str();
}

std::string ipcGetName()
{
  return std::string("ICP_TCP");
}