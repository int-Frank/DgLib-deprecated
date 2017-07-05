#ifndef IPC_TCP_ACCEPTORBASE_H
#define IPC_TCP_ACCEPTORBASE_H

#include <windows.h>

namespace IPC
{
  namespace TCP
  {
    class MediatorBase;
    class SocketData;

    class AcceptorBase
    {
    public:

      AcceptorBase();
      AcceptorBase(AcceptorBase const & a_other);
      virtual ~AcceptorBase() {}

      void SetSocket(SOCKET);

      virtual void Run(MediatorBase *) {}
      virtual AcceptorBase * Clone() const;

    protected:

      SOCKET m_socket;
    };
  }
}

#endif