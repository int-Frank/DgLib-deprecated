#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <sstream>

#define BUILD_DLL

#include "DgIPC.h"
#include "IPC_TPC_common.h"

namespace Dg
{
  namespace IPC
  {
    void(*g_Log)(std::string const &, LogLevel) = nullptr;

    std::string GetUsage()
    {
      std::stringstream ss;
      ss << "Uses TCP protocol to dispatch data a server, and then on to registered clients."
        << " Payloads will always contain a header. The header will have 2 fields:\n"
        << "  ID   : int, 2\n"
        << "  Size : int, 4\n"
        << "Possible values for ID are:\n"
        << "  " << E_IPAddressRequest << " : IP address query\n"
        << "  " << E_RegisterClient << " : Register a client\n"
        << "  " << E_ClientExiting << " : Notification that a client is closing\n"
        << "  " << E_Dispatch << " : Dispatch payload to all registered clients\n"
        << "Payload formats:\n"
        << "  ID : " << E_IPAddressRequest << "\n"
        << "    int, 2 - port to send response to.\n"
        << "And so on..."
        ;
      return ss.str();
    }

    std::string GetName()
    {
      return std::string("ICP_TCP");
    }

    bool Init(void(*a_Log)(std::string const &, LogLevel))
    {
      g_Log = a_Log;
      return true;
    }

    typedef uint16_t ipAddrStringSizeType;

    bool InitWinsock()
    {
      WSADATA wsaData;
      return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
    }

    void ShutdownWinsock()
    {
      WSACleanup();
    }

    Port::Port(PortType a_val)
      : m_port(a_val)
    {

    }

    void Port::Set(PortType a_val)
    {
      m_port = a_val;
    }

    char const * Port::Build(char const * a_buf)
    {
      m_port = *((PortType*)a_buf);
      a_buf += sizeof(PortType);
      return a_buf;
    }

    std::vector<char> Port::Serialize() const
    {
      std::vector<char> result;
      for (size_t i = 0; i < sizeof(PortType); i++)
      {
        result.push_back((reinterpret_cast<char const *>(&m_port))[i]);
      }
      return result;
    }

    std::string Port::As_string() const
    {
      std::stringstream ss;
      ss << m_port;
      return ss.str();
    }

    Port::PortType Port::As_uint16() const
    {
      return m_port;
    }

    char const * SocketData::Build(char const * a_buf)
    {
      a_buf = m_port.Build(a_buf);
      ipAddrStringSizeType len = *((ipAddrStringSizeType*)a_buf);
      a_buf += sizeof(ipAddrStringSizeType);
      m_ipAddr = std::string(a_buf, size_t(len));
      a_buf += sizeof(len);
      return a_buf;
    }

    std::vector<char> SocketData::Serialize() const
    {
      std::vector<char> dataPort = m_port.Serialize();

      std::vector<char> dataIPAddr;
      ipAddrStringSizeType strLen = ipAddrStringSizeType(m_ipAddr.size());
      for (size_t i = 0; i < sizeof(ipAddrStringSizeType); i++)
      {
        dataIPAddr.push_back((reinterpret_cast<char*>(&strLen))[i]);
      }

      for (auto c : m_ipAddr)
      {
        dataIPAddr.push_back(c);
      }

      std::vector<char> dataAll;
      dataAll.reserve(dataPort.size() + dataIPAddr.size());
      dataAll.insert(dataAll.end(), dataPort.begin(), dataPort.end());
      dataAll.insert(dataAll.end(), dataIPAddr.begin(), dataIPAddr.end());
      return dataAll;
    }

    std::string SocketData::Get_IP() const
    {
      return m_ipAddr;
    }

    void SocketData::Set_IP(std::string const & a_ipAddr)
    {
      m_ipAddr = a_ipAddr;
    }

    Port SocketData::Get_Port() const
    {
      return m_port;
    }

    void SocketData::Set_Port(Port a_port)
    {
      m_port = a_port;
    }

    char const * PayloadHeader::Build(char const * a_buf)
    {
      ID = *((uint16_t*)a_buf);
      a_buf += sizeof(uint16_t);
      payloadSize = *((uint32_t*)a_buf);
      a_buf += sizeof(uint32_t);
      return a_buf;
    }

    std::vector<char> PayloadHeader::Serialize() const
    {
      std::vector<char> result;
      for (size_t i = 0; i < sizeof(ID); i++)
      {
        result.push_back((reinterpret_cast<char const *>(&ID))[i]);
      }
      for (size_t i = 0; i < sizeof(payloadSize); i++)
      {
        result.push_back((reinterpret_cast<char const *>(&payloadSize))[i]);
      }
      return result;
    }

    size_t PayloadHeader::Size()
    {
      return sizeof(ID) + sizeof(payloadSize);
    }

    bool SocketData::operator==(SocketData const & a_other) const
    {
      return (m_port.As_uint16() == a_other.m_port.As_uint16())
        && (m_ipAddr == a_other.m_ipAddr);
    }

    bool GetSocketData(SOCKET a_socket, SocketData & a_out)
    {
      struct sockaddr_in sin;
      socklen_t len = sizeof(sin);
      if (getsockname(a_socket, (struct sockaddr *)&sin, &len) == SOCKET_ERROR)
      {
        std::stringstream ss;
        ss << "GetSocketData() -> getsockname() failed with error: " << WSAGetLastError();
        g_Log(ss.str(), LL_Error);
        return false;
      }

      a_out.Set_Port(Port(ntohs(sin.sin_port)));

      int const bufLen = 64;
      char buf[bufLen] = {};
      InetNtop(AF_INET, &sin.sin_addr, buf, bufLen);
      a_out.Set_IP(std::string(buf));
      return true;
    }

