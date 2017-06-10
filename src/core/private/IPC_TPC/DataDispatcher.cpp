#include "DgIPC.h"
#include "IPC_TPC_common.h"
#include "DgStringFunctions.h"

namespace Dg
{
  namespace IPC
  {
    class DispatcherTCP : public DispatcherBase
    {
    public:
      DispatcherTCP()
      : Log(nullptr)
      {}

      ~DispatcherTCP() {}

      bool Init(std::map<std::string, std::string> const & a_config,
                void(*a_Log)(std::string const &, LogLevel))
      {
        Log = a_Log;

        auto it_port = a_config.find("server_port");
        if (it_port == a_config.end())
        {
          Log("listen port not set in config file.", Dg::LL_Error);
          return false;
        }

        auto it_ipAddr = a_config.find("server_ip");
        if (it_ipAddr == a_config.end())
        {
          Log("ip address not set in config file.", Dg::LL_Error);
          return false;
        }

        Port::PortType portAsNumber(0);
        if (!Dg::StringToNumber(portAsNumber, it_port->second, std::dec))
        {
          std::stringstream ss;
          ss << "ServerAppBase() -> Invalid port number: " << it_port->second;
          Log(ss.str(), Dg::LL_Error);
          return false;
        }

        m_serverSocketData.Set_IP(it_ipAddr->second);
        m_serverSocketData.Set_Port(Port(portAsNumber));

        if (!InitWinsock())
        {
          Log("winsock failed to initialise.", Dg::LL_Error);
          return false;
        }
        return true;
      }

      void Shutdown()
      {
        ShutdownWinsock();
      }

      void Dispatch(std::vector<char> const & a_payload)
      {
        Send(m_serverSocketData, a_payload);
      }
      
    private:

      void(*Log)(std::string const &, LogLevel);
      SocketData m_serverSocketData;
    };

    DispatcherBase * GetDispatcher()
    {
      return new DispatcherTCP();
    }
  }
}