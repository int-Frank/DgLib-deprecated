
#include <thread>

#include "Listener.h"
#include "ServerState_On.h"

static void Run_Listen(ServerState_On * a_pApp, IPC::SocketData a_listenSocket)
{
  a_pApp->ListenerRunning(true);
  Listener listener(a_pApp);
  if (listener.Init(a_listenSocket))
  {
    listener.Run();
  }
  a_pApp->ListenerRunning(false);
}

ServerState_On::ServerState_On(TCP_Server * a_pApp, IPC::SocketData const & a_listenSocketData)
  : ServerStateBase(a_pApp)
  , m_shouldStop(false)
  , m_listenSocketData(a_listenSocketData)
{
  m_shouldStop = false;
  m_listenThread = std::thread(Run_Listen, this, m_listenSocketData);
}

void ServerState_On::RegisterClient(IPC::SocketData const & a_sd)
{
  m_clientHandler.AddClient(a_sd);
}

void ServerState_On::DeregisterClient(IPC::SocketData const & a_sd)
{
  m_clientHandler.RemoveClient(a_sd);
}

void ServerState_On::RegisterThread()
{
  m_activeThreads++;
}

void ServerState_On::DeregisterThread()
{
  m_activeThreads--;
}

std::vector<IPC::SocketData> ServerState_On::GetClientList()
{
  return m_clientHandler.GetCurrentClients();
}

bool ServerState_On::ShouldStop() const
{
  return m_shouldStop;
}

void ServerState_On::ListenerRunning(bool a_val)
{
  m_listenerRunning = a_val;
}

ServerState_On::~ServerState_On()
{
  m_rApp.LogToOutputWindow("Shutting down server...", Dg::LL_Info);

  m_shouldStop = true;

  //Send message to wake up listener thread
  IPC::Message message;
  message.header.ID = IPC::E_ServerStop;
  message.header.payloadSize = 0;

  std::vector<char> data = message.Serialize();
  if (m_listenerRunning)
  {
    if (Send(m_listenSocketData, data))
    {
      //join listener thread
      m_listenThread.join();
    }
    else
    {
      m_rApp.LogToOutputWindow("Unable to wake listener thread!", Dg::LL_Error);
    }
  }
  else
  {
    m_listenThread.join();
  }

  //Ensure all worker threads are done
  int elapsedTime = 0;
  while (m_activeThreads > 0)
  {
    int const sleepTime = 200;
    Sleep(sleepTime);
    elapsedTime += sleepTime;
  }

  m_rApp.LogToOutputWindow("Done!", Dg::LL_Info);
}