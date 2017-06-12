#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define BUILD_DLL

#include "DgIPC.h"
#include "DgTypes.h"
#include "IPC_TCP_common.h"
#include "DgStringFunctions.h"

static void LogDefault(std::string const & a_str, int a_logLevel)
{
  std::cout << a_str << ": " << a_logLevel << "\n";
}

static void(*g_Log)(std::string const &, int) = LogDefault;

bool Init(void(*a_Log)(std::string const &, int))
{
  g_Log = a_Log;
  ::IPC::SetLoggingFunction(a_Log);
}

class DispatcherTCP : public ipcDispatcherBase
{
public:
  DispatcherTCP()
  {}

  ~DispatcherTCP() {}

  bool Init(std::map<std::string, std::string> const & a_config)
  {
    auto it_port = a_config.find("server_port");
    if (it_port == a_config.end())
    {
      g_Log("Listen port not set in config file.", Dg::LL_Error);
      return false;
    }

    auto it_ipAddr = a_config.find("server_ip");
    if (it_ipAddr == a_config.end())
    {
      g_Log("IP address not set in config file.", Dg::LL_Error);
      return false;
    }

    ::IPC::Port port;
    if (!port.Set(it_port->second))
    {
      std::stringstream ss;
      ss << "Invalid port number: " << it_port->second;
      g_Log(ss.str(), Dg::LL_Error);
      return false;
    }

    m_serverSocketData.Set_IP(it_ipAddr->second);
    m_serverSocketData.Set_Port(port);

    if (!IPC::InitWinsock())
    {
      g_Log("winsock failed to initialise.", Dg::LL_Error);
      return false;
    }
    return true;
  }

  void Shutdown()
  {
    IPC::ShutdownWinsock();
  }

  void Dispatch(std::vector<char> const & a_payload)
  {
    if (IPC::Send(m_serverSocketData, a_payload))
    {
      g_Log("Dispatch successful.", Dg::LL_Info);
    }
    else
    {
      g_Log("Dispatch failed.", Dg::LL_Warning);
    }
  }

private:

  IPC::SocketData m_serverSocketData;
};

ipcDispatcherBase * ipcGetDispatcher()
{
  return new DispatcherTCP();
}


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