
#include <sstream>

#include "IPC_TCP_Plugin_Acceptor.h"
#include "IPC_TCP_Plugin_ReceiverTCP.h"
#include "IPC_TCP_Logger.h"
#include "DgTypes.h"

using namespace IPC::TCP;
using namespace IPC::TCP::Logger;

Acceptor::Acceptor(ReceiverTCP * a_receiver, ipcNewDataCallback a_clbk)
  : m_pReceiver(a_receiver)
  , m_newDataCallback(a_clbk)
{

}

Acceptor::Acceptor(Acceptor const & a_other)
  : AcceptorBase(a_other)
  , m_pReceiver(a_other.m_pReceiver)
  , m_message(a_other.m_message)
  , m_newDataCallback(a_other.m_newDataCallback)
{

}

Acceptor * Acceptor::Clone() const
{
  return new Acceptor(*this);
}

void Acceptor::Run(IPC::TCP::MediatorBase * a_pMediator)
{
  Log("New client connection...", Dg::LL_Debug);

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
        Log(ss.str(), Dg::LL_Warning);
        break;
      }

      if (!m_message.Build(messageData))
      {
        break;
      }

      switch (m_message.header.ID)
      {
      case IPC::TCP::E_Dispatch:
      {
        Handle_Dispatch();
        break;
      }
      default:
      {
        std::stringstream ss;
        ss << "Run_AcceptClient(): Unrecognised ID: " << m_message.header.ID;
        Log(ss.str(), Dg::LL_Warning);
      }
      }
    }
  } while (false);

  Log("Closing client connection.", Dg::LL_Debug);
  closesocket(m_socket);
  m_socket = INVALID_SOCKET;
}

void Acceptor::Handle_Dispatch()
{

}