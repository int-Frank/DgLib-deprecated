#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <stdint.h>
#include <vector>

namespace Dg
{
  class Mediator
  {
  public:

    //Multiple thresds can call this. Needs a lock_guard.
    virtual void NewPayload(std::vector<char> const & a_payload) {}

  };

  class Client
  {
  public:

  public:

    Client();
    ~Client();

    //Obtain an unused port, and start listening on this port
    bool Startup(Mediator * a_mediator = nullptr);

    //Send the payload to current connections. Asynchronous.
    void SendToAll(std::vector<char> const & a_payload) {}

    //Will terminate all broadcasting and listening theads.
    void Shutdown();

  private:

    class PIMPL;
    PIMPL * m_pimpl;
  };
}

#endif