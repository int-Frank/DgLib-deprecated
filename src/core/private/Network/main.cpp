#include "NetworkClient.h"

int main()
{
  Dg::Client client;

  client.Start();
  client.Shutdown();

  return 0;
}