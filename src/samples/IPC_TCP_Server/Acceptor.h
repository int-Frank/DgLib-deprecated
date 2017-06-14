#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "IPC_TCP_AcceptorBase.h"
#include "IPC_TCP_common.h"

class ServerState_On;

namespace IPC
{
  namespace TCP
  {
    class MediatorBase;
  }
}

class Acceptor : public IPC::TCP::AcceptorBase
{
public:

  Acceptor(ServerState_On * a_pApp)
    : m_rApp(*a_pApp)
  {}

  bool Init(SOCKET);
  void Run(IPC::TCP::MediatorBase *);

  Acceptor * Clone() const;

private:

  void Handle_DeregisterClient();
  void Handle_RegisterClient();
  void Handle_Dispatch(IPC::TCP::MediatorBase *);
  void Handle_IPAddressRequest();

private:

  SOCKET             m_clientSocket;
  IPC::TCP::Message  m_message;
  ServerState_On &   m_rApp;
};

#endif