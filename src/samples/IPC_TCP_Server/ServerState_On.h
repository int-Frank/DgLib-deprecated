#ifndef SERVERSTATE_ON_H
#define SERVERSTATE_ON_H

#include <windows.h>
#include <thread>
#include <atomic>

#include "IPC_TCP_common.h"
#include "ClientHandler.h"
#include "ServerStateBase.h"
#include "DgTypes.h"

class ServerState_On : public ServerStateBase
{
public:

  ServerState_On(TCP_Server * a_pApp, IPC::TCP::SocketData const &);

  ~ServerState_On();

  void RegisterClient(IPC::TCP::SocketData const &);
  void DeregisterClient(IPC::TCP::SocketData const &);
  std::vector<IPC::TCP::SocketData> GetClientList();
  void ListenerRunning(bool);
  bool ShouldStop() const;

private:

  static int const s_textBufLen = 64;

private:

  std::atomic<bool>     m_listenerRunning;
  std::atomic<bool>     m_shouldStop;
  ClientHandler         m_clientHandler;
  std::thread           m_listenThread;
  IPC::TCP::SocketData  m_listenSocketData;
};

#endif