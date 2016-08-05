

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

    if (ImGui::BeginPopupContextItem("item context menu"))
    {
      if (ImGui::Selectable("Delete selected"))
      {
        if (*a_currentItem >= 0)
        {
          Event_DeleteFile e;
          e.SetFileName(m_projectPath + a_items[*a_currentItem]);
          PushEvent(e);
        }
      }
      ImGui::EndPopup();
    }
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

  //Statics for saving...
  static int          save_currentItem = -1;
  static int          save_lastItem = -2;
  static char         save_buf[128] = {};
  static std::string  save_finalFile;

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
      if (ImGui::MenuItem("New")) 
      {
        m_windowStack.push(Modal::NewProjectRequest);
        if (m_appData.dirty)
        {
          m_windowStack.push(Modal::SavePrompt);
        }
      }
      if (ImGui::MenuItem("Open"))
      {
        m_windowStack.push(Modal::OpenWindow);
        if (m_appData.dirty)
        {
          m_windowStack.push(Modal::SavePrompt);
        }
      }
      if (ImGui::MenuItem("Save")) 
      {
        if (m_appData.projName == "")
        {
          m_windowStack.push(Modal::SaveAsWindow);
        }
        else
        {
          printf("'%s' saved from menu bar!\n", (m_projectPath + m_appData.projName).c_str());
        }
      }
      if (ImGui::MenuItem("Save As.."))
      {
        m_windowStack.push(Modal::SaveAsWindow);
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
  //  New Project created info box
  //----------------------------------------------------------------------------------
  if (!m_windowStack.empty() && m_windowStack.top() == Modal::NewProjectRequest)
  {
    Event_NewProject e;
    m_eventManager.PushEvent(e);
    m_windowStack.pop();
  }


  //----------------------------------------------------------------------------------
  //  Save Prompt
  //----------------------------------------------------------------------------------
  if (!m_windowStack.empty() && m_windowStack.top() == Modal::SavePrompt)
  {
    ImGui::OpenPopup("Save current Project?");
  }
  if (ImGui::BeginPopupModal("Save current Project?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    bool finished = false;
    if (ImGui::Button("Yes", ImVec2(80, 0)))
    {
      m_windowStack.pop();
      m_windowStack.push(Modal::SaveAsWindow);
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("No", ImVec2(80, 0)))
    {
      m_windowStack.pop();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }


  //----------------------------------------------------------------------------------
  //  Overwrite Prompt
  //----------------------------------------------------------------------------------
  if (!m_windowStack.empty() && m_windowStack.top() == Modal::OverwriteWindow)
  {
    ImGui::OpenPopup("Overwrite?");
  }
  if (ImGui::BeginPopupModal("Overwrite?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("File already exists. Overwrite?");
    if (ImGui::Button("Yes", ImVec2(80, 0)))
    {
      //Save...
      m_appData.projName = std::string(save_buf);
      Event_SaveProject e;
      e.SetFileName(save_finalFile);
      m_eventManager.PushEvent(e);

      //Reset
      memset(save_buf, 0, sizeof(save_buf) / sizeof(char));
      save_lastItem = -2;
      save_currentItem = -1;
      m_windowStack.pop();
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("No", ImVec2(80, 0)))
    {
      m_windowStack.pop();
      m_windowStack.push(Modal::SaveAsWindow);
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }


  //----------------------------------------------------------------------------------
  //  Save As Window
  //----------------------------------------------------------------------------------
  if (!m_windowStack.empty() && m_windowStack.top() == Modal::SaveAsWindow)
  {
    ImGui::OpenPopup("Save As..");
  }
  if (ImGui::BeginPopupModal("Save As..", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    std::vector<std::string> files = GetProjects();

    CreateFileList(files, "Files", &save_currentItem);

    if (save_currentItem != save_lastItem && save_currentItem >= 0)
    {
      strcpy_s(save_buf, 128, files[save_currentItem].data());
      save_lastItem = save_currentItem;
    }
    ImGui::InputText("File name", save_buf, 128);

    bool shouldClose = false;
    bool shouldReset = true;
    if (ImGui::Button("Save", ImVec2(120, 0)))
    {
      save_finalFile = m_projectPath + std::string(save_buf);
      std::string dotExt = std::string(".") + m_fileExt;
      if (save_finalFile.rfind(dotExt) != save_finalFile.size() - 4)
      {
        save_finalFile += dotExt;
      }
      m_windowStack.pop();
      shouldClose = true;
      if (FileExists(save_finalFile))
      {
        m_windowStack.push(Modal::OverwriteWindow);
        shouldReset = false;
      }
      else
      {
        //Save...
        m_appData.projName = std::string(save_buf);
        Event_SaveProject e;
        e.SetFileName(save_finalFile);
        m_eventManager.PushEvent(e);
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0)))
    {
      m_windowStack.pop();
      shouldClose = true;
    }

    if (shouldClose)
    {
      if (shouldReset)
      {
        memset(save_buf, 0, sizeof(save_buf) / sizeof(save_buf[0]));
        save_lastItem = -2;
        save_currentItem = -1;
      }

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

  ImGui::End();
}