    bool Recv(SOCKET a_socket, std::vector<char> & a_out, std::atomic<bool> const & a_shouldQuit)
    {
      bool result = false;
      while (true)
      {
        int const bufLen = 1024;
        char buf[bufLen] = {};
        int returnValue = recv(a_socket, buf, bufLen, 0);
        if (returnValue == SOCKET_ERROR)
        {
          std::stringstream ss;
          ss << "Recv() -> recv() failed with error: " << WSAGetLastError();
          g_Log(ss.str(), LL_Error);
          break;
        }
        if (a_shouldQuit)
        {
          break;
        }
        if (returnValue == 0)
        {
          result = true;
          break;
        }

        for (int i = 0; i < returnValue; i++)
        {
          a_out.push_back(buf[i]);
        }
      }
      return true;
    }

    bool Send(SocketData a_socketData, std::vector<char> const & a_payload, std::atomic<bool> const & a_shouldQuit)
    {
      struct addrinfo *result = NULL,
        *ptr = NULL,
        hints;

      ZeroMemory(&hints, sizeof(hints));
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;

      int iResult = getaddrinfo(a_socketData.Get_IP().c_str(),
        a_socketData.Get_Port().As_string().c_str(),
        &hints, &result);
      if (iResult != 0)
      {
        std::stringstream ss;
        ss << "Send() -> getaddrinfo() failed with error: " << WSAGetLastError();
        ss << ". IP Address of client: " << a_socketData.Get_IP() << ":" << a_socketData.Get_Port().As_string();
        g_Log(ss.str(), LL_Error);
        return false;
      }

      // Attempt to connect to an address until one succeeds
      SOCKET connectSocket = INVALID_SOCKET;
      for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
      {
        // Create a SOCKET for connecting to server
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (connectSocket == INVALID_SOCKET)
        {
          std::stringstream ss;
          ss << "Send() -> socket() failed with error: " << WSAGetLastError();
          ss << ". IP Address of client: " << a_socketData.Get_IP() << ":" << a_socketData.Get_Port().As_string();
          g_Log(ss.str(), LL_Error);
          return false;
        }

        // Connect to server.
        iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
          closesocket(connectSocket);
          connectSocket = INVALID_SOCKET;
          continue;
        }
        break;
      }

      freeaddrinfo(result);

      if (connectSocket == INVALID_SOCKET)
      {
        std::stringstream ss;
        ss << "Send() -> Unable to connect!" << WSAGetLastError();
        ss << ". IP Address of client: " << a_socketData.Get_IP() << ":" << a_socketData.Get_Port().As_string();
        g_Log(ss.str(), LL_Error);
        return false;
      }

      int targetBytes = int(a_payload.size());
      int bytesSent = 0;
      bool returnResult = true;
      while (bytesSent < targetBytes)
      {
        bytesSent = send(connectSocket, &a_payload[bytesSent], int(a_payload.size()) - bytesSent, 0);

        if (bytesSent == SOCKET_ERROR)
        {
          std::stringstream ss;
          ss << "Send() -> send() failed with error: " << WSAGetLastError();
          ss << ". IP Address of client: " << a_socketData.Get_IP() << ":" << a_socketData.Get_Port().As_string();
          std::string str = ss.str();
          g_Log(ss.str(), LL_Error);
          returnResult = false;
          break;
        }

        if (a_shouldQuit)
        {
          returnResult = false;
          break;
        }
      }

      return returnResult;
    }

    SOCKET CreateListenSocket(char const * a_port, char const * a_ipAddr)
    {
      struct addrinfo *result = NULL, *ptr = NULL, hints;

      ZeroMemory(&hints, sizeof(hints));
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;
      hints.ai_flags = AI_PASSIVE;

      // Resolve the local address and port to be used by the server
      int iResult = getaddrinfo(a_ipAddr, a_port, &hints, &result);
      if (iResult != 0)
      {
        std::stringstream ss;
        ss << "ServerAppBase() -> getaddrinfo() failed with error: " << iResult;
        g_Log(ss.str(), LL_Error);
        return SOCKET_ERROR;
      }

      SOCKET skt = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

      if (skt == INVALID_SOCKET)
      {
        std::stringstream ss;
        ss << "ServerAppBase() -> socket() failed with error: " << WSAGetLastError();
        g_Log(ss.str(), LL_Error);
        freeaddrinfo(result);
        return SOCKET_ERROR;
      }

      iResult = bind(skt, result->ai_addr, (int)result->ai_addrlen);
      if (iResult == SOCKET_ERROR)
      {
        std::stringstream ss;
        ss << "ServerAppBase() -> bind() failed with error: " << WSAGetLastError();
        g_Log(ss.str(), LL_Error);
        freeaddrinfo(result);
        closesocket(skt);
        return SOCKET_ERROR;
      }

      freeaddrinfo(result);

      if (listen(skt, SOMAXCONN) == SOCKET_ERROR)
      {
        std::stringstream ss;
        ss << "ServerAppBase() -> listen() failed with error: " << WSAGetLastError();
        g_Log(ss.str(), LL_Error);
        closesocket(skt);
        return SOCKET_ERROR;
      }
      return skt;
    }
  }
}