
#include "IPC_TCP_AcceptorBase.h"
#include "IPC_TCP_common.h"

namespace IPC
{
  namespace TCP
  {
    AcceptorBase::AcceptorBase()
      : m_socket(INVALID_SOCKET)
    {}

    AcceptorBase::AcceptorBase(AcceptorBase const & a_other)
      : m_socket(a_other.m_socket)
    {

    }

    void AcceptorBase::SetSocket(SOCKET a_socket)
    {
      m_socket = a_socket;
    }

    AcceptorBase * AcceptorBase::Clone() const { return new AcceptorBase(*this); }
  }
}