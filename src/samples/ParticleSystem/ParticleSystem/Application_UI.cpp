

#include <vector>
#include <string>

#include "Application.h"
#include "imgui/imgui.h"
#include "EmitterFactory.h"
#include "AttractorFactory.h"
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
  float   const   vRotMins[2]     = { 0.0f, 0.0f };
  float   const   vRotMaxs[2]     = { Dg::PI_f * 2.0f, Dg::PI_f };
  char    const * vRotformats[2]  = { "yaw = %.2f", "pitch = %.2f" };
  float   const   powers2[2]      = { 1.0f, 1.0f };
  float   const   posMins[3]      = { -10.0f, -10.0f, -10.0f };
  float   const   posMaxs[3]      = { 10.0f, 10.0f, 10.0f };
  char    const * posFormats[3]   = { "x = %.2f", "y = %.2f", "z = %.2f" };
  float   const   powers3[3]      = { 1.0f, 1.0f, 1.0f };

  //Statics for saving...
  static int          save_currentItem = -1;
  static int          save_lastItem = -2;
  static char         save_buf[128] = {};
  static std::string  save_finalFile;

  //Statics for opening...
  static int          open_currentItem = -1;

  ImGui::Begin("Partice System");

  //----------------------------------------------------------------------------------
  //  Menu Bar
  //----------------------------------------------------------------------------------
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("New")) 
      {
        Event_NewProject e;
        m_eventManager.PushEvent(e);
      }
      if (ImGui::MenuItem("Open"))
      {
        m_windowStack.push(Modal::OpenWindow);
        m_windowStack.push(Modal::SavePrompt);
      }
      if (ImGui::MenuItem("Save")) 
      {
        if (m_projData.name == "")
        {
          m_windowStack.push(Modal::SaveAsWindow);
        }
        else
        {
          Event_SaveProject e;
          e.SetFileName(m_projData.name);
          m_eventManager.PushEvent(e);
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
    if (ImGui::BeginMenu("Options"))
    {
      ImGui::MenuItem("Example UI", NULL, &UI::showExampleWindow);
      ImGui::Separator();
      ImGui::MenuItem("Blending", NULL, &UI::showAlphaBlendingWindow);
      ImGui::MenuItem("Metrics", NULL, &UI::showMetrics);
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help"))
    {
      ImGui::MenuItem("About ImGui", NULL, &UI::showAbout);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
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
      m_projData.name = std::string(save_buf);
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
        m_projData.name = std::string(save_buf);
        Event_SaveProject e;
        e.SetFileName(save_finalFile);
        m_eventManager.PushEvent(e);
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Close", ImVec2(120, 0)))
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
        m_projData.name = files[open_currentItem];
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
  //  General particle sytem options
  //----------------------------------------------------------------------------------
  if (ImGui::CollapsingHeader("Particle system options", ImGuiTreeNodeFlags_DefaultOpen))
  {
    ImGui::TextColored(headingClr, "Optional updaters");
    ImGui::Checkbox("Color", &m_projData.parSysOpts[0].useUpdaterColor);
    ImGui::Checkbox("Size", &m_projData.parSysOpts[0].useUpdaterSize);
    ImGui::Checkbox("Rel force", &m_projData.parSysOpts[0].useUpdaterRelativeForce);
  }

  
  //----------------------------------------------------------------------------------
  //  Emitters
  //----------------------------------------------------------------------------------
  if (ImGui::CollapsingHeader("Emitters", ImGuiTreeNodeFlags_DefaultOpen))
  {
    if (ImGui::Button("Add Emitter"))
    {
      AddEmitter();
    }

    ImGui::SameLine();
    if (ImGui::Button("Kill Emitter") && m_projData.eData.size())
    {
      m_particleSystem.RemoveEmitter(m_projData.eData[m_focusEmitter].first.ID);
      m_IDManager.ReturnID(m_projData.eData[m_focusEmitter].first.ID);
      m_projData.eData.erase(m_projData.eData.begin() + m_focusEmitter);
      if (m_focusEmitter == m_projData.eData.size())
      {
        m_focusEmitter--;
      }
    }

    //This doesn't work :( Have to use char**
    //typedef char inner_array_t[32];
    //inner_array_t * currentEmitters = new inner_array_t[4];

    char ** currentEmitters = new char*[m_projData.eData.size()];
    for (int i = 0; i < m_projData.eData.size(); ++i)
    {
      currentEmitters[i] = new char[32]();
      sprintf_s(currentEmitters[i], 32, "Emitter %i", m_projData.eData[i].first.ID);
    }
    ImGui::PushItemWidth(sliderOffset);
    ImGui::ListBox("Emitter", &m_focusEmitter, (char const **)currentEmitters, (int)m_projData.eData.size(), 5);
    ImGui::PopItemWidth();
    ImGui::Separator();

    for (int i = 0; i < m_projData.eData.size(); ++i)
    {
      delete[] currentEmitters[i];
    }
    delete[] currentEmitters;

    if (m_projData.eData.size() > 0)
    {
      EmitterData & curEmData = m_projData.eData[m_focusEmitter].first;

      CreateSpacing(nSpacing);
      ImGui::Checkbox("Turn emitter off/on", &curEmData.on);

      CreateSpacing(nSpacing);
      ImGui::TextColored(headingClr, "Emission method");
      ImGui::RadioButton("Linear", &curEmData.type, E_Emitter_Linear); ImGui::SameLine();
      ImGui::RadioButton("Random", &curEmData.type, E_Emitter_Random);

      CreateSpacing(nSpacing);
      ImGui::TextColored(headingClr, "Define emitter shape");
      ImGui::RadioButton("Point", &curEmData.posGenMethod, E_GenPosPoint); ImGui::SameLine();
      //ImGui::RadioButton("Box", &curEmData.posGenMethod, E_GenPosBox); ImGui::SameLine();
      ImGui::RadioButton("Sphere", &curEmData.posGenMethod, E_GenPosSphere);

      if (curEmData.posGenMethod == E_GenPosBox)
      {
        float mins[3] = { 0.0f, 0.0f, 0.0f };
        float maxs[3] = { 10.0f, 10.0f, 10.0f };
        char const * formats[3] = { "l = %.2f", "w = %.2f", "h = %.2f" };
        ImGui::PushItemWidth(sliderOffset);
        ImGui::SliderFloatNi("Dimensions", &curEmData.boxDim[0], 3, mins, maxs, formats, powers3);
        ImGui::PopItemWidth();
      }

      if (curEmData.posGenMethod == E_GenPosSphere)
      {
        CreateSpacing(nSpacing);
        ImGui::TextColored(headingClr, "Define sphere geometry");
        ImGui::PushItemWidth(sliderOffset);
        ImGui::SliderFloat("Radius", &curEmData.transform[6], 0.0f, 10.0f, "%.1f");
        ImGui::PopItemWidth();
      }

      {
        CreateSpacing(nSpacing);
        ImGui::TextColored(headingClr, "Place Emitter");
        ImGui::PushItemWidth(sliderOffset);
        ImGui::SliderFloatNi("Position", &curEmData.transform[0], 3, posMins, posMaxs, posFormats, powers3);
        ImGui::PopItemWidth();
      }


      //TODO 
      //if (UI::showEmitterPlacer)
      //{
      //ImGui::Begin("Place Emitter"); ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1 / 7.0f, 0.6f, 0.6f));
      //ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1 / 7.0f, 0.7f, 0.6f));
      //ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1 / 7.0f, 0.8f, 0.6f));
      //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
      //ImGui::Button("Drag Me", ImVec2(100.0f, 100.0f));
      //ImGui::PopStyleVar();
      //ImGui::PopStyleColor(3);
      //if (ImGui::IsItemActive())
      //{
      //ImVec2 rect_min = ImGui::GetItemRectMin();
      //ImVec2 rect_max = ImGui::GetItemRectMax();
      //
      //ImVec2 relPos;
      //relPos.x = (ImGui::GetIO().MousePos.x - rect_min.x) / (rect_max.x / rect_min.x);
      //relPos.y = (ImGui::GetIO().MousePos.y - rect_min.y) / (rect_max.y / rect_min.y);
      //
      //ImGui::Text("Rel pos (%.1f, %.1f)", relPos.x, relPos.y);
      //}
      //if (ImGui::Button("Close"))
      //{
      //UI::showEmitterPlacer = false;
      //}
      //ImGui::End();
      //}


      if (curEmData.posGenMethod == E_GenPosBox)
      {
        float mins[3] = { 0.0f, 0.0f, 0.0f };
        float maxs[3] = { Dg::PI_f * 2.0f, Dg::PI_f * 2.0f, Dg::PI_f * 2.0f };
        char const * formats[3] = { "rz = %.2f", "ry = %.2f", "rx = %.2f" };
        float powers[3] = { 1.0f, 1.0f, 1.0f };
        ImGui::PushItemWidth(sliderOffset);
        ImGui::SliderFloatNi("Rotation", &curEmData.transform[3], 3, mins, maxs, formats, powers);
        ImGui::PopItemWidth();
      }

      CreateSpacing(nSpacing);
      ImGui::TextColored(headingClr, "Define Initial velocity vector");
      ImGui::RadioButton("Direction", &curEmData.velGenMethod, E_GenVelCone); ImGui::SameLine();
      ImGui::RadioButton("Outwards", &curEmData.velGenMethod, E_GenVelOutwards);

      if (curEmData.velGenMethod == E_GenVelCone)
      {
        float mins[3] = { 0.0f, 0.0f, 0.0f };
        float maxs[3] = { Dg::PI_f * 2.0f, Dg::PI_f, Dg::PI_f };
        char const * formats[3] = { "yaw = %.2f", "pitch = %.2f", "spread = %.2f" };
        float powers[3] = { 1.0f, 1.0f, 1.0f };
        ImGui::PushItemWidth(sliderOffset);
        ImGui::SliderFloatNi("yaw, pitch, sprd", &curEmData.velCone[0], 3, mins, maxs, formats, powers);
        ImGui::PopItemWidth();
      }

      CreateSpacing(nSpacing);
      ImGui::TextColored(headingClr, "Other attributes");
      ImGui::PushItemWidth(sliderOffset);
      ImGui::ColorEdit4("Start color", curEmData.colors);
      ImGui::ColorEdit4("End color", &curEmData.colors[4]);
      ImGui::SliderFloat("Rate", &curEmData.rate, 0.0f, 500.0f, "%.2f par/s", 2.0f);
      ImGui::SliderFloat("Velocity", &curEmData.velocity, 0.0f, 10.0f, "%.2f m/s");
      if (m_projData.parSysOpts[0].useUpdaterRelativeForce)
      {
        ImGui::SliderFloat("Rel force", &curEmData.relativeForce, 0.0f, 10.0f, "%.4f m/s", 3.0f);
      }
      ImGui::SliderFloat("Life", &curEmData.life, 0.0f, 60.0f, "%.2f s");
      ImGui::SliderFloat("Start size", curEmData.sizes, 0.0f, 1.0f, "%.2f m");
      ImGui::SliderFloat("End size", &curEmData.sizes[1], 0.0f, 1.0f, "%.2f m");

      ImGui::PopItemWidth();
    }
  }

  //----------------------------------------------------------------------------------
  //  Attractors
  //----------------------------------------------------------------------------------
  if (ImGui::CollapsingHeader("Attractors", ImGuiTreeNodeFlags_DefaultOpen))
  {
    if (ImGui::Button("Add Attractor"))
    {
      AddAttractor();
    }

    ImGui::SameLine();
    if (ImGui::Button("Kill Attractor") && m_projData.aData.size())
    {
      m_particleSystem.RemoveUpdater(m_projData.aData[m_focusAttr].first.ID);
      m_IDManager.ReturnID(m_projData.aData[m_focusAttr].first.ID);
      m_projData.aData.erase(m_projData.aData.begin() + m_focusAttr);
      if (m_focusAttr == m_projData.aData.size())
      {
        m_focusAttr--;
      }
    }


    char ** currentAttractors = new char*[m_projData.aData.size()];
    for (int i = 0; i < m_projData.aData.size(); ++i)
    {
      currentAttractors[i] = new char[32]();
      sprintf_s(currentAttractors[i], 32, "Attractor %i", m_projData.aData[i].first.ID);
    }
    ImGui::PushItemWidth(sliderOffset);
    ImGui::ListBox("Attractor", &m_focusAttr, (char const **)currentAttractors, (int)m_projData.aData.size(), 5);
    ImGui::PopItemWidth();
    ImGui::Separator();

    for (int i = 0; i < m_projData.aData.size(); ++i)
    {
      delete[] currentAttractors[i];
    }
    delete[] currentAttractors;


    if (m_projData.aData.size() > 0)
    {
      CreateSpacing(nSpacing);

      AttractorData & curAttData = m_projData.aData[m_focusAttr].first;

      ImGui::Checkbox("Show attractor", &curAttData.show);

      ImGui::TextColored(headingClr, "Attractor type");
      ImGui::PushItemWidth(sliderOffset);

      const char* attrForces[] = { "Force is constant", "Force is a function of distance", "Force is a function of sq distance" };
      ImGui::ListBox("Force", &curAttData.appliedAccelType, attrForces, ((int)(sizeof(attrForces) / sizeof(*attrForces))), 3);

      const char* attrShapes[] = { "Global", "Point", "Line", "Plane" };
      ImGui::ListBox("Shape", &curAttData.type, attrShapes, ((int)(sizeof(attrShapes) / sizeof(*attrShapes))), 5);

      CreateSpacing(nSpacing);
      ImGui::TextColored(headingClr, "Strength");
      ImGui::SliderFloat("Strength", &curAttData.strength, -100.0f, 100.0f, "%.2f m/s");
      ImGui::SliderFloat("Max accel", &curAttData.maxAppliedAccelMag, 0.0f, 500.0f, "%.2f m/s", 2.0f);

      CreateSpacing(nSpacing);
      ImGui::TextColored(headingClr, "Position Attractor");
      switch (curAttData.type)
      {
      case E_AttGlobal:
      {
        ImGui::SliderFloatNi("Accel dir", &curAttData.transform[3], 2, vRotMins, vRotMaxs, vRotformats, powers2);
        break;
      }
      case E_AttPoint:
      {
        ImGui::SliderFloatNi("Att Pos", &curAttData.transform[0], 3, posMins, posMaxs, posFormats, powers3);
        break;
      }
      case E_AttLine:
      {
        ImGui::SliderFloatNi("Line origin", &curAttData.transform[0], 3, posMins, posMaxs, posFormats, powers3);
        ImGui::SliderFloatNi("Line dir", &curAttData.transform[3], 2, vRotMins, vRotMaxs, vRotformats, powers2);
        break;
      }
      case E_AttPlane:
      {

        ImGui::SliderFloatNi("Normal", &curAttData.transform[3], 2, vRotMins, vRotMaxs, vRotformats, powers2);
        ImGui::SliderFloatNi("Plane origin", &curAttData.transform[0], 3, posMins, posMaxs, posFormats, powers3);
        break;
      }
      default: //E_AttNone:
      {
        break;
      }
      }

      ImGui::PopItemWidth();
    }
  }
  ImGui::End();
}
