#ifndef SERVERSTATEBASE_H
#define SERVERSTATEBASE_H

#include <string>

#include "IPC_TCP_Server.h"
#include "IPC_TCP_common.h"
#include "DgTypes.h"

class ServerStateBase
{
public:

  ServerStateBase(TCP_Server * a_pApp)
    : m_rApp(*a_pApp)
  {}

  virtual ~ServerStateBase() {}
  void LogToOutputWindow(std::string const & a_message, Dg::LogLevel a_lvl)
  {
    m_rApp.LogToOutputWindow(a_message, a_lvl);
  }

protected:

  TCP_Server & m_rApp;
};

#endif