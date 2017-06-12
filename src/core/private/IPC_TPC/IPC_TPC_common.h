#ifndef IPC_TCP_COMMON_H
#define IPC_TCP_COMMON_H

#include <windows.h>

#include <vector>
#include <string>
#include <stdint.h>
#include <atomic>

#include "DgIPC.h"

namespace Dg
{
  namespace IPC
  {
    extern void(*g_Log)(std::string const &, LogLevel);

    enum PacketID
    {
      E_None = 1,
      E_IPAddressRequest,
      E_IPAddressResponse,
      E_RegisterClient,
      E_ClientExiting,
      E_Dispatch
    };

    class PayloadHeader
    {
    public:

      char const * Build(char const *);
      std::vector<char> Serialize() const;
      static size_t Size();

    public:

      uint16_t      ID;
      uint32_t      payloadSize;
    };

    class Port
    {
    public:

      typedef uint16_t PortType;

    public:

      Port() {}
      Port(PortType a_val);

      char const * Build(char const *);
      std::vector<char> Serialize() const;

      void Set(PortType);

      PortType As_uint16() const;
      std::string As_string() const;

    private:

      PortType m_port;
    };

    class SocketData
    {
    public:

      char const * Build(char const *);
      std::vector<char> Serialize() const;

      bool operator==(SocketData const & b) const;

      std::string Get_IP() const;
      Port Get_Port() const;

      void Set_IP(std::string const &);
      void Set_Port(Port);

    public:

      std::string m_ipAddr;
      Port        m_port;
    };

    bool Init(void(*a_Log)(std::string const &, LogLevel));

    bool InitWinsock();
    void ShutdownWinsock();

    bool Recv(SOCKET, std::vector<char> &, std::atomic<bool> const &);
    bool Send(SocketData, std::vector<char> const &, std::atomic<bool> const &);
    bool GetSocketData(SOCKET, SocketData &);
  }
}


#endif