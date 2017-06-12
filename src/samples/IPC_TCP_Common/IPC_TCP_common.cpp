#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <sstream>
#include <iostream>

#include "DgStringFunctions.h"
#include "DgIPC.h"
#include "IPC_TCP_common.h"

namespace IPC
{
  typedef uint16_t ipAddrStringSizeType;

  void LogDefault(std::string const & a_str, int a_logLevel)
  {
    std::cout << a_str << ": " << a_logLevel << "\n";
  }

  void(*g_Log)(std::string const &, int) = LogDefault;

  bool SetLoggingFunction(void(*a_Log)(std::string const &, int))
  {
    g_Log = a_Log;
    return true;
  }

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

  bool Port::Set(std::string const & a_str)
  {
    if (!Dg::StringToNumber(m_port, a_str, std::dec))
    {
      std::stringstream ss;
      ss << "Port::Set() -> could not read port number from string: " << a_str;
      g_Log(ss.str(), Dg::LL_Error);
      m_port = 0;
      return false;
    }
    return true;
  }

  char const * Port::Build(char const * a_pData)
  {
    m_port = *((PortType*)a_pData);
    a_pData += sizeof(PortType);
    return a_pData;
  }

  void Port::Serialize(std::vector<char> & a_out) const
  {
    for (size_t i = 0; i < sizeof(PortType); i++)
    {
      a_out.push_back((reinterpret_cast<char const *>(&m_port))[i]);
    }
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

  void SocketData::Serialize(std::vector<char>  & a_out) const
  {
    m_port.Serialize(a_out);
    ipAddrStringSizeType strLen = ipAddrStringSizeType(m_ipAddr.size());
    for (size_t i = 0; i < sizeof(ipAddrStringSizeType); i++)
    {
      a_out.push_back((reinterpret_cast<char*>(&strLen))[i]);
    }

    for (auto c : m_ipAddr)
    {
      a_out.push_back(c);
    }
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

  char const * MessageHeader::Build(char const * a_pData)
  {
    ID = *((type_ID*)a_pData);
    a_pData += sizeof(type_ID);
    payloadSize = *((type_payloadSize*)a_pData);
    a_pData += sizeof(type_payloadSize);
    return a_pData;
  }

  void MessageHeader::Serialize(std::vector<char> & a_out) const
  {
    for (size_t i = 0; i < sizeof(ID); i++)
    {
      a_out.push_back((reinterpret_cast<char const *>(&ID))[i]);
    }
    for (size_t i = 0; i < sizeof(payloadSize); i++)
    {
      a_out.push_back((reinterpret_cast<char const *>(&payloadSize))[i]);
    }
  }

  void MessageHeader::Clear()
  {
    ID = E_None;
    payloadSize = 0;
  }

  bool Message::Build(std::vector<char> const & a_message)
  {
    Clear();

    if (a_message.size() < MessageHeader::Size())
    {
      std::stringstream ss;
      ss << "Message::Build() -> message too small. Min size: "
        << MessageHeader::Size() << ", message size: " << a_message.size();
      g_Log(ss.str(), Dg::LL_Warning);
      return false;
    }

    char const * pData = header.Build(a_message.data());
    payload.assign(pData, &a_message.back());

    if (payload.size() < size_t(header.payloadSize))
    {
      std::stringstream ss;
      ss << "Message::Build() -> payload size mismatch. Payload size is "
        << payload.size() << " but flagged as " << header.payloadSize;
      g_Log(ss.str(), Dg::LL_Warning);
      return false;
    }
    return true;
  }

  std::vector<char> Message::Serialize() const
  {
    std::vector<char> headerData;
    header.Serialize(headerData);

    std::vector<char> messageData;
    messageData.reserve(headerData.size() + payload.size());
    messageData.insert(messageData.end(), headerData.begin(), headerData.end());
    messageData.insert(messageData.end(), payload.begin(), payload.end());
    return messageData;
  }

  void Message::Clear()
  {
    header.Clear();
    payload.clear();
  }

  void Message::Set(PacketID a_id, std::vector<char> const & a_payload)
  {
    header.ID = a_id;
    header.payloadSize = payload.size();
    payload = a_payload;
  }

  size_t MessageHeader::Size()
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
      g_Log(ss.str(), Dg::LL_Error);
      return false;
    }

    a_out.Set_Port(Port(ntohs(sin.sin_port)));

    int const bufLen = 64;
    char buf[bufLen] = {};
    InetNtop(AF_INET, &sin.sin_addr, buf, bufLen);
    a_out.Set_IP(std::string(buf));
    return true;
  }

  bool Recv(SOCKET a_socket, std::vector<char> & a_out)
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
        g_Log(ss.str(), Dg::LL_Error);
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

  bool Send(SocketData a_socketData, std::vector<char> const & a_payload)
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
      g_Log(ss.str(), Dg::LL_Error);
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
        g_Log(ss.str(), Dg::LL_Error);
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
      g_Log(ss.str(), Dg::LL_Error);
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
        g_Log(ss.str(), Dg::LL_Error);
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
      g_Log(ss.str(), Dg::LL_Error);
      return SOCKET_ERROR;
    }

    SOCKET skt = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (skt == INVALID_SOCKET)
    {
      std::stringstream ss;
      ss << "ServerAppBase() -> socket() failed with error: " << WSAGetLastError();
      g_Log(ss.str(), Dg::LL_Error);
      freeaddrinfo(result);
      return SOCKET_ERROR;
    }

    iResult = bind(skt, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
      std::stringstream ss;
      ss << "ServerAppBase() -> bind() failed with error: " << WSAGetLastError();
      g_Log(ss.str(), Dg::LL_Error);
      freeaddrinfo(result);
      closesocket(skt);
      return SOCKET_ERROR;
    }

    freeaddrinfo(result);

    if (listen(skt, SOMAXCONN) == SOCKET_ERROR)
    {
      std::stringstream ss;
      ss << "ServerAppBase() -> listen() failed with error: " << WSAGetLastError();
      g_Log(ss.str(), Dg::LL_Error);
      closesocket(skt);
      return SOCKET_ERROR;
    }
    return skt;
  }
}