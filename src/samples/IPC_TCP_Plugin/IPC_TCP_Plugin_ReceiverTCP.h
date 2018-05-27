#ifndef IPC_TCP_PLUGIN_RECEIVER_H
#define IPC_TCP_PLUGIN_RECEIVER_H

#include <thread>
#include <atomic>

#include "IPC_TCP_PluginInterface.h"
#include "IPC_TCP_common.h"

class ReceiverTCP : public IIPC_Receiver
{
public:

  ReceiverTCP();
  ReceiverTCP(ReceiverTCP const &) = delete;
  ReceiverTCP & operator=(ReceiverTCP const &) = delete;
  ~ReceiverTCP();

  bool Init(void const *, ipcNewDataCallback);
  bool ShouldStop();
  bool Shutdown();

private:

  std::atomic<bool>     m_shouldStop;
  std::thread *         m_pThreadListener;
  std::thread *         m_pThreadPing;
  IPC::TCP::SocketData  m_listenSocketData;
};

#endif