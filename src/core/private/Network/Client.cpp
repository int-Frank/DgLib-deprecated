#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <thread>
#include <mutex>

#include "NetworkClient.h"
#include "NetworkCommon.h"

#include "DgDoublyLinkedList.h"

namespace Dg
{
  struct SocketAddress
  {
    union
    {
      struct Socket
      {
        union
        {
          uint8_t   ip4_8[4];
          uint32_t  ip4_32;
        };
        uint16_t  port;
      } socket;
      uint64_t data;
    };
  };

  class PeerInfo
  {
  public:

    bool operator==(PeerInfo  const & a_other) { return socketAddress.data == a_other.socketAddress.data; }

    SocketAddress socketAddress;
    int32_t deadTime;
  };

  class PeerHandler
  {
  public:

    void AddPeer(PeerInfo const & a_data)
    {
      std::lock_guard<std::mutex> lock(mutexPeers);

      auto it = find(peers.begin(), peers.end(), a_data);
      if (it == peers.end())
      {
        peers.push_back(a_data);
      }
      else
      {
        it->deadTime = 0;
      }
    }

    void CleanPeerList(int32_t a_dt)
    {
      std::lock_guard<std::mutex> lock(mutexPeers);

      for (auto it = peers.end(); it != peers.begin();)
      {
        it--;
        it->deadTime += a_dt;
        if (it->deadTime > 5000)
        {
          it = peers.erase(it);
          it--;
        }
      }
    }

    DoublyLinkedList<PeerInfo> GetPeers()
    {
      std::lock_guard<std::mutex> lock(mutexPeers);
      return peers;
    }

  private:

    std::mutex                      mutexPeers;
    DoublyLinkedList<PeerInfo>  peers;
  };

  class PeerNode::PIMPL
  {
  public:

    PIMPL()
      : pMediator(nullptr)
      , shouldClose(false)
      , portRecieve(0)
    {}

    Mediator *    pMediator;
    bool          shouldClose;

    USHORT        portRecieve;

    std::thread   thread_RecieveConnections;
    std::thread   threadSend;
    std::thread   thread_AlertPeers;
    std::thread   thread_GetPeers;

    PeerHandler   peers;
  };

  static void Run_AlertPeers(bool const * a_shouldClose,
                             PeerHandler * a_peerHandler)
  {
    //Broadcast ClientInfo

    //Check if any peers are out of time.
  }

  static void Run_GetPeers(bool const & a_shouldClose,
                           PeerHandler & a_peerHandler)
  {

  }

  static void Run_SendToAll(std::vector<char> a_payload,
                            DoublyLinkedList<PeerInfo> a_peers)
  {

  }

  static void Run_RecieveConnections(bool const & a_shouldClose,
                                     PeerNode & a_node)
  {

  }

  PeerNode::PeerNode()
    : m_pimpl(new PIMPL())
  {

  }

  PeerNode::~PeerNode()
  {
    Shutdown();
  }

  void PeerNode::Shutdown()
  {
    m_pimpl->shouldClose = true;
    
    //AlertPeers thread should close by itself

    //Send dummy packet to broadcast address to wake up the GetPeers thread

    //Send dummy packet to this peers recieving address to wake up the RecieveConnections thread

    //Wait to join threads

    //Close sockets
  }

  bool PeerNode::Init()
  {
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    int iResult = getaddrinfo(NULL, "0", &hints, &result);
    if (iResult != 0) 
    {
      std::cout << "getaddrinfo failed: " << iResult;
      return false;
    }

    SOCKET ListenSocket = INVALID_SOCKET;

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) 
    {
      std::cout << "Error at socket(): " << WSAGetLastError();
      freeaddrinfo(result);
      return false;
    }

    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) 
    {
      std::cout << "bind failed with error: " << WSAGetLastError();
      freeaddrinfo(result);
      closesocket(ListenSocket);
      return false;
    }

    freeaddrinfo(result);

    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) 
    {
      std::cout << "Listen failed with error: " << WSAGetLastError();
      closesocket(ListenSocket);
      return false;
    }

    struct sockaddr_in sin;
    int addrlen = sizeof(sin);
    if (!(getsockname(ListenSocket, (struct sockaddr *)&sin, &addrlen) == 0 &&
        sin.sin_family == AF_INET &&
        addrlen == sizeof(sin)))
    {
      std::cout << "getsockname failed with error: " << WSAGetLastError();
      closesocket(ListenSocket);
      return false;
    }

    m_pimpl->portRecieve = sin.sin_port;

    //May not need to store ip address. Perhaps when the peer recieves a broadcast message
    //we can get the ip from the message.
  }

  void PeerNode::Start_AlertPeers()
  {
    m_pimpl->thread_AlertPeers = std::thread(Run_AlertPeers, &m_pimpl->shouldClose, &m_pimpl->peers);
  }

  void PeerNode::Start_GetPeers()
  {
    //m_pimpl->thread_GetPeers = std::thread(Run_GetPeers, m_pimpl->shouldClose, m_pimpl->peers);
  }

  void PeerNode::Start_RecieveConnections(Mediator *)
  {
    //m_pimpl->thread_RecieveConnections = std::thread(Run_RecieveConnections, this);
  }
}