#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <map>
#include <string>

#define BUILD_DLL

#include "DgTypes.h"
#include "IPC_TCP_common.h"
#include "IPC_TCP_Logger.h"
#include "DgStringFunctions.h"
#include "IPC_TCP_PluginInterface.h"

using IPC::TCP::Logger::Log;
using namespace IPC::TCP;

class DispatcherTCP : public IIPC_Dispatcher
{
public:
  DispatcherTCP()
  {}

  ~DispatcherTCP() {}

  bool Init(void const * a_pConfigMap)
  {
    std::map <std::string, std::string> const * pConfig =
      reinterpret_cast<std::map<std::string, std::string> const *>(a_pConfigMap);

    auto it_port = pConfig->find("server_port");
    if (it_port == pConfig->end())
    {
      Log("Listen port not set in config file.", Dg::LL_Error);
      return false;
    }

    auto it_ipAddr = pConfig->find("server_ip");
    if (it_ipAddr == pConfig->end())
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

  void Dispatch(char const * a_data, int a_size)
  {
    std::vector<char> payload(a_data, a_data + size_t(a_size));
    Message message;
    message.Set(E_Dispatch, payload);
    if (Send(m_serverSocketData, message.Serialize()))
    {
      std::stringstream ss;
      ss << "Dispatch successful. " << message.payload.size() << " bytes sent.";
      Log(ss.str(), Dg::LL_Info);
    }
    else
    {
      Log("Dispatch failed.", Dg::LL_Warning);
    }
  }

private:

  SocketData m_serverSocketData;
};

IIPC_Dispatcher * ipcCreateDispatcher()
{
  return new DispatcherTCP();
}

void ipcReleaseDispatcher(IIPC_Dispatcher * a_pDispatcher)
{
  delete a_pDispatcher;
}