#include <sstream>
#include <thread>

#include "ServerState_On.h"
#include "DgTypes.h"
#include "Acceptor.h"
#include "IPC_TCP_Mediator.h"

bool Acceptor::Init(SOCKET a_socket)
{
  m_clientSocket = a_socket;
  return true;
}

Acceptor * Acceptor::Clone() const
{
  return new Acceptor(&m_rApp);
}

void Acceptor::Run()
{
  m_rApp.LogToOutputWindow("New client connection...", Dg::LL_Debug);
  m_rApp.RegisterThread();

  std::vector<char> messageData;
  do
  {
    if (IPC::TCP::Recv(m_clientSocket, messageData) && !IPC::TCP::Mediator::ShouldStop())
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
        Handle_Dispatch();
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

  m_rApp.LogToOutputWindow("Closing client connection.", Dg::LL_Debug);
  closesocket(m_clientSocket);
  m_clientSocket = INVALID_SOCKET;
  m_rApp.DeregisterThread();
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
  if (IPC::TCP::GetSocketData(m_clientSocket, sd))
  {
    std::vector<char> payload;
    sd.Serialize(payload);

    IPC::TCP::Message message;
    message.Set(IPC::TCP::E_IPAddressResponse, payload);

    IPC::TCP::Send(sd, message.Serialize());
  }
}

void Acceptor::Handle_Dispatch()
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
    if (IPC::TCP::Mediator::ShouldStop())
    {
      break;
    }
  }
}