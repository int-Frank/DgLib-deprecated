#ifndef IPC_TCP_PLUGIN_RECEIVER_H
#define IPC_TCP_PLUGIN_RECEIVER_H

#include <thread>
#include <atomic>

#include "DgIPC.h"
#include "IPC_TCP_common.h"

class ReceiverTCP : public ipcReceiverBase
{
public:

  ReceiverTCP();
  ReceiverTCP(ReceiverTCP const &) = delete;
  ReceiverTCP & operator=(ReceiverTCP const &) = delete;
  ~ReceiverTCP();

  bool Init(ipcConfigMap const & a_config, ipcNewDataCallback a_clbk);
  bool ShouldStop();
  bool Shutdown();

private:

  std::atomic<bool>     m_shouldStop;
  std::thread *         m_pThreadListener;
  std::thread *         m_pThreadPing;
  IPC::TCP::SocketData  m_listenSocketData;
};

#endif