

#include <vector>
#include <string>

#include "Application.h"
#include "imgui.h"
#include "UI.h"

static void CreateSpacing(int a_n)
{
  for (int i = 0; i < a_n; ++i) { ImGui::Spacing(); }
}

void Application::CreateFileList(std::vector<std::string> const & a_items
                               , char const * a_name
                               , int * a_currentItem)
{
  if (a_items.size() == 0)
  {
    ImGui::ListBox(a_name, a_currentItem, nullptr, 0, 5);
    *a_currentItem = -1;
  }
  else
  {
    char * * pItems = new char*[a_items.size()];
    for (int i = 0; i < a_items.size(); ++i)
    {
      pItems[i] = new char[a_items[i].size() + 1]();
      memcpy(pItems[i], a_items[i].data(), a_items[i].size() * sizeof(char));
    }
    if (a_items.size() == 0)
    {
      *a_currentItem = -1;
    }
    ImGui::ListBox(a_name, a_currentItem, (char const **)pItems, (int)a_items.size(), 5);

    for (int i = 0; i < a_items.size(); ++i)
    {
      delete[] pItems[i];
    }
    delete[] pItems;
  }
}

static bool FileExists(std::string const & a_name)
{
  if (FILE *file = fopen(a_name.c_str(), "r"))
  {
    fclose(file);
    return true;
  }
  else
  {
    return false;
  }
}

void Application::ShowMainGUIWindow()
{
  //Settings
  ImVec4  const   headingClr(1.0f, 0.0f, 1.0f, 1.0f);
  int     const   nSpacing        = 3;
  float   const   sliderOffset    = -90.0f;

  //Statics for opening...
  static int          open_currentItem = -1;

  ImGui::Begin("Editor");

  //----------------------------------------------------------------------------------
  //  Menu Bar
  //----------------------------------------------------------------------------------
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Open"))
      {
        m_windowStack.push(Modal::OpenWindow);
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Quit", "Esc")) { m_shouldQuit = true; }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help"))
    {
      if (ImGui::MenuItem("View Help", NULL, &UI::showHelp))
      {
        m_windowStack.push(Modal::ViewHelp);
      }
      ImGui::Separator();
      if (ImGui::MenuItem("About", NULL, &UI::showAbout))
      {
        m_windowStack.push(Modal::ViewAbout);
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  //----------------------------------------------------------------------------------
  //  Help Window
  //----------------------------------------------------------------------------------
  if (!m_windowStack.empty() && m_windowStack.top() == Modal::ViewHelp)
  {
    ImGui::OpenPopup("Help");
  }
  if (ImGui::BeginPopupModal("Help", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Help window");
    ImGui::Text(""); ImGui::SameLine(ImGui::GetWindowWidth() - 140.0f);
    if (ImGui::Button("OK", ImVec2(130, 0)))
    {
      UI::showHelp = false;
      m_windowStack.pop();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
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
    ImGui::Text("About window");
    if (ImGui::Button("OK", ImVec2(130, 0)))
    {
      UI::showAbout = false;
      m_windowStack.pop();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }


  //----------------------------------------------------------------------------------
  //  Open Project Window
  //----------------------------------------------------------------------------------
  if (!m_windowStack.empty() && m_windowStack.top() == Modal::OpenWindow)
  {
    ImGui::OpenPopup("Open file");
  }
  if (ImGui::BeginPopupModal("Open file", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    std::vector<std::string> files = GetProjects();
    CreateFileList(files, "Files", &open_currentItem);

    bool finished = false;
    if (ImGui::Button("Open", ImVec2(120, 0)))
    {
      if (open_currentItem != -1)
      {
        m_appData.projName = files[open_currentItem];
        Event_LoadProject e;
        e.SetFileName(m_projectPath + files[open_currentItem]);
        m_eventManager.PushEvent(e);
      }
      finished = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0)))
    {
      finished = true;
    }
    if (finished)
    {
      open_currentItem = -1;
      m_windowStack.pop();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  //----------------------------------------------------------------------------------
  //  UI...
  //----------------------------------------------------------------------------------

  /*
  Sliders: power, number of pieces [selection of few, many, lots...]
           retarding force, 
  Button: EXPLODE!
  */

  ImGui::SliderFloat("Power", &m_appData.power, 0.f, 10.f, "%0.1f");
  ImGui::SliderFloat("Ret. Force", &m_appData.retardingForce, 0.f, 10.f, "%0.1f");
  ImGui::SliderInt("Pieces", &m_appData.numberPieces, 1, 3, "%0.f Few    Some    Lots");
  ImGui::Separator();
  m_appData.explode = ImGui::Button("EXPLODE!", ImVec2(100, 30));
  ImGui::End();
}
