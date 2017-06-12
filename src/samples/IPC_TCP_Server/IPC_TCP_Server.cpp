#include <sstream>
#include <mutex>
#include <atomic>

#include <GL/glew.h>

#include "IPC_TCP_Server.h"

#include "ServerState_On.h"
#include "DgDoublyLinkedList.h"
#include "DgTimer.h"
#include "DgTypes.h"
#include "Event.h"
#include "IPC_TCP_common.h"
#include "imgui.h"
#include "Listener.h"

static void Logger(std::string const & a_message, int a_val)
{
  DgApp::GetInstance()->LogToOutputWindow(a_message, a_val);
}

TCP_Server::TCP_Server()
  : m_currentState(State_Off)
  , m_desiredState(State_Off)
  , m_pServerState(nullptr)
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
  delete m_pServerState;
  m_pServerState = nullptr;
  IPC::ShutdownWinsock();
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
      std::string portStr(m_portBuf);
      std::string ipAddr(m_ipBuf);

      IPC::Port port;
      if (!port.Set(portStr))
      {
        return;
      }

      IPC::SocketData sd;
      sd.Set_Port(port);
      sd.Set_IP(ipAddr);

      delete m_pServerState;
      m_pServerState = new ServerState_On(this, sd);
      break;
    }
    case State_Off:
    {
      delete m_pServerState;
      m_pServerState = nullptr;
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

  ImGui::TextColored(ImVec4(0.0, 1.0, 1.0, 1.0), "IP   :"); ImGui::SameLine();
  ImGui::InputText(" ", m_ipBuf, s_textBufLen, ImGuiInputTextFlags_CharsDecimal);
  ImGui::TextColored(ImVec4(0.0, 1.0, 1.0, 1.0), "PORT :"); ImGui::SameLine();
  ImGui::InputText("  ", m_portBuf, s_textBufLen, ImGuiInputTextFlags_CharsDecimal);

  ImGui::Spacing();
  ImGui::Spacing();

  switch (m_currentState)
  {
  case State_On:
  {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0.0, 0.0, 1.0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8, 0.0, 0.0, 1.0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8, 0.0, 0.0, 1.0));
    ImGui::Indent(80.f);
    if (ImGui::Button(" Stop  ")) m_desiredState = State_Off;
    ImGui::PopStyleColor(3);
    ImGui::Indent(-80.f);
    ImGui::End();
    break;
  }
  case State_Off:
  {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0, 0.7, 0.0, 1.0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0, 0.7, 0.0, 1.0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0, 0.7, 0.0, 1.0));
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