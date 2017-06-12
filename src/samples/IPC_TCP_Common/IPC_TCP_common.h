#ifndef IPC_TCP_COMMON_H
#define IPC_TCP_COMMON_H

#include <windows.h>

#include <vector>
#include <string>
#include <stdint.h>

namespace IPC
{
  enum PacketID
  {
    E_None = 0xA4C9,
    E_IPAddressRequest,
    E_IPAddressResponse,
    E_RegisterClient,
    E_ClientExiting,
    E_ServerStop,
    E_Dispatch
  };

  class MessageHeader
  {
  public:

    MessageHeader()
      : ID(E_None)
      , payloadSize(0)
    {}

    char const * Build(char const * a_buf);
    void Serialize(std::vector<char> &) const;
    static size_t Size();
    void Clear();

  public:

    typedef uint16_t type_ID;
    typedef uint32_t type_payloadSize;

    type_ID           ID;
    type_payloadSize  payloadSize;
  };

  class Message
  {
  public:

    bool Build(std::vector<char> const &);
    std::vector<char> Serialize() const;
    void Set(PacketID, std::vector<char> const & payload);
    void Clear();

  public:

    MessageHeader     header;
    std::vector<char> payload;
  };

  class Port
  {
  public:

    typedef uint16_t PortType;

  public:

    Port(): m_port(0) {}
    Port(PortType);

    char const * Build(char const * a_buf);
    void Serialize(std::vector<char> &) const;

    void Set(PortType);
    bool Set(std::string const &);

    PortType As_uint16() const;
    std::string As_string() const;

  private:

    PortType m_port;
  };

  class SocketData
  {
  public:

    char const * Build(char const * a_buf);
    void Serialize(std::vector<char> &) const;

    bool operator==(SocketData const & b) const;

    std::string Get_IP() const;
    Port Get_Port() const;

    void Set_IP(std::string const &);
    void Set_Port(Port);

  public:

    std::string m_ipAddr;
    Port        m_port;
  };

  //Log levels have the same values as the DgTypes.h -> LogLevel enum
  bool SetLoggingFunction(void(*a_Log)(std::string const &, int));

  bool InitWinsock();
  void ShutdownWinsock();

  SOCKET CreateListenSocket(char const * a_port, char const * a_ipAddr);

  bool Recv(SOCKET, std::vector<char> &);
  bool Send(SocketData, std::vector<char> const &);
  bool GetSocketData(SOCKET, SocketData &);
}


#endif