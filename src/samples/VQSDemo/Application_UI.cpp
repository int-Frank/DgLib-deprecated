
#include <stdio.h>

#include "Application.h"
#include "imgui.h"
#include "UI.h"

static void CreateSpacing(int a_n)
{
  for (int i = 0; i < a_n; ++i) { ImGui::Spacing(); }
}

void Application::ShowMainGUIWindow()
{
  //Settings
  ImVec4  const   headingClr(1.0f, 0.0f, 1.0f, 1.0f);
  int     const   nSpacing        = 3;

  ImGui::Begin("Editor");

  //----------------------------------------------------------------------------------
  //  Menu Bar
  //----------------------------------------------------------------------------------
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("Help"))
    {
      if (ImGui::MenuItem("About", NULL, &UI::showAbout))
      {
        m_windowStack.push(Modal::ViewAbout);
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }


  //----------------------------------------------------------------------------------
  //  About Window
  //----------------------------------------------------------------------------------
  if (!m_windowStack.empty() && m_windowStack.top() == Modal::ViewAbout)
  {
    ImGui::OpenPopup("About");
  }
  if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("This app demonstrates the VQS structure.");
    if (ImGui::Button("OK", ImVec2(130, 0)))
    {
      UI::showAbout = false;
      m_windowStack.pop();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  ImGui::End();

  m_model.ShowEditor();
}
