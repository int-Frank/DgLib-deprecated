
#include <sstream>
#include <iostream>

#define BUILD_DLL

#include "IPC_TCP_common.h"
#include "IPC_TCP_Logger.h"
#include "IPC_TCP_Plugin_common.h"
#include "DgIPC.h"

bool ipcInit(void(*a_Log)(std::string const &, int))
{
  return IPC::TCP::Init(a_Log);
}

bool ipcShutdown()
{
  IPC::TCP::Shutdown();
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
    << "  " << IPC::TCP::E_IPAddressRequest << " : IP address query\n"
    << "  " << IPC::TCP::E_IPAddressResponse << " : IP address query response\n"
    << "  " << IPC::TCP::E_RegisterClient << " : Register a client\n"
    << "  " << IPC::TCP::E_ClientExiting << " : Notification that a client is closing\n"
    << "  " << IPC::TCP::E_Dispatch << " : Dispatch payload to all registered clients\n"
    << "Payload formats:\n"
    << "  ID : " << IPC::TCP::E_IPAddressRequest << "\n"
    << "    int, 2 - port to send response to.\n"
    << "And so on..."
    ;
  return ss.str();
}

std::string ipcGetName()
{
  return std::string("ICP_TCP");
}