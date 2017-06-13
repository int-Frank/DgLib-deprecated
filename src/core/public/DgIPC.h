#ifndef DGIPC_H
#define DGIPC_H

#include <vector>
#include <map>
#include <string>
#include <atomic>

#ifdef BUILD_DLL
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI
#endif // BUILD_DLL

class ipcDispatcherBase
{
public:
  virtual ~ipcDispatcherBase() = default;

  virtual bool Init(std::map<std::string, std::string> const & config) = 0;
  virtual void Shutdown() = 0;

  virtual void Dispatch(std::vector<char> const &) = 0;
};

class ipcReceiverBase
{
public:
  virtual ~ipcReceiverBase() = default;

  virtual bool Init(std::map<std::string, std::string> const & config,
                    void (*NewDataCallback)(std::vector<char> const &)) = 0;
  virtual void Shutdown() = 0;
};

DLLAPI bool ipcInit(void(*a_Log)(std::string const &, int));
DLLAPI bool ipcShutdown();

DLLAPI ipcDispatcherBase * ipcGetDispatcher();
DLLAPI ipcReceiverBase * GetReceiver();

DLLAPI std::string ipcGetName();
DLLAPI std::string ipcGetUsage();

#endif