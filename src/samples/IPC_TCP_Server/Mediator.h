#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "IPC_TCP_MediatorBase.h"

class ServerState_On;

class Mediator : public IPC::TCP::MediatorBase
{
public: 

  Mediator(ServerState_On const *);
  Mediator(Mediator const &);
  ~Mediator() {}
  bool ShouldStop();
  void HasStopped() {}
  Mediator * Clone();

private:

  ServerState_On const * m_pParent;
};

#endif