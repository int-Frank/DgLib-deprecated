#ifndef IPC_TCP_ACCEPTORBASE_H
#define IPC_TCP_ACCEPTORBASE_H

#include <windows.h>

class MediatorBase;

namespace IPC
{
  namespace TCP
  {
    class AcceptorBase
    {
    public:

      virtual ~AcceptorBase() {}

      virtual bool Init(SOCKET a_socket) { return true; }
      virtual void Run() {}
      virtual AcceptorBase * Clone() const { return new AcceptorBase(*this); }

    };
  }
}

#endif