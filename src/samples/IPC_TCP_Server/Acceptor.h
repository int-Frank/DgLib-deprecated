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

  Acceptor(Acceptor const & a_other)
    : AcceptorBase(a_other)
    , m_rApp(a_other.m_rApp)
    , m_message(a_other.m_message)
  {}

  void Run(IPC::TCP::MediatorBase *);

  Acceptor * Clone() const;

private:

  void Handle_DeregisterClient();
  void Handle_RegisterClient();
  void Handle_Dispatch(IPC::TCP::MediatorBase *);
  void Handle_IPAddressRequest();

private:

  IPC::TCP::Message  m_message;
  ServerState_On &   m_rApp;
};

#endif