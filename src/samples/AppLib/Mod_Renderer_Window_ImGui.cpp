
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
  {
    float marginX = 9.0f;
    float marginY = 18.0f;
    float width = float(a_data.width) + 2.0f * marginX;
    float height = float(a_data.height) + 2.0f * marginY;

    ImGui::SetNextWindowSize(ImVec2(width, height));
    ImGui::SetNextWindowPos(ImVec2(float(a_data.indentX), float(a_data.indentY)));
    ImGui::Begin(a_data.name.c_str(), nullptr, a_data.ImGuiWindowFlags);
    ImGui::End();
  }

  void Window_ImGui::Update()
  {
    ImGui::Begin(m_name.c_str());

    ImGui::GetWindowDrawList()->AddImage(
      (void *)GetTexture(), 
      ImVec2(ImGui::GetCursorScreenPos()),
      ImVec2(ImGui::GetCursorScreenPos().x + GetWidth(), 
        ImGui::GetCursorScreenPos().y + GetHeight()), 
      ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
  }
}