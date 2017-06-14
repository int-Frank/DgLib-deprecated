#ifndef IPC_TCP_MEDIATOR_H
#define IPC_TCP_MEDIATOR_H

#include <string>

namespace IPC
{
  namespace TCP
  {
    class MediatorBase
    {
    public:
      virtual ~MediatorBase() {}
      virtual bool ShouldStop() { return false; }
      virtual MediatorBase * Clone() { return new MediatorBase(); }
    };
  }
}
#endif