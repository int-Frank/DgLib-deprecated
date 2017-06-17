#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define BUILD_DLL

#include "DgTypes.h"
#include "IPC_TCP_common.h"
#include "IPC_TCP_Logger.h"
#include "IPC_TCP_Plugin_common.h"
#include "DgStringFunctions.h"
#include "DgIPC.h"

using IPC::TCP::Logger::Log;
using namespace IPC::TCP;

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
      Log("Listen port not set in config file.", Dg::LL_Error);
      return false;
    }

    auto it_ipAddr = a_config.find("server_ip");
    if (it_ipAddr == a_config.end())
    {
      Log("IP address not set in config file.", Dg::LL_Error);
      return false;
    }

    Port port;
    if (!port.Set(it_port->second))
    {
      std::stringstream ss;
      ss << "Invalid port number: " << it_port->second;
      Log(ss.str(), Dg::LL_Error);
      return false;
    }

    m_serverSocketData.Set_IP(it_ipAddr->second);
    m_serverSocketData.Set_Port(port);

    return true;
  }

  bool Shutdown()
  {
    return true;
  }

  void Dispatch(std::vector<char> const & a_payload)
  {
    if (Send(m_serverSocketData, a_payload))
    {
      Log("Dispatch successful.", Dg::LL_Info);
    }
    else
    {
      Log("Dispatch failed.", Dg::LL_Warning);
    }
  }

private:

  SocketData m_serverSocketData;
};

ipcDispatcherBase * ipcGetDispatcher()
{
  return new DispatcherTCP();
}