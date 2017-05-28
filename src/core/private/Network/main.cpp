#include "NetworkCommon.h"
#include "NetworkClient.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
  Dg::InitWinsock();

  struct addrinfo *result = NULL,
    *ptr = NULL,
    hints;

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;

  int iResult = getaddrinfo(NULL, NULL, &hints, &result);
  if (iResult != 0) 
  {
    printf("getaddrinfo failed: %d\n", iResult);
    WSACleanup();
    return 1;
  }

  Dg::ShutdownWinsock();

  //Dg::InitWinsock();
  //
  //Dg::PeerNode peer;
  //
  //peer.Init();
  //peer.Shutdown();
  //
  //Dg::ShutdownWinsock();

  char g(0);
  std::cin >> g;

  return 0;
}