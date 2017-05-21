#include "NetworkClient.h"

int main()
{
  Dg::Client client;

  client.Startup();
  client.Shutdown();

  return 0;
}