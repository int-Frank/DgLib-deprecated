
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mod_Renderer_Window_ImGui.h"
#include "imgui.h"

namespace Renderer
{

  Window_ImGui::~Window_ImGui()
  {

  }

  Window_ImGui::Window_ImGui(InitData const & a_data)
    : Window(a_data)
    , m_name(a_data.name)
    , m_marginX(17.0f)
    , m_marginY(36.0f)
  {
    float windowWidth = float(a_data.width) + m_marginX;
    float windowHeight = float(a_data.height) + m_marginY;

    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
    ImGui::SetNextWindowPos(ImVec2(float(a_data.indentX), float(a_data.indentY)));
    ImGui::Begin(a_data.name.c_str(), nullptr, a_data.ImGuiWindowFlags);
    ImGui::End();
  }

  void Window_ImGui::Update()
  {
    ImGui::Begin(m_name.c_str());

    ImVec2 dim = ImGui::GetWindowSize();

    float width = dim.x - m_marginX;
    float height = dim.y - m_marginY;

    ImGui::GetWindowDrawList()->AddImage(
      (void *)GetTexture(), 
      ImVec2(ImGui::GetCursorScreenPos()),
      ImVec2(ImGui::GetCursorScreenPos().x + width, 
             ImGui::GetCursorScreenPos().y + height), 
      ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
  }
}