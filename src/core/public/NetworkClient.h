#ifndef NETWORKPeer_H
#define NETWORKPeer_H

#include <vector>

namespace Dg
{
  class Mediator
  {
  public:

    //Multiple thresds can call this. Needs a lock_guard.
    virtual void NewPayload(std::vector<char> const & a_payload) {}

  };

  class PeerNode
  {
  public:

  public:

    PeerNode();
    ~PeerNode();

    //Obtain port
    bool Init();

    //Will terminate all broadcasting and listening theads.
    //Release ports.
    void Shutdown();

    //Send the payload to current connections. Asynchronous.
    void SendToAll(std::vector<char> const & a_payload) {}

    //Call order should be:
    // 1. Start_RecieveConnections
    // 2. Start_GetPeers
    // 3. Start_AlertPeers

    void Start_RecieveConnections(Mediator * = nullptr);
    void Start_GetPeers();
    void Start_AlertPeers();

  private:

    class PIMPL;
    PIMPL * m_pimpl;
  };
}

#endif