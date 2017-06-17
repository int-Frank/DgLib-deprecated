#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <thread>
#include <atomic>
#include <sstream>

#include "IPC_TCP_Plugin_ReceiverTCP.h"
#include "IPC_TCP_Plugin_Mediator.h"
#include "DgTypes.h"
#include "IPC_TCP_common.h"
#include "IPC_TCP_AcceptorBase.h"
#include "IPC_TCP_MediatorBase.h"
#include "IPC_TCP_Plugin_Acceptor.h"
#include "IPC_TCP_Logger.h"
#include "IPC_TCP_Listener.h"
#include "IPC_TCP_Plugin_common.h"
#include "DgIPC.h"

using IPC::TCP::Logger::Log;
using namespace IPC::TCP;

static void Run_Ping(SocketData a_serverData,
                     SocketData a_listenData,
                     MediatorBase * a_pMediator)
{
  Message message;
  std::vector<char> payload;
  a_listenData.Serialize(payload);
  message.Set(E_RegisterClient, payload);
  payload = message.Serialize();

  DWORD const pingInterval = 2000;
  DWORD const sleepInterval = 100;
  DWORD totalTime = 0;
  while (!a_pMediator->ShouldStop())
  {
    if (totalTime % pingInterval == 0)
    {
      if (!Send(a_serverData, payload))
      {
        Log("Failed to ping server!", Dg::LL_Warning);
      }
    }
    Sleep(100);
    totalTime += sleepInterval;
  }
}

static void Run_Listen(Listener * a_pListener,
                       Mediator * a_pMediator,
                       Acceptor * a_pAcceptor)
{
  a_pListener->Run(a_pAcceptor, a_pMediator);

  delete a_pListener;
  delete a_pMediator;
  delete a_pAcceptor;
}

ReceiverTCP::ReceiverTCP()
  : m_pThreadListener(nullptr)
  , m_pThreadPing(nullptr)
  , m_shouldStop(false)
{}

ReceiverTCP::~ReceiverTCP()
{
  Shutdown();
}

bool ReceiverTCP::Init(ipcConfigMap const & a_config, ipcNewDataCallback a_clbk)
{
  if (m_pThreadListener || m_pThreadPing)
  {
    Log("Attempt to initialise ReceiverTCP. It is aleady initialised!", Dg::LL_Error);
    return false;
  }

  auto it_port = a_config.find("server_port");
  if (it_port == a_config.end())
  {
    Log("Couldn't initialise ReceiverTCP! Listen port not set in config file. Set using server_port=XXXXX", Dg::LL_Error);
    return false;
  }

  auto it_ipAddr = a_config.find("server_ip");
  if (it_ipAddr == a_config.end())
  {
    Log("Couldn't initialise ReceiverTCP! IP address not set in config file. Set using server_ip=XXX.XXX.XXX.XXX", Dg::LL_Error);
    return false;
  }

  Port port;
  if (!port.Set(it_port->second))
  {
    std::stringstream ss;
    ss << "Couldn't initialise ReceiverTCP! Invalid port number: " << it_port->second;
    Log(ss.str(), Dg::LL_Error);
    return false;
  }

  SocketData serverSocketData;
  serverSocketData.Set_IP(it_ipAddr->second);
  serverSocketData.Set_Port(port);

  Listener * plistener(new Listener());
  if (!plistener->InitByRequest(serverSocketData, m_listenSocketData))
  {
    return false;
  }

  std::stringstream ss;
  ss << "ReceiverTCP initialised at address " << m_listenSocketData.Get_IP()
    << ":" << m_listenSocketData.Get_Port().As_string();
  Log(ss.str(), Dg::LL_Info);

  m_pThreadListener = new std::thread(Run_Listen, plistener, 
                                      new Mediator(this),
                                      new Acceptor(this, a_clbk));
  m_pThreadPing = new std::thread(Run_Ping, serverSocketData, m_listenSocketData, new Mediator(this));

  return true;
}

bool ReceiverTCP::ShouldStop()
{
  return m_shouldStop;
}

bool ReceiverTCP::Shutdown()
{
  m_shouldStop = true;
  bool success = true;

  if (m_pThreadListener)
  {
    //Send message to wake up listener thread
    IPC::TCP::Message message;
    message.header.ID = IPC::TCP::E_ServerStop;
    message.header.payloadSize = 0;

    std::vector<char> data = message.Serialize();

    if (!Send(m_listenSocketData, data))
    {
      Log("Failed to properly shutdown ReceiverTCP. Unable to wake listener thread!", Dg::LL_Warning);
      success = false;
    }
    else
    {
      m_pThreadListener->join();
      delete m_pThreadListener;
      m_pThreadListener = nullptr;
    }
  }

  if (m_pThreadPing)
  {
    m_pThreadPing->join();
    delete m_pThreadPing;
    m_pThreadPing = nullptr;
  }

  return success;
}

ipcReceiverBase * ipcGetReceiver()
{
  return new ReceiverTCP();
}