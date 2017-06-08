
#include "StopwatchApp.h"

//Comment out to get the console window (use for debugging)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(void)
{

  StopwatchApp *app(nullptr);
  try
  {
    app = new StopwatchApp();
  }
  catch (...)
  {
    return 1;
  }

  app->Run();
  delete app;
  return 0;
}