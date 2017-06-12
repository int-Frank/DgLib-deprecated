#ifndef DGIPC_H
#define DGIPC_H

#include <vector>
#include <map>
#include <string>
#include <atomic>

#include "DgTypes.h"

#ifdef BUILD_DLL
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI
#endif // BUILD_DLL

namespace Dg
{
  namespace IPC
  {
    class DispatcherBase
    {
    public:
      virtual ~DispatcherBase() = default;

      virtual bool Init(std::map<std::string, std::string> const & config) = 0;

      virtual void Shutdown() = 0;

      virtual void Dispatch(std::vector<char> const &, std::atomic<bool> const & shouldQuit) = 0;
    };

    //class ReceiverBase
    //{
    //public:
    //  virtual ~ReceiverBase() = default;
    //
    //  virtual bool Init(std::map<std::string, std::string> const & config,
    //                    void (*NewDataCallback)(std::vector<char> const &)) = 0;
    //
    //  virtual void Shutdown() = 0;
    //};

    DLLAPI bool Init(void(*a_Log)(std::string const &, LogLevel));

    DLLAPI DispatcherBase * GetDispatcher();
    //DLLAPI ReceiverBase * GetReceiver();

    DLLAPI std::string GetName();
    DLLAPI std::string GetUsage();
  }
}

#endif