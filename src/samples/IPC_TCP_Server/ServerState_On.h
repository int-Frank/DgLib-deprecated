#ifndef SERVERSTATE_ON_H
#define SERVERSTATE_ON_H

#include <windows.h>
#include <thread>
#include <atomic>

#include "IPC_TCP_common.h"
#include "ClientHandler.h"
#include "ServerStateBase.h"
#include "DgTypes.h"

namespace IPC
{
  class ServerState_On : public ServerStateBase
  {
  public:

    ServerState_On(TCP_Server * a_pApp, SocketData const &);

    ~ServerState_On();

    void RegisterThread();
    void DeregisterThread();
    void RegisterClient(SocketData const &);
    void DeregisterClient(SocketData const &);
    std::vector<SocketData> GetClientList();
    bool ShouldStop() const;
    void ListenerRunning(bool);

  private:

    static int const s_textBufLen = 64;

  private:

    std::atomic<int>  m_activeThreads;
    std::atomic<bool> m_shouldStop;
    std::atomic<bool> m_listenerRunning;
    ClientHandler     m_clientHandler;
    std::thread       m_listenThread;
    SocketData        m_listenSocketData;
  };

}

#endif