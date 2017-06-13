#include "ClientHandler.h"

double const ClientHandler::s_clientTimeout = 4.0;

ClientHandler::ClientHandler()
{
  m_timer.Start();
}

void ClientHandler::AddClient(IPC::TCP::SocketData a_sd)
{
  CleanClients();
  std::lock_guard<std::mutex> lock(m_mutex);

  for (auto it = m_currentClients.begin();
    it != m_currentClients.end();
    it++)
  {
    if (it->socketData == a_sd)
    {
      it->lastPing = 0;
      return;
    }
  }

  TimedClient tc;
  tc.socketData = a_sd;
  tc.lastPing = m_timer.GetTime();
  m_currentClients.push_back(tc);
}

void ClientHandler::RemoveClient(IPC::TCP::SocketData a_sd)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  for (auto it = m_currentClients.end();
    it != m_currentClients.begin();)
  {
    it--;
    if (it->socketData == a_sd)
    {
      it = m_currentClients.erase(it);
    }
  }
}

std::vector<IPC::TCP::SocketData> ClientHandler::GetCurrentClients()
{
  std::vector<IPC::TCP::SocketData> result;
  CleanClients();
  std::lock_guard<std::mutex> lock(m_mutex);
  for (auto it = m_currentClients.cbegin();
    it != m_currentClients.cend();
    it++)
  {
    result.push_back(it->socketData);
  }
  return result;
}

void ClientHandler::CleanClients()
{
  double curTime = m_timer.GetTime();
  std::lock_guard<std::mutex> lock(m_mutex);
  for (auto it = m_currentClients.end();
    it != m_currentClients.begin();)
  {
    it--;
    if (curTime - it->lastPing > s_clientTimeout)
    {
      it = m_currentClients.erase(it);
    }
  }
}