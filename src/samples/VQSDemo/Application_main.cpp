﻿
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "Application.h"
#include "UI.h"
#include "DgParser_INI.h"
#include "DgStringFunctions.h"
#include "dgvqs.h"

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
  m_camRotX = Dg::PI * 0.5;
	m_camZoom = 5.0;
	m_camZoomTarget = 5.0;
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
	  Dg::ClampNumber(0.001, Dg::PI_d - 0.001, m_camRotX);
  }
}

void Application::DoLogic(double a_dt)
{
  m_model.Update(a_dt);

  //Zoom the camera
  if (abs(m_camZoomTarget - m_camZoom) > 0.01)
  {
    double diff = m_camZoomTarget - m_camZoom;
    m_camZoom = m_camZoomTarget - diff / (pow(1.3f, 26.0f * a_dt));
  }
}

void Application::GetRenderTransforms(Dg::Matrix44<float> & a_mv
                                    , Dg::Matrix44<float> & a_proj)
{
  
}

//void Application::Render()
//{
//  float camHeight = 2.0f;
//  float fov = 1.5f;
//  float nearClip = 0.1f;
//  float farClip = 1000.0f;
//
//  static float r1[3] = {0.0f, 0.0f, 0.0f};
//  static float v1[3] = {1.0f, 0.0f, 0.0f};
//  static float s0 = 1.0f, s1 = 1.0f;
//  ImGui::Begin("Editor");
//  //ImGui::SliderFloat3("RH", r0, 0.0f, 2 * Dg::PI_f, "%.3f");
//  ImGui::SliderFloat3("RF", r1, 0.0f, 2 * Dg::PI_f, "%.3f");
//  //ImGui::SliderFloat3("VH", v0, -1.0f, 1.0f, "%.2f");
//  ImGui::SliderFloat3("VF", v1, -2.0f, 2.0f, "%.2f");
//  ImGui::SliderFloat("SH", &s0, 0.1f, 2.0f, "%0.2f");
//  ImGui::SliderFloat("SF", &s1, 0.1f, 2.0f, "%0.2f");
//  if (ImGui::Button("Reset"))
//  {
//    r1[0] = r1[1] = r1[2] = 0.0f;
//    v1[0] = 1.0f;
//    v1[1] = v1[2] = 0.0f;
//    s0 = s1 = 1.0f;
//    m_camRotX = m_camRotZ = 0.0f;
//  }
//  ImGui::End();
//
//  Dg::VQS<float> TVW, TW0, T01;
//  Dg::Quaternion<float> q;
//  q.SetRotation(r1[0], r1[1], r1[2], Dg::EulerOrder::XYZ);
//  T01.Set(Dg::Vector4<float>(v1[0], v1[1], v1[2], 0.0f), q, s1);
//
//  q.SetRotation(float(m_camRotX)
//    , 0.0f
//    , float(m_camRotZ)
//    , Dg::EulerOrder::XYZ);
//  TW0.SetQ(q);
//  TW0.SetV(Dg::Vector4<float>(0.0f, -camHeight, float(-m_camZoom), 0.0f));
//  TW0.SetS(s0);
//
//  Dg::Matrix44<float> mats[ArmSkeleton::BONE_COUNT];
//  TW0.GetMatrix(mats[0]);
//  (T01 * TW0).GetMatrix(mats[1]);
//  
//  //Set up the viewport
//  float ratio;
//  int width, height;
//
//  glfwGetFramebufferSize(m_window, &width, &height);
//  ratio = width / (float)height;
//
//  glViewport(0, 0, width, height);
//
//  //Set up the perspective matrix;
//  Dg::Matrix44<float> proj;
//  proj.Perspective(fov, ratio, nearClip, farClip);
//
//  m_renderer.Render(proj, mats, ArmSkeleton::BONE_COUNT);
//}

void Application::Render()
{
  float camHeight = 0.0f;
  float fov = 1.5f;
  float nearClip = 0.5f;
  float farClip = 10.0f;

  Dg::VQS<float> T;
  Dg::Quaternion<float> q;
  q.SetRotation(float(m_camRotX + 3.14159f)
    , 0.0f
    , float(m_camRotZ)
    , Dg::EulerOrder::ZYX);
  T.SetQ(q);
  T.SetV(Dg::Vector4<float>(0.0f, -camHeight, float(-m_camZoom), 0.0f));
  T.SetS(2.0f);

  m_model.SetTransform(T);
  Dg::Matrix44<float> mats[ArmSkeleton::BONE_COUNT];
  m_model.SetMatrices(mats);
  
  //Set up the viewport
  float ratio;
  int width, height;

  glfwGetFramebufferSize(m_window, &width, &height);
  ratio = width / (float)height;

  glViewport(0, 0, width, height);

  //Set up the perspective matrix;
  Dg::Matrix44<float> proj;
  proj.Perspective(fov, ratio, nearClip, farClip);

  m_renderer.Render(proj, mats, ArmSkeleton::BONE_COUNT);
}

void Application::GetConfiguration()
{
  //Set defaults
  strcpy_s(m_info.title, "Dg Paricle System Example");
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
    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_MenuBar);
    ImGui::End();
  }

  //FPS
  if (UI::showMetrics)
  {
    ImGui::Begin("Stats", nullptr);
    ImGui::Text("FPS %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
  }

  //TODO Get rid of all ImGui unwanted code, demos, etc...
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
