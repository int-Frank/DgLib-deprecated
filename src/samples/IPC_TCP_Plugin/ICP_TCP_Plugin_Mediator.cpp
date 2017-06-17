#include "IPC_TCP_Plugin_Mediator.h"
#include "IPC_TCP_Plugin_ReceiverTCP.h"

Mediator::Mediator(Mediator const & a_other)
  : MediatorBase(a_other)
  , m_pReceiverTCP(a_other.m_pReceiverTCP)
{

}

bool Mediator::ShouldStop()
{
  return m_pReceiverTCP->ShouldStop();
}

Mediator * Mediator::Clone()
{
  return new Mediator(*this);
}