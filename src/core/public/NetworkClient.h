#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <stdint.h>
#include <vector>

#include "DgDoublyLinkedList.h"
#include "DgIDManager.h"

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

    Client();
    ~Client();

    //Obtain an unused port, and start listening on this port
    bool Start(Mediator * a_mediator = nullptr);

    //Send the payload to current connections. Asynchronous.
    void Send(std::vector<char> const & a_payload) {}

    //Will terminate all broadcasting and listening theads.
    void Shutdown();

  private:

    bool ReservePorts();

    void Start_Broadcast_Listener() {}
    void Start_Broadcast_Speaker() {}
    void Start_Client_Listener() {}

    void SendDummyPayloads() {}

    bool AssignPort(uint16_t &);

  private:

    Mediator *  m_pMediator;
    bool        m_shouldClose;

    struct Connection
    {
      uint16_t port;
      uint32_t ipAddress;
    };

    uint16_t  m_myRecievePort;
    uint16_t  m_mySendPort;
    DoublyLinkedList<Connection> m_peers;
  };
}

#endif