#include "NetworkCommon.h"
#include "NetworkClient.h"

int main()
{
  Dg::InitWinsock();

  Dg::PeerNode peer;

  peer.Init();
  peer.Shutdown();

  Dg::ShutdownWinsock();

  return 0;
}