
#include <thread>

#include "IPC_TCP_Listener.h"
#include "ServerState_On.h"
#include "Acceptor.h"
#include "IPC_TCP_Listener.h"
#include "Mediator.h"

static void Run_Listen(ServerState_On * a_pApp, 
                       IPC::TCP::SocketData a_listenSocket,
                       IPC::TCP::MediatorBase * a_pMediator)
{
  a_pApp->ListenerRunning(true);
  IPC::TCP::Listener listener(new Acceptor(a_pApp));
  if (listener.InitStrict(a_listenSocket))
  {
    listener.Run(a_pMediator);
  }
  a_pApp->ListenerRunning(false);
  listener.Shutdown();
  delete a_pMediator;
}

ServerState_On::ServerState_On(TCP_Server * a_pApp, IPC::TCP::SocketData const & a_listenSocketData)
  : ServerStateBase(a_pApp)
  , m_listenSocketData(a_listenSocketData)
  , m_shouldStop(false)
{
  m_listenThread = std::thread(Run_Listen, this, m_listenSocketData, new Mediator(this));
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

bool ServerState_On::ShouldStop() const
{
  return m_shouldStop;
}

ServerState_On::~ServerState_On()
{
  m_rApp.LogToOutputWindow("Shutting down server...", Dg::LL_Info);

  m_shouldStop = true;

  //Send message to wake up listener thread
  IPC::TCP::Message message;
  message.header.ID = IPC::TCP::E_ServerStop;
  message.header.payloadSize = 0;

  std::vector<char> data = message.Serialize();
  if (m_listenerRunning)
  {
    if (!Send(m_listenSocketData, data))
    {
      m_rApp.LogToOutputWindow("Unable to wake listener thread!", Dg::LL_Warning);
    }
  }
  
  m_listenThread.join();
  
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