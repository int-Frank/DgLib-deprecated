
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "Application.h"
#include "UI.h"
#include "DgParser_INI.h"
#include "DgStringFunctions.h"
#include "dgR3vqs.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

Application* Application::s_app(nullptr);

Application::Application() 
  : m_window(nullptr)
  , m_shouldQuit(false)
{
  s_app = this;
}

bool Application::Init()
{
  GetConfiguration();

  //Set options
  m_shouldQuit = false;
  m_camCanRotate = false;
  m_maxCamZoomOut = 50.0;

  if (!InitGL())
  {
    return false;
  }

  ResetCamera();
  m_renderer.Init();
  ImGui_ImplGlfwGL3_Init(m_window, true);
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

void Application::DoLogic(double a_dt)
{
  m_model.Update(float(a_dt));

  //Zoom the camera
  if (abs(m_camZoomTarget - m_camZoom) > 0.01)
  {
    double diff = m_camZoomTarget - m_camZoom;
    m_camZoom = m_camZoomTarget - diff / (pow(1.3f, 26.0f * a_dt));
  }
}


void Application::Render()
{
  float camHeight = 0.0f;
  float fov = 1.5f;
  float nearClip = 0.5f;
  float farClip = 100.0f;

  Dg::R3::VQS<float> T;
  Dg::R3::Quaternion<float> q;
  q.SetRotation(float(m_camRotX + 3.14159f)
    , 0.0f
    , float(m_camRotZ)
    , Dg::EulerOrder::ZYX);
  T.SetQ(q);
  T.SetV(Dg::R3::Vector<float>(0.0f, -camHeight, float(-m_camZoom), 0.0f));
  T.SetS(2.0f);

  m_model.SetTransform(T);
  Dg::R3::Matrix<float> mats[ArmSkeleton::BONE_COUNT];
  m_model.SetMatrices(mats);
  
  //Set up the viewport
  float ratio;
  int width, height;

  glfwGetFramebufferSize(m_window, &width, &height);
  ratio = width / (float)height;

  glViewport(0, 0, width, height);

  //Set up the perspective matrix;
  Dg::R3::Matrix<float> proj;
  proj.Perspective(fov, ratio, nearClip, farClip);

  m_renderer.Render(proj, mats, ArmSkeleton::BONE_COUNT);
}

void Application::GetConfiguration()
{
  //Set defaults
  strcpy_s(m_info.title, "VQS (vector, quaternion, scalar) Demo");
  m_info.windowWidth = 800;
  m_info.windowHeight = 600;
  m_info.majorVersion = 4;
  m_info.minorVersion = 3;
  m_info.samples = 0;
  m_info.flags.all = 0;
  m_info.flags.fullscreen = 0;

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

void Application::UI_NewFrame()
{
  glfwPollEvents();
  ImGui_ImplGlfwGL3_NewFrame();

  int winWidth(0), winHeight(0);
  glfwGetWindowSize(m_window, &winWidth, &winHeight);

  //Editor
  {
    ImGui::Begin("Main", nullptr, ImGuiWindowFlags_MenuBar);
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
