#ifndef IPC_TCP_COMMON_H
#define IPC_TCP_COMMON_H

#include <windows.h>

#include <vector>
#include <string>
#include <stdint.h>

namespace IPC
{
  namespace TCP
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
      PortType const INVALID_PORT = 0;

    public:

      Port() : m_port(INVALID_PORT) {}
      Port(PortType);
      Port & operator=(Port const &);

      char const * Build(char const * a_buf);
      void Serialize(std::vector<char> &) const;

      void Set(PortType);
      bool Set(std::string const &);

      PortType As_uint16() const;
      std::string As_string() const;

      bool IsValid() const;
      void SetToInvalid();

    private:

      PortType m_port;
    };

    //If the pNodeName parameter points to a string equal to "localhost",
    //all loopback addresses on the local computer are returned.
    //If the pNodeName parameter contains an empty string, 
    //all registered addresses on the local computer are returned.
    class SocketData
    {
    public:

      char const * Build(char const * a_buf);
      void Serialize(std::vector<char> &) const;

      bool operator==(SocketData const & b) const;
      bool operator!=(SocketData const & b) const;

      std::string Get_IP() const;
      Port Get_Port() const;

      void Set_IP(std::string const &);
      void Set_Port(Port);

    public:

      std::string m_ipAddr;
      Port        m_port;
    };

    bool Init(void(*Log)(std::string const &, int));
    bool Shutdown();

    bool GetSocketData(SOCKET, SocketData &);
    bool Recv(SOCKET, std::vector<char> &);
    bool Send(SocketData const & server, std::vector<char> const & message);
    bool SendQuery(SocketData const & server,
                   std::vector<char> const & message,
                   std::vector<char> & response);
  }
}


#endif