
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#include "NetworkCommon.h"

namespace Dg
{
  bool InitWinsock()
  {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
  }

  void ShutdownWinsock()
  {
    WSACleanup();
  }
}