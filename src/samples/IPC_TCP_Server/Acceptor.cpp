#include <sstream>
#include <thread>

#include "IPC_TCP_Server.h"
#include "DgTypes.h"
#include "Acceptor.h"

namespace IPC
{
  bool Acceptor::Init(SOCKET a_socket)
  {
    m_clientSocket = a_socket;
    return true;
  }

  void Acceptor::Run()
  {
    m_rApp.RegisterThread();

    std::vector<char> messageData;
    do
    {
      if (Recv(m_clientSocket, messageData) && !m_rApp.ShouldStop())
      {
        if (messageData.size() < MessageHeader::Size())
        {
          std::stringstream ss;
          ss << "Run_AcceptClient(): Message too small: " << messageData.size() << " bytes. ";
          ss << "Require at least " << MessageHeader::Size() << " bytes for the header.";
          m_rApp.LogToOutputWindow(ss.str(), Dg::LL_Error);
          break;
        }

        if (!m_message.Build(messageData))
        {
          break;
        }

        switch (m_message.header.ID)
        {
        case E_IPAddressRequest:
        {
          Handle_IPAddressRequest();
          break;
        }
        case E_RegisterClient:
        {
          Handle_RegisterClient();
          break;
        }
        case E_ClientExiting:
        {
          Handle_DeregisterClient();
          break;
        }
        case E_Dispatch:
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

    closesocket(m_clientSocket);
    m_clientSocket = INVALID_SOCKET;
    m_rApp.DeregisterThread();
  }

  void Acceptor::Handle_RegisterClient()
  {
    SocketData sd;
    if (sd.Build(m_message.payload.data()))
    {
      m_rApp.RegisterClient(sd);
    }
  }

  void Acceptor::Handle_DeregisterClient()
  {
    SocketData sd;
    if (sd.Build(m_message.payload.data()))
    {
      m_rApp.DeregisterClient(sd);
    }
  }

  void Acceptor::Handle_IPAddressRequest()
  {
    SocketData sd;
    if (GetSocketData(m_clientSocket, sd))
    {
      std::vector<char> payload;
      sd.Serialize(payload);

      Message message;
      message.Set(E_IPAddressResponse, payload);

      Send(sd, message.Serialize());
    }
  }

  void Acceptor::Handle_Dispatch()
  {
    auto clientList = m_rApp.GetClientList();
    for (auto it = clientList.cbegin(); it != clientList.cend(); it++)
    {
      if (!Send(*it, m_message.payload))
      {
        std::stringstream ss;
        ss << "Failed to send to: " << it->Get_IP() << ":" << it->Get_Port().As_string();
        m_rApp.LogToOutputWindow(ss.str(), Dg::LL_Warning);
      }
      if (m_rApp.ShouldStop())
      {
        break;
      }
    }
  }
}