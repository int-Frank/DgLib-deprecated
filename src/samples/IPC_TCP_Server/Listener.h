#ifndef LISTENER_H
#define LISTENER_H

#include "IPC_TCP_common.h"

namespace IPC
{
  class ServerState_On;

  class Listener
  {
  public:

    Listener(ServerState_On * a_pApp)
      : m_rApp(*a_pApp)
      , m_listenSocket(INVALID_SOCKET)
    {}

    bool Init(SocketData const &);
    void Run();

  private:

    SOCKET            m_listenSocket;
    ServerState_On &  m_rApp;
  };
}

#endif