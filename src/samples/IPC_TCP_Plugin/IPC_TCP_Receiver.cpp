#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include "DgTypes.h"
#include "IPC_TCP_common.h"
#include "IPC_TCP_Plugin_common.h"
#include "DgStringFunctions.h"
#include "DgIPC.h"

class ReceiverTCP;

static void Run_Listen(ReceiverTCP * a_pReceiver)
{

}

class ReceiverTCP : public ipcReceiverBase
{
public:
  ReceiverTCP()
  {}

  ~ReceiverTCP() {}

  bool Init(std::map<std::string, std::string> const & a_config,
            void(*NewDataCallback)(std::vector<char> const &))
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

    bool AquireIPAddress();

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

ipcReceiverBase * ipcGetReceiver()
{
  return new ReceiverTCP();
}