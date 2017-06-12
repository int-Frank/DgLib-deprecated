#ifndef LISTENER_H
#define LISTENER_H

#include "IPC_TCP_common.h"

namespace IPC
{
  class TCP_Server;

  class Listener
  {
  public:

    Listener(TCP_Server * a_pApp)
      : m_rApp(*a_pApp)
      , m_listenSocket(INVALID_SOCKET)
    {}

    bool Init(SocketData const &);
    void Run();

  private:

    SOCKET        m_listenSocket;
    TCP_Server &  m_rApp;
  };
}

#endif