#ifndef IPC_TCP_PLUGIN_MEDIATOR_H
#define IPC_TCP_PLUGIN_MEDIATOR_H

#include "IPC_TCP_Logger.h"
#include "IPC_TCP_MediatorBase.h"

using IPC::TCP::Logger::Log;
using namespace IPC::TCP;

class ReceiverTCP;

class Mediator : public MediatorBase
{
public:

  Mediator(ReceiverTCP * a_pReceiver)
    : m_pReceiverTCP(a_pReceiver)
  {}

  Mediator(Mediator const &);

  ~Mediator() {}

  bool ShouldStop();

  Mediator * Clone();

private:

  ReceiverTCP * m_pReceiverTCP;
};

#endif