#ifndef IPC_TCP_PLUGIN_ACCEPTOR_H
#define IPC_TCP_PLUGIN_ACCEPTOR_H

#include "IPC_TCP_AcceptorBase.h"
#include "IPC_TCP_MediatorBase.h"
#include "IPC_TCP_common.h"
#include "IPC_TCP_PluginInterface.h"

class ReceiverTCP;

class Acceptor : public IPC::TCP::AcceptorBase
{
public:

  Acceptor(ReceiverTCP *, ipcNewDataCallback);
  Acceptor(Acceptor const &);
  ~Acceptor() {}

  void Run(IPC::TCP::MediatorBase *);
  Acceptor * Clone() const;

private:

  void Handle_Dispatch();

private:

  ReceiverTCP *      m_pReceiver;
  IPC::TCP::Message  m_message;
  ipcNewDataCallback m_newDataCallback;
};

#endif