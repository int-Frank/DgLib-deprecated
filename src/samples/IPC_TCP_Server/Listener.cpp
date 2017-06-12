
#include <sstream>
#include <thread>

#include "ServerState_On.h"
#include "Listener.h"
#include "DgTypes.h"
#include "Acceptor.h"

namespace IPC
{
  static void Run_AcceptConnection(ServerState_On * a_pApp, SOCKET a_socket)
  {
    Acceptor acceptor(a_pApp);
    if (acceptor.Init(a_socket))
    {
      acceptor.Run();
    }
  }

  bool Listener::Init(SocketData const & a_sd)
  {
    std::stringstream ss;
    ss << "Setting up server at " << a_sd.Get_IP() << ":" << a_sd.Get_Port().As_string() << "...";
    m_rApp.LogToOutputWindow(ss.str(), Dg::LL_Info);

    m_listenSocket = IPC::CreateListenSocket(a_sd.Get_Port().As_string().c_str(),
                                             a_sd.Get_IP().c_str());
    if (m_listenSocket == SOCKET_ERROR)
    {
      m_rApp.LogToOutputWindow("Unable to create listener!", Dg::LL_Error);
      m_listenSocket = INVALID_SOCKET;
      return false;
    }

    m_rApp.LogToOutputWindow("Done!", Dg::LL_Info);
    return true;
  }

  void Listener::Run()
  {
    m_rApp.LogToOutputWindow("Listening for clients...", Dg::LL_Info);

    while (true)
    {
      // Accept a client socket
      SOCKET clientSocket = INVALID_SOCKET;
      clientSocket = accept(m_listenSocket, NULL, NULL);
      if (clientSocket == INVALID_SOCKET)
      {
        std::stringstream ss;
        ss << "accept() failed with error: " << WSAGetLastError() << ". Client connection discarded.";
        m_rApp.LogToOutputWindow(ss.str(), Dg::LL_Warning);
        continue;
      }


      //DEBUG
      //sockaddr_in sin;
      //socklen_t addr_len = sizeof(sin);
      //int res = getpeername(ClientSocket, (struct sockaddr *) &sin, &addr_len);


      if (m_rApp.ShouldStop())
      {
        break;
      }

      std::thread(Run_AcceptConnection, &m_rApp, clientSocket);
    }

    closesocket(m_listenSocket);
  }
}