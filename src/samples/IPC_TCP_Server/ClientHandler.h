#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <mutex>
#include <vector>

#include "IPC_TCP_common.h"
#include "DgDoublyLinkedList.h"
#include "DgTimer.h"

class ClientHandler
{
public:

  ClientHandler();

  void AddClient(IPC::SocketData a_sd);
  void RemoveClient(IPC::SocketData a_sd);
  std::vector<IPC::SocketData> GetCurrentClients();

private:

  void CleanClients();

private:

  static double const s_clientTimeout;

private:

  struct TimedClient
  {
    IPC::SocketData  socketData;
    double           lastPing;
  };

  Dg::Timer                           m_timer;
  std::mutex                          m_mutex;
  Dg::DoublyLinkedList<TimedClient>   m_currentClients;
};

#endif