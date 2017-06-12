#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "IPC_TCP_common.h"

class ServerState_On;

class Acceptor
{
public:

  Acceptor(ServerState_On * a_pApp)
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

  SOCKET            m_clientSocket;
  IPC::Message      m_message;
  ServerState_On &  m_rApp;
};

#endif