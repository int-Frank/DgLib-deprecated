#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include "DgIPC.h"
#include "IPC_TCP_common.h"
#include "DgStringFunctions.h"

//namespace Dg
//{
//  namespace IPC
//  {
//    void LogDefault(std::string const & a_str, int a_logLevel)
//    {
//      std::cout << a_str << ": " << a_logLevel << "\n";
//    }
//
//    void(*g_Log)(std::string const &, int) = LogDefault;
//
//    bool Init(void(*a_Log)(std::string const &, LogLevel))
//    {
//      g_Log = a_Log;
//      ::IPC::SetLoggingFunction(a_Log);
//    }
//
//    class DispatcherTCP : public DispatcherBase
//    {
//    public:
//      DispatcherTCP()
//      {}
//
//      ~DispatcherTCP() {}
//
//      bool Init(std::map<std::string, std::string> const & a_config)
//      {
//        auto it_port = a_config.find("server_port");
//        if (it_port == a_config.end())
//        {
//          g_Log("listen port not set in config file.", Dg::LL_Error);
//          return false;
//        }
//
//        auto it_ipAddr = a_config.find("server_ip");
//        if (it_ipAddr == a_config.end())
//        {
//          g_Log("ip address not set in config file.", Dg::LL_Error);
//          return false;
//        }
//
//        Port::PortType portAsNumber(0);
//        if (!Dg::StringToNumber(portAsNumber, it_port->second, std::dec))
//        {
//          std::stringstream ss;
//          ss << "ServerAppBase() -> Invalid port number: " << it_port->second;
//          g_Log(ss.str(), Dg::LL_Error);
//          return false;
//        }
//
//        m_serverSocketData.Set_IP(it_ipAddr->second);
//        m_serverSocketData.Set_Port(Port(portAsNumber));
//
//        if (!InitWinsock())
//        {
//          g_Log("winsock failed to initialise.", Dg::LL_Error);
//          return false;
//        }
//        return true;
//      }
//
//      void Shutdown()
//      {
//        ShutdownWinsock();
//      }
//
//      void Dispatch(std::vector<char> const & a_payload, std::atomic<bool> const & a_shouldQuit)
//      {
//        Send(m_serverSocketData, a_payload, a_shouldQuit);
//      }
//
//    private:
//
//      SocketData m_serverSocketData;
//    };
//
//    DispatcherBase * GetDispatcher()
//    {
//      return new DispatcherTCP();
//    }
//  }
//}