﻿
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "Application.h"
#include "UI.h"
#include "DgParser_INI.h"
#include "DgStringFunctions.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

Application* Application::s_app(nullptr);

Application::Application() 
  : m_window(nullptr)
  , m_shouldQuit(false)
  , m_mouseScroll(0.0)
{
  s_app = this;
}

bool Application::Init()
{
  GetConfiguration();

  if (!InitGL())
  {
    return false;
  }

  ImGui_ImplGlfwGL3_Init(m_window, true);
  ClearProject();

  //DEBUG
  //LoadProject("./maps/simple_cp.cp");
  LoadProject("./maps/map_01.cp");

  return true;
}


bool Application::InitGL()
{
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 0);

  m_window = glfwCreateWindow(m_windowWidth,
                              m_windowHeight,
                              m_info.title,
                              nullptr,
                              nullptr);
  
  if (!m_window)
  {
    fprintf(stderr, "Failed to open m_window\n");
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(m_window);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  // Display version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);
  return true;
}

void Application::Shutdown()
{
}

void Application::ClearProject()
{
  m_appState.projName.clear();
}

void Application::PushEvent(Event const & a_event)
{
  m_eventManager.PushEvent(a_event);
}

void Application::KeyEvent(int a_key, int a_action)
{
  switch (a_key)
  {
  case GLFW_KEY_ESCAPE:
  {
    m_shouldQuit = true;
    break;
  }
  }
}

void Application::UpdateScroll(double a_val)
{
  if (!ImGui::GetIO().WantCaptureMouse)
  {
    m_mouseScroll += a_val;
  }
}

void AppOnKeyEvent(int a_key, int a_action)
{
  Event_KeyEvent e;
  e.SetKey(a_key);
  e.SetAction(a_action);
  Application::GetInstance()->PushEvent(e);
}

void AppOnMouseScroll(double yOffset)
{
  Event_MouseScroll e;
  e.SetOffset(yOffset);
  Application::GetInstance()->PushEvent(e);
}

void Application::HandleEvents()
{
  //Get user input
  ShowMainGUIWindow();

  eObject e(nullptr);
  while (m_eventManager.PollEvent(e))
  {
    e->DoEvent();
  }
}

void Application::DoLogic(double a_dt)
{
}


void Application::DrawPolygonEdges()
{
  for (auto const & kv : m_polygons)
  {
    auto it0 = kv.second.chead();
    for (size_t i = 0; i < kv.second.size(); i++)
    {
      auto it1 = it0;
      it1++;


    }
  }
}

void Application::Render()
{
  m_renderer.Begin();

  
  DrawPolygonEdges();

  m_renderer.End();
}

std::vector<std::string> Application::GetProjects()
{
  std::vector<std::string> result;
  WIN32_FIND_DATA ffd;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  std::string szDir = m_projectPath + "*";

  hFind = FindFirstFile(szDir.c_str(), &ffd);

  if (INVALID_HANDLE_VALUE == hFind)
  {
    return result;
  }

  do
  {
    if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
      size_t pos = PathFindExtension(ffd.cFileName) - &ffd.cFileName[0];
      std::string ext = (std::string(ffd.cFileName).substr(pos + 1));
      if (ext == m_fileExt)
      {
        result.push_back(ffd.cFileName);
      }
    }
  } while (FindNextFile(hFind, &ffd) != 0);

  FindClose(hFind);
  return result;
}

void Application::GetConfiguration()
{
  //Set defaults
  strcpy_s(m_info.title, "Dg Paricle System Example");

  Dg::Parser_INI parser;
  Dg::ErrorCode result = parser.Parse(m_configFileName);

  if (result == Dg::ErrorCode::FailedToOpenFile)
  {
    fprintf(stderr, "Failed to open config file '%s'. Using defaults...\n", m_configFileName.c_str());
    return;
  }
  else if (result != Dg::ErrorCode::None)
  {
    fprintf(stderr, "Failed trying to parse config file '%s'. Using defaults...\n", m_configFileName.c_str());
    return;
  }

  for (int i = 0; i < parser.GetItems().size(); ++i)
  {
    
  }
}

void Application::UI_NewFrame()
{
  glfwPollEvents();
  ImGui_ImplGlfwGL3_NewFrame();

  //Editor
  {
    ImGui::Begin("Editor", nullptr, 
        ImGuiWindowFlags_MenuBar
      | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowPos(ImVec2(3, 3));
    ImGui::SetWindowSize(ImVec2(200, m_windowHeight - 10));
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
    ImGui::Text(((m_appState.projName == "") ? "New Project" : m_appState.projName.c_str()));

    //ImGuiID id = ImGui::GetID("##Title");
    ImVec2 size = ImGui::GetItemRectSize();
    ImGui::SetWindowPos(ImVec2(((float)m_windowWidth - size.x) / 2.0f, indent));
    ImGui::End();
  }

  //Example UI window
  if (UI::showExampleWindow)
  {
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
    ImGui::ShowTestWindow(&UI::showExampleWindow);
  }
}

void Application::Run(Application* the_app)
{
  s_app = the_app;

  if (!Init())
  {
    fprintf(stderr, "Failed to initialize Application\n");
    return;
  }

  double lastTick = glfwGetTime();

  do
  {
    double thisTick = glfwGetTime();
    double dt = static_cast<float>(thisTick - lastTick);
    lastTick = thisTick;

    UI_NewFrame();
    HandleEvents();
    DoLogic(dt);
    Render();

	  ImGui::Render();
    glfwSwapBuffers(m_window);

  } while (!glfwWindowShouldClose(m_window) && !m_shouldQuit);

  Shutdown();

  ImGui_ImplGlfwGL3_Shutdown();
  glfwDestroyWindow(m_window);
  glfwTerminate();
}
