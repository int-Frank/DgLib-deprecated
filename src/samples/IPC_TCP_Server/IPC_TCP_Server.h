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
  class ServerStateBase;

  class TCP_Server : public DgApp
  {
  public:

    TCP_Server();
    ~TCP_Server();

    void BuildUI();
    void DoFrame(double);

  private:

    static int const s_textBufLen = 64;

  private:

    enum State
    {
      State_On,
      State_Off
    };

    ServerStateBase * m_pServerState;
    char              m_ipBuf[s_textBufLen];
    char              m_portBuf[s_textBufLen];
    int               m_currentState;
    int               m_desiredState;
  };
}

#endif