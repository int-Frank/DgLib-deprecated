#ifndef IPC_TCP_SERVER_H
#define IPC_TCP_SERVER_H

#include <windows.h>
#include <thread>
#include <atomic>

#include "DgApp.h"
#include "IPC_TCP_common.h"
#include "ClientHandler.h"

namespace IPC
{
  class TCP_Server : public DgApp
  {
  public:

    TCP_Server();
    ~TCP_Server();

    void BuildUI();
    void DoFrame(double);

  public:

    //These are called from other threads...
    void RegisterThread();
    void DeregisterThread();
    void RegisterClient(SocketData const &);
    void DeregisterClient(SocketData const &);
    std::vector<SocketData> GetClientList();
    bool ShouldStop() const;

  private:

    void Start();
    void Stop();

  private:

    static int const s_textBufLen = 64;

  private:

    enum State
    {
      State_On,
      State_Off
    };

    std::atomic<int>  m_activeThreads;
    std::atomic<bool> m_shouldStop;
    ClientHandler     m_clientHandler;
    std::thread       m_listenThread;
    SocketData        m_listenSocketData;
    char              m_ipBuf[s_textBufLen];
    char              m_portBuf[s_textBufLen];
    int               m_currentState;
    int               m_desiredState;
  };
}

#endif