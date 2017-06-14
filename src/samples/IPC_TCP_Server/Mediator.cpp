#include "Mediator.h"
#include "ServerState_On.h"

Mediator::Mediator(ServerState_On const * a_pParent)
  : m_pParent(a_pParent)
{

}

Mediator::Mediator(Mediator const & a_other)
  : m_pParent(a_other.m_pParent)
{

}

bool Mediator::ShouldStop()
{
  return m_pParent->ShouldStop();
}

Mediator * Mediator::Clone()
{
  return new Mediator(*this);
}