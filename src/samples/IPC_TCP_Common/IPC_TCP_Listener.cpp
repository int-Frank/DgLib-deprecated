#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <sstream>
#include <thread>

#include "IPC_TCP_Listener.h"
#include "DgTypes.h"
#include "IPC_TCP_Mediator.h"
#include "IPC_TCP_AcceptorBase.h"

namespace IPC
{
  namespace TCP
  {
    static void Run_AcceptConnection(SOCKET a_socket, AcceptorBase * a_pAcceptor)
    {
      if (a_pAcceptor->Init(a_socket))
      {
        a_pAcceptor->Run();
      }
    }

    class Listener::PIMPL 
    {
    public:

      PIMPL(AcceptorBase * a_pAcceptor)
        : listenSocket(INVALID_SOCKET)
        , pAcceptor(a_pAcceptor)
      {}

      PIMPL(PIMPL const & a_other)
        : listenSocket(a_other.listenSocket)
        , pAcceptor(a_other.pAcceptor->Clone())
      {}

      ~PIMPL()
      {
        delete pAcceptor;
        pAcceptor = nullptr;
      }

      static bool RequestIPInfoFromServer(SocketData const & server, SocketData & out);

      AcceptorBase * pAcceptor;
      SOCKET         listenSocket;
    };

    Listener::Listener(AcceptorBase * a_pAcceptor)
      : m_pimpl(new PIMPL(a_pAcceptor))
    {

    }

    Listener::Listener(Listener const & a_other)
      : m_pimpl(new PIMPL(*a_other.m_pimpl))
    {

    }

    Listener::~Listener()
    {
      delete m_pimpl;
      m_pimpl = nullptr;
    }

    Listener & Listener::operator=(Listener const & a_other)
    {
      if (this != &a_other)
      {
        delete m_pimpl;
        m_pimpl = new PIMPL(*a_other.m_pimpl);
      }
      return *this;
    }

    bool Listener::Init(SocketData const & a_server, SocketData & a_result)
    {
      if (!PIMPL::RequestIPInfoFromServer(a_server, a_result))
      {
        return false;
      }

      a_result.Set_IP(a_result.Get_IP());
      Port p;
      p.SetToInvalid();
      a_result.Set_Port(p);

      return Init(a_result);
    }

    bool Listener::Init(SocketData & a_sd)
    {
      std::stringstream ss;
      ss << "Setting up listening socket at " << a_sd.Get_IP() << ":" << a_sd.Get_Port().As_string() << "...";
      Mediator::Log(ss.str(), Dg::LL_Info);

      struct addrinfo *result = NULL, *ptr = NULL, hints;

      ZeroMemory(&hints, sizeof(hints));
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;
      hints.ai_flags = AI_PASSIVE;

      int iResult(-1);

      if (a_sd.Get_Port().IsValid())
      {
        // Resolve the local address and port to be used by the server
        iResult = getaddrinfo(a_sd.Get_IP().c_str(),
                              a_sd.Get_Port().As_string().c_str(),
                              &hints, &result);
      }
      else
      {
        //Attempt to assign port
        iResult = getaddrinfo(a_sd.Get_IP().c_str(),
                              nullptr,
                              &hints, &result);
      }

      if (iResult != 0)
      {
        std::stringstream ss;
        ss << "ServerAppBase() -> getaddrinfo() failed with error: " << iResult;
        Mediator::Log(ss.str(), Dg::LL_Warning);
        return false;
      }

      m_pimpl->listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

      if (m_pimpl->listenSocket == INVALID_SOCKET)
      {
        std::stringstream ss;
        ss << "ServerAppBase() -> socket() failed with error: " << WSAGetLastError();
        Mediator::Log(ss.str(), Dg::LL_Warning);
        freeaddrinfo(result);
        return false;
      }

      iResult = bind(m_pimpl->listenSocket, result->ai_addr, (int)result->ai_addrlen);
      if (iResult == SOCKET_ERROR)
      {
        std::stringstream ss;
        ss << "ServerAppBase() -> bind() failed with error: " << WSAGetLastError();
        Mediator::Log(ss.str(), Dg::LL_Warning);
        freeaddrinfo(result);
        closesocket(m_pimpl->listenSocket);
        return false;
      }

      freeaddrinfo(result);

      if (listen(m_pimpl->listenSocket, SOMAXCONN) == SOCKET_ERROR)
      {
        std::stringstream ss;
        ss << "ServerAppBase() -> listen() failed with error: " << WSAGetLastError();
        Mediator::Log(ss.str(), Dg::LL_Warning);
        closesocket(m_pimpl->listenSocket);
        return false;
      }

      //Set the port if we started with an invalid port
      if (!a_sd.Get_Port().IsValid())
      {
        SocketData sd;
        if (!GetSocketData(m_pimpl->listenSocket, sd))
        {
          Mediator::Log("Listener initialized, but unable to retrieve port!", Dg::LL_Warning);
        }
        a_sd.Set_Port(sd.Get_Port());
      }

      Mediator::Log("Done!", Dg::LL_Info);
      return true;
    }

    void Listener::Run()
    {
      Mediator::Log("Listening for clients...", Dg::LL_Info);

      while (true)
      {
        // Accept a client socket
        SOCKET clientSocket = INVALID_SOCKET;
        clientSocket = accept(m_pimpl->listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET)
        {
          std::stringstream ss;
          ss << "accept() failed with error: " << WSAGetLastError() << ". Client connection discarded.";
          Mediator::Log(ss.str(), Dg::LL_Warning);
          continue;
        }


        //DEBUG
        //sockaddr_in sin;
        //socklen_t addr_len = sizeof(sin);
        //int res = getpeername(ClientSocket, (struct sockaddr *) &sin, &addr_len);


        if (Mediator::ShouldStop())
        {
          break;
        }

        std::thread(Run_AcceptConnection, clientSocket, m_pimpl->pAcceptor);
      }
    }

    void Listener::Shutdown()
    {
      closesocket(m_pimpl->listenSocket);
      m_pimpl->listenSocket = INVALID_SOCKET;
    }

    bool Listener::PIMPL::RequestIPInfoFromServer(SocketData const & a_server, SocketData & a_out)
    {
      Message message;
      message.Set(E_IPAddressRequest, std::vector<char>());
      
      std::vector<char> responseData;

      if (!SendQuery(a_server, message.Serialize(), responseData))
      {
        Mediator::Log("Request for ip failed.", Dg::LL_Warning);
        return false;
      }

      Message response;
      if (!response.Build(responseData))
      {
        Mediator::Log("Request for ip failed.", Dg::LL_Warning);
        return false;
      }

      if (response.header.ID != E_IPAddressResponse)
      {
        std::stringstream ss;
        ss << "Request for ip failed: incorrect response header ID: " << response.header.ID
          << ". Expected: " << E_IPAddressResponse;
        Mediator::Log(ss.str(), Dg::LL_Warning);
        return false;
      }

      return true;
    }
  }
}