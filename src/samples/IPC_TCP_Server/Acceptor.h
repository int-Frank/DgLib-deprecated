#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "IPC_TCP_common.h"

namespace IPC
{
  class TCP_Server;

  class Acceptor
  {
  public:

    Acceptor(TCP_Server * a_pApp)
      : m_rApp(*a_pApp)
    {}

    bool Init(SOCKET);
    void Run();

  private:

    void Handle_DeregisterClient();
    void Handle_RegisterClient();
    void Handle_Dispatch();
    void Handle_IPAddressRequest();

  private:

    SOCKET        m_clientSocket;
    Message       m_message;
    TCP_Server &  m_rApp;
  };
}

#endif