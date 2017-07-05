#include <sstream>
#include <mutex>
#include <atomic>

#include <GL/glew.h>

#include "IPC_TCP_Server.h"

#include "ServerState_On.h"
#include "ServerState_Off.h"
#include "DgDoublyLinkedList.h"
#include "DgTimer.h"
#include "DgTypes.h"
#include "Event.h"
#include "IPC_TCP_common.h"
#include "imgui.h"

static void LogCallback(std::string const & a_message, int a_val)
{
  DgApp::GetInstance()->LogToOutputWindow(a_message, a_val);
}

TCP_Server::TCP_Server()
  : m_currentState(State_Off)
  , m_desiredState(State_Off)
  , m_pServerState(new ServerState_Off(this))
{
  strcpy_s(m_ipBuf, "127.0.0.1");
  strcpy_s(m_portBuf, "5555");

  if (!IPC::TCP::Init(LogCallback))
  {
    LogToFile("IPC::TCP::Init() failed", Dg::LL_Error);
    throw AppInitFailed;
  }
}

TCP_Server::~TCP_Server()
{
  delete m_pServerState;
  m_pServerState = nullptr;
  IPC::TCP::Shutdown();
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

      IPC::TCP::Port port;
      if (!port.Set(portStr))
      {
        LogToOutputWindow("Server startup failed.", Dg::LL_Error);
        break;
      }
      if (!port.IsValid())
      {
        LogToOutputWindow("Invalid port number. Server startup failed.", Dg::LL_Error);
        break;
      }

      IPC::TCP::SocketData sd;
      sd.Set_Port(port);
      sd.Set_IP(ipAddr);

      delete m_pServerState;
      m_pServerState = new ServerState_On(this, sd);
      break;
    }
    case State_Off:
    {
      delete m_pServerState;
      m_pServerState = new ServerState_Off(this);
      break;
    }
    }
    m_currentState = m_desiredState;
  }
}

void TCP_Server::BuildUI()
{
  ImGui::Begin("MainPanel", nullptr, ImGuiWindowFlags_NoCollapse);

  ImGui::TextColored(ImVec4(0.0, 1.0, 1.0, 1.0), "IP   :"); ImGui::SameLine();
  ImGui::InputText(" ", m_ipBuf, s_textBufLen, ImGuiInputTextFlags_CharsDecimal);
  ImGui::TextColored(ImVec4(0.0, 1.0, 1.0, 1.0), "PORT :"); ImGui::SameLine();
  ImGui::InputText("  ", m_portBuf, s_textBufLen, ImGuiInputTextFlags_CharsDecimal);

  ImGui::Spacing();
  ImGui::Spacing();

  ImGui::Indent(80.f);
  switch (m_currentState)
  {
  case State_On:
  {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0.0, 0.0, 1.0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8, 0.0, 0.0, 1.0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8, 0.0, 0.0, 1.0));
    if (ImGui::Button(" Stop  ")) m_desiredState = State_Off;
    break;
  }
  case State_Off:
  {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0, 0.7, 0.0, 1.0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0, 0.7, 0.0, 1.0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0, 0.7, 0.0, 1.0));
    if (ImGui::Button(" Start ")) m_desiredState = State_On;
    break;
  }
  default:
  {
    break;
  }
  }

  ImGui::PopStyleColor(3);
  ImGui::Unindent(80.0f);
  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Separator();
  ImGui::End();

  m_pServerState->BuildUI();
}