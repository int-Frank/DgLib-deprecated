#ifndef SERVERSTATE_OFF_H
#define SERVERSTATE_OFF_H

#include <windows.h>
#include <thread>
#include <atomic>

#include "IPC_TCP_common.h"
#include "ClientHandler.h"
#include "ServerStateBase.h"
#include "DgTypes.h"

class ServerState_Off : public ServerStateBase
{
public:

  ServerState_Off(TCP_Server * a_pApp)
    : ServerStateBase(a_pApp)
  {
  }
};

#endif