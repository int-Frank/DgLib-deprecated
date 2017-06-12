#include <sstream>
#include <mutex>
#include <atomic>

#include <GL/glew.h>

#include "IPC_TCP_Server.h"

#include "DgDoublyLinkedList.h"
#include "DgTimer.h"
#include "DgTypes.h"
#include "Event.h"
#include "IPC_TCP_common.h"
#include "imgui.h"
#include "Listener.h"

namespace IPC
{
  static void Run_Listen(TCP_Server * a_pApp, SocketData a_listenSocket)
  {
    Listener listener(a_pApp);
    if (listener.Init(a_listenSocket))
    {
      listener.Run();
    }
  }

  void Logger(std::string const & a_message, int a_val)
  {
    DgApp::GetInstance()->LogToOutputWindow(a_message, a_val);
  }

  TCP_Server::TCP_Server()
    : m_currentState(State_Off)
    , m_desiredState(State_Off)
    , m_shouldStop(true)
  {
    strcpy_s(m_ipBuf, "127.0.0.1");
    strcpy_s(m_portBuf, "5555");

    if (!IPC::InitWinsock())
    {
      LogToFile("winsock failed to initialise.", Dg::LL_Error);
      throw AppInitFailed;
    }

    IPC::SetLoggingFunction(Logger);
  }

  TCP_Server::~TCP_Server()
  {
    if (m_currentState == State_On)
    {
      Stop();
    }
    ShutdownWinsock();
  }

  void TCP_Server::DoFrame(double a_dt)
  {
    glClear(GL_COLOR_BUFFER_BIT);

    if (m_desiredState != m_currentState)
    {
      switch (m_desiredState)
      {
      case State_On:
      {
        Start();
        break;
      }
      case State_Off:
      {
        Stop();
        break;
      }
      }
      m_currentState = m_desiredState;
    }
  }

  void TCP_Server::BuildUI()
  {
    ImGui::SetNextWindowSize(ImVec2(300, 90));
    ImGui::Begin("MainPanel", nullptr, ImGuiWindowFlags_NoTitleBar
                                     | ImGuiWindowFlags_NoResize
                                     | ImGuiWindowFlags_NoMove
                                     | ImGuiWindowFlags_NoScrollbar
                                     | ImGuiWindowFlags_NoScrollWithMouse
                                     | ImGuiWindowFlags_NoCollapse
                                     | ImGuiWindowFlags_NoSavedSettings);
    
    switch (m_currentState)
    {
    case State_On:
    {
      ImGui::TextColored(ImVec4(0.0, 1.0, 1.0, 1.0), "IP   :"); ImGui::SameLine();
      ImGui::Text(m_ipBuf);
      ImGui::SetCursorPosY(31.f);
      ImGui::TextColored(ImVec4(0.0, 1.0, 1.0, 1.0), "PORT :"); ImGui::SameLine();
      ImGui::Text(m_portBuf);

      ImGui::SetCursorPosY(62.f);

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0.0, 0.0, 1.0));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8, 0.0, 0.0, 1.0));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8, 0.2, 0.2, 1.0));
      ImGui::Indent(80.f);
      if (ImGui::Button(" Stop  ")) m_desiredState = State_Off;
      ImGui::PopStyleColor(3);
      ImGui::Indent(-80.f);
      ImGui::End();
      break;
    }
    case State_Off:
    {
      ImGui::TextColored(ImVec4(0.0, 1.0, 1.0, 1.0), "IP   :"); ImGui::SameLine();
      ImGui::InputText(" ", m_ipBuf, s_textBufLen, ImGuiInputTextFlags_CharsDecimal);
      ImGui::TextColored(ImVec4(0.0, 1.0, 1.0, 1.0), "PORT :"); ImGui::SameLine();
      ImGui::InputText("  ", m_portBuf, s_textBufLen, ImGuiInputTextFlags_CharsDecimal);

      ImGui::Spacing();
      ImGui::Spacing();

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0, 0.7, 0.0, 1.0));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0, 0.7, 0.0, 1.0));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2, 0.7, 0.2, 1.0));
      ImGui::Indent(80.f);
      if (ImGui::Button(" Start ")) m_desiredState = State_On;
      ImGui::PopStyleColor(3);
      ImGui::Indent(-80.f);
      ImGui::End();
      break;
    }
    default:
    {
      break;
    }
    }
  }

  void TCP_Server::RegisterClient(SocketData const & a_sd)
  {
    m_clientHandler.AddClient(a_sd);
  }

  void TCP_Server::DeregisterClient(SocketData const & a_sd)
  {
    m_clientHandler.RemoveClient(a_sd);
  }

  void TCP_Server::RegisterThread()
  {
    m_activeThreads++;
  }

  void TCP_Server::DeregisterThread()
  {
    m_activeThreads--;
  }

  std::vector<SocketData> TCP_Server::GetClientList()
  {
    return m_clientHandler.GetCurrentClients();
  }

  bool TCP_Server::ShouldStop() const
  {
    return m_shouldStop;
  }

  void TCP_Server::Start()
  {
    m_shouldStop = false;

    std::string portStr(m_portBuf);
    std::string ipAddr(m_ipBuf);

    Port port;
    if (!port.Set(portStr))
    {
      return;
    }

    m_listenSocketData.Set_Port(port);
    m_listenSocketData.Set_IP(ipAddr);

    m_listenThread = std::thread(Run_Listen, this, m_listenSocketData);
  }

  void TCP_Server::Stop()
  {
    LogToOutputWindow("Shutting down server...", Dg::LL_Log);

    m_shouldStop = true;

    //Send message to wake up listener thread
    Message message;
    message.header.ID = IPC::E_ServerStop;
    message.header.payloadSize = 0;

    std::vector<char> data = message.Serialize();
    if (Send(m_listenSocketData, data))
    {
      //join listener thread
      m_listenThread.join();
    }
    else
    {
      LogToOutputWindow("Unable to wake listener thread!", Dg::LL_Warning);
    }

    //Ensure all worker threads are done
    int elapsedTime = 0;
    while (m_activeThreads > 0)
    {
      int const sleepTime = 200;
      Sleep(sleepTime);
      elapsedTime += sleepTime;
    }

    LogToOutputWindow("Done!", Dg::LL_OK);
  }
}