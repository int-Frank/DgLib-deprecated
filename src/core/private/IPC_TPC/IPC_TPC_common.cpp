
#include <sstream>

#define BUILD_DLL

#include "DgIPC.h"
#include "IPC_TPC_common.h"

namespace Dg
{
  namespace IPC
  {
    std::string GetUsage()
    {
      std::stringstream ss;
      ss << "Uses TCP protocol to dispatch data a server, and then on to registered clients."
        << " Payloads will always contain a header. The header will have 2 fields:\n"
        << "  ID   : int, 2\n"
        << "  Size : int, 4\n"
        << "Possible values for ID are:\n"
        << "  " << E_IPAddressRequest << " : IP address query\n"
        << "  " << E_RegisterClient << " : Register a client\n"
        << "  " << E_ClientExiting << " : Notification that a client is closing\n"
        << "  " << E_Dispatch << " : Dispatch payload to all registered clients\n"
        << "Payload formats:\n"
        << "  ID : " << E_IPAddressRequest << "\n"
        << "    int, 2 - port to send response to.\n"
        << "And so on..."
        ;
      return ss.str();
    }

    std::string GetName()
    {
      return std::string("ICP_TCP");
    }
  }
}