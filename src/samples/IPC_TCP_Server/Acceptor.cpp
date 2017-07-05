#include <sstream>
#include <thread>

#include "ServerState_On.h"
#include "DgTypes.h"
#include "Acceptor.h"
#include "Mediator.h"

Acceptor * Acceptor::Clone() const
{
  return new Acceptor(*this);
}

void Acceptor::Run(IPC::TCP::MediatorBase * a_pMediator)
{
  std::vector<char> messageData;
  do
  {
    if (IPC::TCP::Recv(m_socket, messageData) && !a_pMediator->ShouldStop())
    {
      if (messageData.size() < IPC::TCP::MessageHeader::Size())
      {
        std::stringstream ss;
        ss << "Run_AcceptClient(): Message too small: " << messageData.size() << " bytes. ";
        ss << "Require at least " << IPC::TCP::MessageHeader::Size() << " bytes for the header.";
        m_rApp.LogToOutputWindow(ss.str(), Dg::LL_Warning);
        break;
      }

      if (!m_message.Build(messageData))
      {
        break;
      }

      switch (m_message.header.ID)
      {
      case IPC::TCP::E_IPAddressRequest:
      {
        Handle_IPAddressRequest();
        break;
      }
      case IPC::TCP::E_RegisterClient:
      {
        Handle_RegisterClient();
        break;
      }
      case IPC::TCP::E_ClientExiting:
      {
        Handle_DeregisterClient();
        break;
      }
      case IPC::TCP::E_Dispatch:
      {
        Handle_Dispatch(a_pMediator);
        break;
      }
      default:
      {
        std::stringstream ss;
        ss << "Run_AcceptClient(): Unrecognised ID: " << m_message.header.ID;
        m_rApp.LogToOutputWindow(ss.str(), Dg::LL_Warning);
      }
      }
    }
  } while (false);

  closesocket(m_socket);
  m_socket = INVALID_SOCKET;
}

void Acceptor::Handle_RegisterClient()
{
  IPC::TCP::SocketData sd;
  if (sd.Build(m_message.payload.data()))
  {
    m_rApp.RegisterClient(sd);
  }
}

void Acceptor::Handle_DeregisterClient()
{
  IPC::TCP::SocketData sd;
  if (sd.Build(m_message.payload.data()))
  {
    m_rApp.DeregisterClient(sd);
  }
}

void Acceptor::Handle_IPAddressRequest()
{
  IPC::TCP::SocketData sd;
  if (IPC::TCP::GetPeerData(m_socket, sd))
  {
    std::vector<char> payload;
    sd.Serialize(payload);

    IPC::TCP::Message message;
    message.Set(IPC::TCP::E_IPAddressResponse, payload);

    IPC::TCP::Send(m_socket, message.Serialize());
  }
  else
  {
    m_rApp.LogToOutputWindow("Handle_IPAddressRequest() -> Failed to get peer data from socket during ", Dg::LL_Warning);
  }
}

void Acceptor::Handle_Dispatch(IPC::TCP::MediatorBase * a_pMediator)
{
  auto clientList = m_rApp.GetClientList();
  for (auto it = clientList.cbegin(); it != clientList.cend(); it++)
  {
    if (!IPC::TCP::Send(*it, m_message.payload))
    {
      std::stringstream ss;
      ss << "Handle_Dispatch() -> Failed to send to: " << it->Get_IP() << ":" << it->Get_Port().As_string();
      m_rApp.LogToOutputWindow(ss.str(), Dg::LL_Warning);
    }
    if (a_pMediator->ShouldStop())
    {
      break;
    }
  }
}