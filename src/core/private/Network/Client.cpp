#include <iostream>

#include "NetworkClient.h"
#include "NetworkCommon.h"

namespace Dg
{
  Client::Client()
    : m_shouldClose(false)
    , m_pMediator(nullptr)
    , m_mySendPort(0)
    , m_myRecievePort(0)
  {

  }

  Client::~Client()
  {
    Shutdown();
  }

  void Client::Shutdown()
  {
    m_shouldClose = true;
    SendDummyPayloads();

    //Wait to join threads

    //Delete sockets
    if (m_mySendPort != 0)
    {
      unsigned long status = DeletePersistentTcpPortReservation((USHORT)m_mySendPort, 1);
      if (status != NO_ERROR)
      {
        std::cout << "mySendPort: DeletePersistentTcpPortReservation returned error: %ld\n" << status;
      }
      else
      {
        m_mySendPort = 0;
      }
    }

    if (m_mySendPort != 0)
    {
      unsigned long status = DeletePersistentTcpPortReservation((USHORT)m_myRecievePort, 1);
      if (status != NO_ERROR)
      {
        std::cout << "m_myRecievePort: DeletePersistentTcpPortReservation returned error: %ld\n" << status;
      }
      else
      {
        m_myRecievePort = 0;
      }
    }
  }

  bool Client::Start(Mediator * a_pMediator)
  {
    if (!ReservePorts())
    {
      return false;
    }

    m_pMediator = a_pMediator;

    Start_Broadcast_Listener();
    Start_Broadcast_Speaker();
    Start_Client_Listener();
  }

  bool Client::ReservePorts()
  {
    if (!AssignPort(m_mySendPort)) return false;
    if (!AssignPort(m_myRecievePort)) return false;
  }

  bool Client::AssignPort(uint16_t & a_out)
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
}