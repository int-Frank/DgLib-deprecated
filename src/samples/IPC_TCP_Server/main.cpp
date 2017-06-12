
#include "IPC_TCP_Server.h"

//Comment out to get the console window (use for debugging)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(void)
{

  TCP_Server *app(nullptr);
  try
  {
    app = new TCP_Server();
  }
  catch (...)
  {
    return 1;
  }

  app->Run();
  delete app;
  return 0;
}