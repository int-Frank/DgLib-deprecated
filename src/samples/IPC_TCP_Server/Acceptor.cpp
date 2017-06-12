#include <sstream>
#include <thread>

#include "ServerState_On.h"
#include "DgTypes.h"
#include "Acceptor.h"

bool Acceptor::Init(SOCKET a_socket)
{
  m_clientSocket = a_socket;
  return true;
}

void Acceptor::Run()
{
  m_rApp.LogToOutputWindow("New client connection...", Dg::LL_Debug);
  m_rApp.RegisterThread();

  std::vector<char> messageData;
  do
  {
    if (IPC::Recv(m_clientSocket, messageData) && !m_rApp.ShouldStop())
    {
      if (messageData.size() < IPC::MessageHeader::Size())
      {
        std::stringstream ss;
        ss << "Run_AcceptClient(): Message too small: " << messageData.size() << " bytes. ";
        ss << "Require at least " << IPC::MessageHeader::Size() << " bytes for the header.";
        m_rApp.LogToOutputWindow(ss.str(), Dg::LL_Warning);
        break;
      }

      if (!m_message.Build(messageData))
      {
        break;
      }

      switch (m_message.header.ID)
      {
      case IPC::E_IPAddressRequest:
      {
        Handle_IPAddressRequest();
        break;
      }
      case IPC::E_RegisterClient:
      {
        Handle_RegisterClient();
        break;
      }
      case IPC::E_ClientExiting:
      {
        Handle_DeregisterClient();
        break;
      }
      case IPC::E_Dispatch:
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
  IPC::SocketData sd;
  if (sd.Build(m_message.payload.data()))
  {
    m_rApp.RegisterClient(sd);
  }
}

void Acceptor::Handle_DeregisterClient()
{
  IPC::SocketData sd;
  if (sd.Build(m_message.payload.data()))
  {
    m_rApp.DeregisterClient(sd);
  }
}

void Acceptor::Handle_IPAddressRequest()
{
  IPC::SocketData sd;
  if (GetSocketData(m_clientSocket, sd))
  {
    std::vector<char> payload;
    sd.Serialize(payload);

    IPC::Message message;
    message.Set(IPC::E_IPAddressResponse, payload);

    IPC::Send(sd, message.Serialize());
  }
}

void Acceptor::Handle_Dispatch()
{
  auto clientList = m_rApp.GetClientList();
  for (auto it = clientList.cbegin(); it != clientList.cend(); it++)
  {
    if (!IPC::Send(*it, m_message.payload))
    {
      std::stringstream ss;
      ss << "Handle_Dispatch() -> Failed to send to: " << it->Get_IP() << ":" << it->Get_Port().As_string();
      m_rApp.LogToOutputWindow(ss.str(), Dg::LL_Warning);
    }
    if (m_rApp.ShouldStop())
    {
      break;
    }
  }
}