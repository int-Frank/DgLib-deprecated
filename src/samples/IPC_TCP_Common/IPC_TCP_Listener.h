#ifndef IPC_TCP_LISTENER_H
#define IPC_TCP_LISTENER_H

#include "IPC_TCP_common.h"

namespace IPC
{
  namespace TCP
  {
    class MediatorBase;
    class AcceptorBase;

    class Listener
    {
    public:

      Listener(AcceptorBase * a_pAcceptor);
      ~Listener();
      Listener(Listener const &);
      Listener & operator=(Listener const &);

      //Call ONE of the Init functions.

      //Use this if you want to use a specific ip and port to listen on. This
      //method will try to open a use the input socket data, but may change.
      //If so the method will update listenSocketData. In particular, if the
      //input port is Invalid, a port will be chosen.
      bool InitSoft(SocketData & listenSocketData);

      //Will fail if unable to create a socket at the desired ip and port.
      bool InitStrict(SocketData const & listenSocketData);

      //Use this if you want the listener to decide the ip and port.
      //NOTE: The listener will need to communicate with a
      //server to query its ip address.
      bool InitByRequest(SocketData const & server, SocketData & result);

      void Shutdown();
      void Run();

    private:

      class PIMPL;
      PIMPL * m_pimpl;

    };
  }
}


#endif