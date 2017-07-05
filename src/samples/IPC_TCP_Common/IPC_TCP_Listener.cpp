#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <sstream>
#include <thread>
#include <atomic>

#include "IPC_TCP_Listener.h"
#include "DgTypes.h"
#include "IPC_TCP_MediatorBase.h"
#include "IPC_TCP_AcceptorBase.h"
#include "IPC_TCP_Logger.h"

namespace IPC
{
  namespace TCP
  {
    static void Run_AcceptConnection(AcceptorBase * a_pAcceptor,
                                     MediatorBase * a_pMediator,
                                     std::atomic<int> * a_activeThreads)
    {
      a_pAcceptor->Run(a_pMediator);
      delete a_pAcceptor;
      delete a_pMediator;
      (*a_activeThreads)--;
    }

    class Listener::PIMPL 
    {
    public:

      PIMPL()
        : listenSocket(INVALID_SOCKET)
        , activeThreads(0)
      {}

      ~PIMPL()
      {
      }

      static bool RequestIPInfoFromServer(SocketData const & server, SocketData & out);

      SOCKET            listenSocket;
      std::atomic<int>  activeThreads;

    private:

      PIMPL(PIMPL const & a_other);
      PIMPL & operator=(PIMPL const & a_other);

    };

    Listener::Listener()
      : m_pimpl(new PIMPL())
    {

    }

    Listener::~Listener()
    {
      Shutdown();
      delete m_pimpl;
      m_pimpl = nullptr;
    }

    bool Listener::InitByRequest(SocketData const & a_server, SocketData & a_result)
    {
      if (!PIMPL::RequestIPInfoFromServer(a_server, a_result))
      {
        return false;
      }
      Logger::Log(std::string("ASDSAD") + a_result.Get_IP(), Dg::LL_Debug);
      a_result.Set_IP(a_result.Get_IP());
      Port p;
      p.SetToInvalid();
      a_result.Set_Port(p);

      return InitSoft(a_result);
    }

    bool Listener::InitStrict(SocketData const & a_listenSocketData)
    {
      IPC::TCP::SocketData sd(a_listenSocketData);
      if (!InitSoft(sd))
      {
        return false;
      }
      if (sd != a_listenSocketData)
      {
        Shutdown();
        return false;
      }
      return true;
    }

    bool Listener::InitSoft(SocketData & a_sd)
    {
      std::stringstream ss;
      ss << "Setting up listening socket at " << a_sd.Get_IP() << ":" << a_sd.Get_Port().As_string() << "...";
      Logger::Log(ss.str(), Dg::LL_Info);

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
        Logger::Log(ss.str(), Dg::LL_Warning);
        return false;
      }

      m_pimpl->listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

      if (m_pimpl->listenSocket == INVALID_SOCKET)
      {
        std::stringstream ss;
        ss << "ServerAppBase() -> socket() failed with error: " << WSAGetLastError();
        Logger::Log(ss.str(), Dg::LL_Warning);
        freeaddrinfo(result);
        return false;
      }

      iResult = bind(m_pimpl->listenSocket, result->ai_addr, (int)result->ai_addrlen);
      if (iResult == SOCKET_ERROR)
      {
        std::stringstream ss;
        ss << "ServerAppBase() -> bind() failed with error: " << WSAGetLastError();
        Logger::Log(ss.str(), Dg::LL_Warning);
        freeaddrinfo(result);
        closesocket(m_pimpl->listenSocket);
        return false;
      }

      freeaddrinfo(result);

      if (listen(m_pimpl->listenSocket, SOMAXCONN) == SOCKET_ERROR)
      {
        std::stringstream ss;
        ss << "ServerAppBase() -> listen() failed with error: " << WSAGetLastError();
        Logger::Log(ss.str(), Dg::LL_Warning);
        closesocket(m_pimpl->listenSocket);
        return false;
      }

      //Set the port if we started with an invalid port
      if (!a_sd.Get_Port().IsValid())
      {
        SocketData sd;
        if (!GetSocketData(m_pimpl->listenSocket, sd))
        {
          Logger::Log("Listener initialized, but unable to retrieve port!", Dg::LL_Warning);
        }
        a_sd.Set_Port(sd.Get_Port());
      }

      Logger::Log("Done!", Dg::LL_Info);
      return true;
    }

    void Listener::Run(AcceptorBase * a_pAcceptor, MediatorBase * a_pMediator)
    {
      Logger::Log("Listening for clients...", Dg::LL_Info);

      while (true)
      {
        // Accept a client socket
        SOCKET clientSocket = INVALID_SOCKET;
        clientSocket = accept(m_pimpl->listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET)
        {
          std::stringstream ss;
          ss << "accept() failed with error: " << WSAGetLastError() << ". Client connection discarded.";
          Logger::Log(ss.str(), Dg::LL_Warning);
          continue;
        }

        if (a_pMediator->ShouldStop())
        {
          break;
        }

        MediatorBase * pMediator = a_pMediator->Clone();
        AcceptorBase * pAcceptor = a_pAcceptor->Clone();
        pAcceptor->SetSocket(clientSocket);
        m_pimpl->activeThreads++;
        try
        {
          std::thread t(Run_AcceptConnection, 
                        pAcceptor, 
                        pMediator, 
                        &m_pimpl->activeThreads);
          t.detach();
        }
        catch (...)
        {
          Logger::Log("Could not start acceptor thread!" , Dg::LL_Warning);
          m_pimpl->activeThreads--;
          delete pMediator;
          delete pAcceptor;
        }
      }
    }

    void Listener::Shutdown()
    {
      closesocket(m_pimpl->listenSocket);
      m_pimpl->listenSocket = INVALID_SOCKET;

      int elapsedTime = 0;
      int t = 0;
      while (m_pimpl->activeThreads > 0)
      {
        int const sleepTime = 200;
        Sleep(sleepTime);
        elapsedTime += sleepTime;
      }
    }

    bool Listener::PIMPL::RequestIPInfoFromServer(SocketData const & a_server, SocketData & a_out)
    {
      Message message;
      message.Set(E_IPAddressRequest, std::vector<char>());
      
      std::vector<char> responseData;

      if (!SendQuery(a_server, message.Serialize(), responseData))
      {
        Logger::Log("Request for ip failed: could not contact server.", Dg::LL_Warning);
        return false;
      }

      Message response;
      if (!response.Build(responseData))
      {
        Logger::Log("Request for ip failed: Server response gibberish.", Dg::LL_Warning);
        return false;
      }

      if (response.header.ID != E_IPAddressResponse)
      {
        std::stringstream ss;
        ss << "Request for ip failed: incorrect response header ID: " << response.header.ID
          << ". Expected: " << E_IPAddressResponse;
        Logger::Log(ss.str(), Dg::LL_Warning);
        return false;
      }

      a_out.Build(response.payload.data());

      return true;
    }
  }
}