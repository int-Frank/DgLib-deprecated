
#include <thread>

#include "IPC_TCP_Listener.h"
#include "ServerState_On.h"
#include "Acceptor.h"
#include "IPC_TCP_Listener.h"


static std::atomic<bool> g_shouldStop = false;
static bool ShouldStop()
{
  return g_shouldStop;
}

static void Run_Listen(ServerState_On * a_pApp, IPC::TCP::SocketData a_listenSocket)
{
  a_pApp->ListenerRunning(true);
  IPC::TCP::Listener listener(new Acceptor(a_pApp));
  if (listener.InitStrict(a_listenSocket))
  {
    listener.Run();
  }
  listener.Shutdown();
  a_pApp->ListenerRunning(false);
}

ServerState_On::ServerState_On(TCP_Server * a_pApp, IPC::TCP::SocketData const & a_listenSocketData)
  : ServerStateBase(a_pApp)
  , m_listenSocketData(a_listenSocketData)
{
  g_shouldStop = false;
  IPC::TCP::SetStopFlagCallback(ShouldStop);
  m_listenThread = std::thread(Run_Listen, this, m_listenSocketData);
}

void ServerState_On::RegisterClient(IPC::TCP::SocketData const & a_sd)
{
  m_clientHandler.AddClient(a_sd);
}

void ServerState_On::DeregisterClient(IPC::TCP::SocketData const & a_sd)
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

std::vector<IPC::TCP::SocketData> ServerState_On::GetClientList()
{
  return m_clientHandler.GetCurrentClients();
}

void ServerState_On::ListenerRunning(bool a_val)
{
  m_listenerRunning = a_val;
}

ServerState_On::~ServerState_On()
{
  m_rApp.LogToOutputWindow("Shutting down server...", Dg::LL_Info);

  g_shouldStop = true;

  //Send message to wake up listener thread
  IPC::TCP::Message message;
  message.header.ID = IPC::TCP::E_ServerStop;
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