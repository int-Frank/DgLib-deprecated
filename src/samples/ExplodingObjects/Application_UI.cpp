

#include <vector>
#include <string>

#include "Application.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "UI.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

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


void Application::UI_NewFrame()
{
  glfwPollEvents();
  ImGui_ImplGlfwGL3_NewFrame();

  int winWidth(0), winHeight(0);
  glfwGetWindowSize(m_window, &winWidth, &winHeight);

  //Editor
  {
    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_MenuBar);
    ImGui::End();
  }

  //Title window
  {
    float indent = 5.0f;
    ImGui::Begin("##Title", nullptr
      , ImGuiWindowFlags_NoTitleBar
      | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_AlwaysAutoResize
      | ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_NoCollapse
      | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Text(((m_appData.projName == "") ? "New Project" : m_appData.projName.c_str()));

    //ImGuiID id = ImGui::GetID("##Title");
    ImVec2 size = ImGui::GetItemRectSize();
    ImGui::SetWindowPos(ImVec2(((float)winWidth - size.x) / 2.0f, indent));
    ImGui::End();
  }

  //FPS
  if (UI::showMetrics)
  {
    ImGui::Begin("Stats", nullptr);
    ImGui::Text("FPS %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
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
    ImGui::Text("A simple exploding teapot demo\n\nFrank Hart 2017\n.");
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

  ImGui::Separator();

  const char* listbox_items[] = { "Complete", "Chunked"};
  ImGui::ListBox("Explosion type", &m_appData.explodeType, listbox_items, IM_ARRAYSIZE(listbox_items), 3);

  ImGui::Separator();
  m_appData.explode = ImGui::Button("EXPLODE!", ImVec2(100, 30));
  m_appData.reset = ImGui::Button("Reset", ImVec2(100, 30));
  ImGui::End();
}
