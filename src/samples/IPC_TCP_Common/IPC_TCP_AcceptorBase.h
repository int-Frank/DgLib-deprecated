#ifndef IPC_TCP_ACCEPTORBASE_H
#define IPC_TCP_ACCEPTORBASE_H

#include <windows.h>

namespace IPC
{
  namespace TCP
  {
    class MediatorBase;

    class AcceptorBase
    {
    public:

      AcceptorBase() {}
      virtual ~AcceptorBase() {}

      virtual bool Init(SOCKET a_socket) { return true; }
      virtual void Run(MediatorBase *) {}
      virtual AcceptorBase * Clone() const { return new AcceptorBase(*this); }

    };
  }
}

#endif