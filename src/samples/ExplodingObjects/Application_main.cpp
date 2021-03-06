﻿
#include <windows.h>
#include <shlwapi.h>
#include <time.h>
#pragma comment(lib,"shlwapi.lib")

#include "Application.h"
#include "ExplosionTypes.h"
#include "UI.h"
#include "DgParser_INI.h"
#include "DgStringFunctions.h"
#include "DgR3Vector_ancillary.h"
#include "dgrng.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

Application* Application::s_app(nullptr);

Application::Application() 
  : m_window(nullptr)
  , m_shouldQuit(false)
  , m_ptimeManager(new TimeManager())
{
  s_app = this;
}

bool Application::Init()
{
  GetConfiguration();

  //Set options
  m_camCanRotate = false;
  m_maxCamZoomOut = 50.0;

  if (!InitGL())
  {
    return false;
  }

  Dg::RNG::SetSeed(unsigned int(time(0)));

  ResetCamera();
  m_renderer.Init();
  ImGui_ImplGlfwGL3_Init(m_window, true);
  ClearProject();

  LoadProject("./objects/teapot.obj");

  return true;
}


bool Application::InitGL()
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

  if (m_info.flags.fullscreen)
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
                                glfwSwapInterval((int)m_info.flags.vsync);
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

void Application::ResetCamera()
{
  m_mouseSpeed = 0.01;
  m_camRotZ = 0.0;
  m_camRotX = Dg::Constants<double>::PI * 0.5;
  m_camZoom = 7.0;
  m_camZoomTarget = 7.0;
  glfwGetCursorPos(m_window, &m_mouseCurrentX, &m_mouseCurrentX);
  m_mousePrevX = m_mouseCurrentX;
  m_mousePrevY = m_mouseCurrentY;
}

void Application::Shutdown()
{
  m_renderer.ShutDown();
}

void Application::ClearProject()
{
  m_centroids.clear();
  m_appData.projName.clear();
  m_renderer.Clear();
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
  case GLFW_MOUSE_BUTTON_LEFT:
  {
    m_camCanRotate = (a_action == GLFW_PRESS);
    break;
  }
  }
}

void Application::UpdateScroll(double a_val)
{
  if (!ImGui::GetIO().WantCaptureMouse)
  {
    m_camZoomTarget -= a_val;
    Dg::ClampNumber(1.0, m_maxCamZoomOut, m_camZoomTarget);
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

  m_mousePrevX = m_mouseCurrentX;
  m_mousePrevY = m_mouseCurrentY;
  glfwGetCursorPos(m_window, &m_mouseCurrentX, &m_mouseCurrentY);

  //Mouse
  if (!ImGui::GetIO().WantCaptureMouse
    && !ImGui::GetIO().WantCaptureKeyboard
    && m_camCanRotate)
  {
    //Handle mouse rotation
    m_camRotZ += (m_mouseCurrentX - m_mousePrevX) * m_mouseSpeed;
    Dg::WrapAngle(m_camRotZ);
    m_camRotX += (m_mouseCurrentY - m_mousePrevY) * m_mouseSpeed;
    Dg::ClampNumber(0.001, Dg::Constants<double>::PI - 0.001, m_camRotX);
  }
}

void Application::Explode()
{
  std::vector<TransformData> td_vec;
  vec4 source(vec4::Origin());

  ExplosionTypeBase * pGen(nullptr);

  switch (m_appData.explodeType)
  {
  case 1:
  {
    pGen = new ExplosionType_Chunked();
    break;
  }
  default:
  {
    pGen = new ExplosionType_Complete();
    break;
  }
  }

  pGen->Init(source);

  for (auto const & c : m_centroids)
  {
    td_vec.push_back(pGen->GetData(c));
  }

  m_renderer.SetTransformData(td_vec);
  delete pGen;
}

void Application::DoLogic(double a_dt)
{
  //Zoom the camera
  if (abs(m_camZoomTarget - m_camZoom) > 0.01)
  {
    double diff = m_camZoomTarget - m_camZoom;
    m_camZoom = m_camZoomTarget - diff / (pow(1.3f, 26.0f * a_dt));
  }

  if (m_appData.explode)
  {
    delete m_ptimeManager;
    m_ptimeManager = new TimeAccumulator();
    m_appData.explode = false;

    Explode();
  }
  if (m_appData.reset)
  {
    delete m_ptimeManager;
    m_ptimeManager = new TimeManager();
    m_appData.reset = false;
  }

  m_renderer.SetTime(m_ptimeManager->GetTime(float(a_dt)));
}

void Application::Render()
{
  float camHeight = 0.0f;
  float fov = 1.5f;
  float nearClip = 0.5f;
  float farClip = 100.0f;

  mat44 mt, mr;
  mt.Translation(vec4(0.0f, -camHeight, float(-m_camZoom), 0.0f));
  mr.Rotation(float(Dg::Constants<double>::PI + m_camRotX),
                  0.0f,
                  float(m_camRotZ),
                  Dg::EulerOrder::ZYX);
  mat44 T_V_W = mr * mt;

  //Set up the viewport
  float ratio;
  int width, height;

  glfwGetFramebufferSize(m_window, &width, &height);
  ratio = width / (float)height;

  glViewport(0, 0, width, height);

  //Set up the perspective matrix;
  mat44 T_S_V;
  T_S_V.Perspective(fov, ratio, nearClip, farClip);

  mat44 T_W_M;
  T_W_M.Identity();

  m_renderer.Begin();
  m_renderer.Render(T_S_V, T_V_W, T_W_M);
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
  strcpy_s(m_info.title, "Exploding Teapot Example");
  m_info.windowWidth = 800;
  m_info.windowHeight = 600;
  m_info.majorVersion = 4;
  m_info.minorVersion = 3;
  m_info.samples = 0;
  m_info.flags.all = 0;
  m_info.flags.fullscreen = 0;

  Dg::Parser_INI parser;
  Dg::ErrorCode result = parser.Parse(m_configFileName);

  if (result == Dg::Err_FailedToOpenFile)
  {
    fprintf(stderr, "Failed to open config file '%s'. Using defaults...\n", m_configFileName.c_str());
    return;
  }
  else if (result != Dg::Err_None)
  {
    fprintf(stderr, "Failed trying to parse config file '%s'. Using defaults...\n", m_configFileName.c_str());
    return;
  }

  for (auto kv : parser.GetItems())
  {
    if (kv.first == "windowWidth")
    {
      int val = 0;
      if (Dg::StringToNumber(val, kv.second, std::dec))
      {
        m_info.windowWidth = val;
      }
    }
    else if (kv.first == "windowHeight")
    {
      int val = 0;
      if (Dg::StringToNumber(val, kv.second, std::dec))
      {
        m_info.windowHeight = val;
      }
    }
    else if (kv.first == "glMajorVersion")
    {
      int val = 0;
      if (Dg::StringToNumber(val, kv.second, std::dec))
      {
        m_info.majorVersion = val;
      }
    }
    else if (kv.first == "glMinorVersion")
    {
      int val = 0;
      if (Dg::StringToNumber(val, kv.second, std::dec))
      {
        m_info.minorVersion = val;
      }
    }
    else if (kv.first == "glSamples")
    {
      int val = 0;
      if (Dg::StringToNumber(val, kv.second, std::dec))
      {
        m_info.samples = val;
      }
    }
    else if (kv.first == "fullscreen")
    {
      int val = 0;
      if (Dg::StringToNumber(val, kv.second, std::dec))
      {
        m_info.flags.fullscreen = val;
      }
    }
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
