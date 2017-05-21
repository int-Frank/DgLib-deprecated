#include <iostream>
#include <thread>
#include <mutex>

#include "NetworkClient.h"
#include "NetworkCommon.h"

#include "DgDoublyLinkedList.h"
#include "DgIDManager.h"

namespace Dg
{
  class __ClientInfo
  {
  public:

    bool operator==(__ClientInfo  const & a_other) { return data == a_other.data; }

    uint64_t data;
    int32_t deadTime;
  };

  class Client::PIMPL
  {
  public:

    PIMPL()
      : pMediator(nullptr)
      , shouldClose(false)
      , portRecieve(0)
      , portSend(0)
    {}

    void AddPeer(__ClientInfo const & a_data)
    {
      std::lock_guard<std::mutex> lock(mutexPeers);

      auto it = find(peers.begin(), peers.end(), a_data);
      if (it == peers.end())
      {
        it->deadTime = 0;
      }
      else
      {
        peers.push_back(a_data);
      }
    }

    void CleanPeers(int32_t a_dt)
    {
      std::lock_guard<std::mutex> lock(mutexPeers);

      for (auto it = peers.end(); it != peers.begin();)
      {
        it--;
        it->deadTime -= a_dt;
        if (it->deadTime < 0)
        {
          it = peers.erase(it);
          it--;
        }
      }
    }

    Mediator *                        pMediator;
    bool                              shouldClose;

    uint16_t                          portRecieve;
    uint16_t                          portSend;

    std::thread                       threadRecieve;
    std::thread                       threadSend;
    std::thread                       threadBroadcastSpeaker;
    std::thread                       threadBroadcastListener;

    std::mutex                        mutexPeers;
    DoublyLinkedList<__ClientInfo>  peers;
  };

  static void RunBroadcastSpeaker(Client * a_client)
  {
    if (a_client == nullptr) return;

    //Broadcast ClientInfo

    //Check if any peers are out of time.
  }

  static void RunBroadcastListener(Client * a_client)
  {
    if (a_client == nullptr) return;
  }

  static void RunSend(std::vector<char> const & a_payload)
  {

  }

  static void RunRecieve(Client * a_client)
  {
    if (a_client == nullptr) return;
  }

  static bool AssignPort(uint16_t & a_out)
  {
    ULONG64 resToken = { 0 };
    for (USHORT i = 5000; i < 65535; i++)
    {
      unsigned long status = CreatePersistentTcpPortReservation(i, 1, &resToken);
      if (status == NO_ERROR)
      {
        a_out = i;
        return true;
      }
    }
    return false;
  }

  Client::Client()
    : m_pimpl(new PIMPL())
  {

  }

  Client::~Client()
  {
    Shutdown();
  }

  void Client::Shutdown()
  {
    m_pimpl->shouldClose = true;
    //SendDummyPayloads();

    //Wait to join threads

    //Delete sockets
    if (m_pimpl->portSend != 0)
    {
      unsigned long status = DeletePersistentTcpPortReservation((USHORT)m_pimpl->portSend, 1);
      if (status != NO_ERROR)
      {
        std::cout << "myportSend: DeletePersistentTcpPortReservation returned error: %ld\n" << status;
      }
      else
      {
        m_pimpl->portSend = 0;
      }
    }

    if (m_pimpl->portRecieve != 0)
    {
      unsigned long status = DeletePersistentTcpPortReservation((USHORT)m_pimpl->portRecieve, 1);
      if (status != NO_ERROR)
      {
        std::cout << "m_myportRecieve: DeletePersistentTcpPortReservation returned error: %ld\n" << status;
      }
      else
      {
        m_pimpl->portRecieve = 0;
      }
    }
  }

  bool Client::Startup(Mediator * a_pMediator)
  {
    if (!AssignPort(m_pimpl->portSend)) return false;
    if (!AssignPort(m_pimpl->portRecieve)) return false;

    m_pimpl->pMediator = a_pMediator;

    m_pimpl->threadBroadcastListener = std::thread(RunBroadcastListener, this);
    m_pimpl->threadBroadcastSpeaker = std::thread(RunBroadcastSpeaker, this);
    m_pimpl->threadRecieve = std::thread(RunRecieve, this);

    return true;
  }
}