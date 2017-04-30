
#include <windows.h>
#include <shlwapi.h>
#include <time.h>
#pragma comment(lib,"shlwapi.lib")

#include "DgApp.h"
#include "DgParser_INI.h"
#include "DgStringFunctions.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

DgApp* DgApp::s_app(nullptr);

DgApp::DgApp()
  : m_window(nullptr)
  , m_shouldQuit(false)
{
  s_app = this;
}

bool DgApp::Init()
{
  GetConfiguration();

  if (!InitGL())
  {
    return false;
  }

  m_pRenderer->Init();
  ImGui_ImplGlfwGL3_Init(m_window, true);
  return _Init();
}


bool DgApp::InitGL()
{
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_info.majorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_info.minorVersion);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, m_info.samples);

  if (m_info.fullscreen)
  {
    if (m_info.windowWidth == 0 || m_info.windowHeight == 0)
    {
      const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

      m_info.windowWidth = mode->width;
      m_info.windowHeight = mode->height;
    }
    m_window = glfwCreateWindow(m_info.windowWidth,
                                m_info.windowHeight,
                                m_info.title,
                                glfwGetPrimaryMonitor(),
                                nullptr);
  }
  else
  {
    m_window = glfwCreateWindow(m_info.windowWidth,
                                m_info.windowHeight,
                                m_info.title,
                                nullptr,
                                nullptr);
  }

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

void DgApp::Shutdown()
{
  m_pRenderer->Shutdown();
  _Shutdown();
}

void DgApp::PushEvent(Event const & a_event)
{
  m_eventManager.PushEvent(a_event);
}

void DgApp::KeyEvent(int a_key, int a_action)
{
  switch (a_key)
  {
  case GLFW_KEY_ESCAPE:
  {
    m_shouldQuit = true;
    break;
  }
  default:
  {
    _KeyEvent(a_key, a_action);
  }
  }
}

void DgApp::UpdateScroll(double a_val)
{
  _UpdateScroll(a_val);
}

void AppOnKeyEvent(int a_key, int a_action)
{
  Event_KeyEvent e;
  e.SetKey(a_key);
  e.SetAction(a_action);
  DgApp::GetInstance()->PushEvent(e);
}

void AppOnMouseScroll(double yOffset)
{
  Event_MouseScroll e;
  e.SetOffset(yOffset);
  DgApp::GetInstance()->PushEvent(e);
}

void DgApp::HandleEvents()
{
  eObject e(nullptr);
  while (m_eventManager.PollEvent(e))
  {
    e->DoEvent();
  }

  _HandleEvents();
}

void DgApp::DoLogic(double a_dt)
{
  
  _DoLogic(a_dt);
}

void DgApp::Render()
{
  _Render();
}

bool DgApp::FileExists(std::string const & a_name) const
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

std::vector<std::string> DgApp::GetFiles(std::string const & a_dirPath,
                                         std::string a_ext) const
{
  std::vector<std::string> result;
  WIN32_FIND_DATA ffd;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  std::string szDir = a_dirPath + "*";

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
      if (ext == a_ext || a_ext == "*")
      {
        result.push_back(ffd.cFileName);
      }
    }
  } while (FindNextFile(hFind, &ffd) != 0);

  FindClose(hFind);
  return result;
}

void DgApp::GetConfiguration()
{
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
    if (parser.GetItems().query_key(i) == "windowWidth")
    {
      int val = 0;
      if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
      {
        m_info.windowWidth = val;
      }
    }
    else if (parser.GetItems().query_key(i) == "windowHeight")
    {
      int val = 0;
      if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
      {
        m_info.windowHeight = val;
      }
    }
    else if (parser.GetItems().query_key(i) == "glMajorVersion")
    {
      int val = 0;
      if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
      {
        m_info.majorVersion = val;
      }
    }
    else if (parser.GetItems().query_key(i) == "glMinorVersion")
    {
      int val = 0;
      if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
      {
        m_info.minorVersion = val;
      }
    }
    else if (parser.GetItems().query_key(i) == "glSamples")
    {
      int val = 0;
      if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
      {
        m_info.samples = val;
      }
    }
    else if (parser.GetItems().query_key(i) == "fullscreen")
    {
      int val = 0;
      if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
      {
        m_info.fullscreen = (val != 0);
      }
    }
  }
}


void DgApp::Run(DgApp* the_app)
{
  s_app = the_app;

  if (!Init())
  {
    fprintf(stderr, "Failed to initialize DgApp\n");
    return;
  }

  double lastTick = glfwGetTime();

  do
  {
    double thisTick = glfwGetTime();
    double dt = static_cast<float>(thisTick - lastTick);
    lastTick = thisTick;

    glfwPollEvents();
    ImGui_ImplGlfwGL3_NewFrame();
    _BuildUI();

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
