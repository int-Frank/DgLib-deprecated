#ifndef DGIPC_H
#define DGIPC_H

#ifdef BUILD_DLL
#define DLLAPI extern "C" __declspec(dllexport)
#else
#define DLLAPI
#endif // BUILD_DLL

typedef void(*ipcNewDataCallback)(char const *, int);
typedef void(*ipcLogCallback)(char const *, int);

typedef bool(*ProcipcInit)(ipcLogCallback);
typedef bool(*ProcipcShutdown)();
typedef char*(*ProcipcGetName)();
typedef char*(*ProcipcGetUsage)();

class IIPC_Dispatcher
{
public:
  IIPC_Dispatcher() {}
  virtual ~IIPC_Dispatcher() {}

  //ConfigMap should be std::map<std::string, std::string>
  virtual bool Init(void const * pConfigMap) = 0;
  virtual bool Shutdown() = 0;
  virtual void Dispatch(char const * data, int size) = 0;
};

class IIPC_Receiver
{
public:

  IIPC_Receiver() {}
  virtual ~IIPC_Receiver() {}

  //ConfigMap should be std::map<std::string, std::string>
  virtual bool Init(void const * pConfigMap, ipcNewDataCallback) = 0;
  virtual bool Shutdown() = 0;
};

typedef IIPC_Dispatcher*(*ProcipcCreateDispatcher)();
typedef void(*ProcipcReleaseDispatcher)(IIPC_Dispatcher *);
typedef IIPC_Receiver*(*ProcipcCreateReceiver)();
typedef void(*ProcipcReleaseReceiver)(IIPC_Receiver *);

DLLAPI bool ipcInit(ipcLogCallback);
DLLAPI bool ipcShutdown();

DLLAPI IIPC_Dispatcher * ipcCreateDispatcher();
DLLAPI void ipcReleaseDispatcher(IIPC_Dispatcher *);

DLLAPI IIPC_Receiver * ipcCreateReceiver();
DLLAPI void ipcReleaseReceiver(IIPC_Receiver *);

DLLAPI char * ipcGetName();
DLLAPI char * ipcGetUsage();

#endif