/*!
* @file Application.cpp
*
* @author Frank Hart
* @date 30/01/2014
*
* [description]
*/



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Application.h"
#include <string>
#include <cstring>
#include "DgMatrix44.h"
#include <math.h>

#include "DgParser_INI.h"
#include "DgStringFunctions.h"

#include "imgui/imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "UI.h"

Application* Application::s_app(nullptr);

typedef Dg::Matrix44<float> mat44;
typedef Dg::Vector4<float>  vec4;

double Application::s_dZoom;

bool Application::Init()
{
  GetConfiguration();

  if (!InitGL())
  {
    return false;
  }

  m_camRotZ = 0.0;
  m_camRotX = Dg::PI * 0.5;
  m_camZoom = 7.0;

  InitControls();
  InitParticleSystem();
  m_renderer.Init(m_particleSystem.GetParticleData());
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


void Application::InitControls()
{
  GLFWscrollfun res = glfwSetScrollCallback(m_window, OnMouseScroll);

  s_dZoom = 0.0;
  m_mouseSpeed = 0.01;
  m_camZoom = 5.0;
  m_canRotate = false;
  glfwGetCursorPos(m_window, &m_mouseCurentX, &m_mouseCurentX);
  m_mousePrevX = m_mouseCurentX;
  m_mousePrevY = m_mouseCurentY;
}


void Application::Shutdown()
{
  m_renderer.ShutDown();
}


void Application::HandleInput()
{
  //Mouse
  if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    m_canRotate = true;
  }
  else if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
  {
    m_canRotate = false;
  }

  m_mousePrevX = m_mouseCurentX;
  m_mousePrevY = m_mouseCurentY; 
  glfwGetCursorPos(m_window, &m_mouseCurentX, &m_mouseCurentY);

}

void Application::DoLogic()
{
  {
    ImVec4 headingClr(1.0f, 0.0f, 1.0f, 1.0f);

    ImGui::Begin("Partice System");
    
    ImGui::BeginChild("Emitter attributes", ImVec2(ImGui::GetWindowWidth() - 40.0f, 700.0f), true);

    static int curEm = 0;
    ImGui::RadioButton("Emitter 1", &curEm, 0); ImGui::SameLine();
    ImGui::RadioButton("Emitter 2", &curEm, 1); ImGui::SameLine();
    ImGui::RadioButton("Emitter 3", &curEm, 2);
    
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    ImGui::Checkbox("Turn emitter off/on", &m_eData[curEm].on);

    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    ImGui::TextColored(headingClr, "Emission method");
    ImGui::RadioButton("Linear", &m_eData[curEm].emitterType, 0); ImGui::SameLine();
    ImGui::RadioButton("Poisson", &m_eData[curEm].emitterType, 1);

    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    ImGui::TextColored(headingClr, "Define emitter shape");
    ImGui::RadioButton("Point", &m_eData[curEm].posGenMethod, E_GenPosPoint); ImGui::SameLine();
    ImGui::RadioButton("Box", &m_eData[curEm].posGenMethod, E_GenPosBox); ImGui::SameLine();
    ImGui::RadioButton("Sphere", &m_eData[curEm].posGenMethod, E_GenPosSphere);

    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.27f);
    if (m_eData[curEm].posGenMethod == E_GenPosBox)
    {
      ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
      ImGui::TextColored(headingClr, "Define box geometry");
      ImGui::SliderFloat("L", &m_eData[curEm].boxDim[0], 0.0f, 10.0f, "%.1f"); ImGui::SameLine();
      ImGui::SliderFloat("W", &m_eData[curEm].boxDim[1], 0.0f, 10.0f, "%.1f"); ImGui::SameLine();
      ImGui::SliderFloat("H", &m_eData[curEm].boxDim[2], 0.0f, 10.0f, "%.1f");
      ImGui::SliderFloat("rx", &m_eData[curEm].rot[0], 0.0f, Dg::PI_f, "%.2f"); ImGui::SameLine();
      ImGui::SliderFloat("rz", &m_eData[curEm].rot[1], 0.0f, Dg::PI_f * 2.0f, "%.2f");
    }

    if (m_eData[curEm].posGenMethod == E_GenPosSphere)
    {
      ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
      ImGui::TextColored(headingClr, "Define sphere geometry");
      ImGui::SliderFloat("Radius", &m_eData[curEm].sphereRadius, 0.0f, 10.0f, "%.1f");
    }

    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    ImGui::TextColored(headingClr, "Place Emitter");
    ImGui::SliderFloat("X", &m_eData[curEm].pos[0], -5.0f, 5.0f, "%.1f"); ImGui::SameLine();
    ImGui::SliderFloat("Y", &m_eData[curEm].pos[1], -5.0f, 5.0f, "%.1f"); ImGui::SameLine();
    ImGui::SliderFloat("Z", &m_eData[curEm].pos[2], -5.0f, 5.0f, "%.1f");
    
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    ImGui::TextColored(headingClr, "Define Initial velocity vector");
    ImGui::Checkbox("Repel from Center", &m_eData[curEm].repelFromCenter);
    if (!m_eData[curEm].repelFromCenter)
    {
      ImGui::SliderFloat("rz", &m_eData[curEm].velRot[0], 0.0f, Dg::PI_f * 2.0f, "%.2f"); ImGui::SameLine();
      ImGui::SliderFloat("rx", &m_eData[curEm].velRot[1], 0.0f, Dg::PI_f, "%.2f");
      ImGui::SliderFloat("Spread", &m_eData[curEm].spread, 0.0f, Dg::PI_f, "%.2f");
    }

    ImGui::PopItemWidth();

    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    ImGui::TextColored(headingClr, "Other attributes");
    ImGui::ColorEdit4("Start color", m_eData[curEm].startColor);
    ImGui::ColorEdit4("End color", m_eData[curEm].endColor);
    ImGui::SliderFloat("Rate (par/s)", &m_eData[curEm].rate, 0.0f, 200.0f, "%.2f");
    ImGui::SliderFloat("Velocity (m/s)", &m_eData[curEm].velocity, 0.0f, 10.0f, "%.2f");
    ImGui::SliderFloat("Life (s)", &m_eData[curEm].life, 0.0f, 60.0f, "%.2f");
    ImGui::SliderFloat("Force (m/s/s)", &m_eData[curEm].force, -100.0f, 100.0f, "%.2f");
    ImGui::SliderFloat("Start size (m)", &m_eData[curEm].startSize, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("End size (m)", &m_eData[curEm].endSize, 0.0f, 1.0f, "%.2f");

    ImGui::EndChild();
    ImGui::End();
  }

  UpdateParSysAttr();
  m_particleSystem.Update(m_dt);

  //Camera
  if (m_canRotate)
  {
    m_camRotZ += (m_mouseCurentX - m_mousePrevX) * m_mouseSpeed;
    Dg::WrapAngle(m_camRotZ);

    m_camRotX += (m_mouseCurentY - m_mousePrevY) * m_mouseSpeed;
    Dg::ClampNumber(0.001, Dg::PI_d - 0.001, m_camRotX);

    m_camZoom += s_dZoom;
    Dg::ClampNumber(1.0, 20.0, m_camZoom);
    s_dZoom = 0.0;
  }
}

void Application::Render()
{
  mat44 translate, rotate, scale;
  translate.Translation(vec4(0.0f, -2.0f, float(-m_camZoom), 0.0f));
  rotate.Rotation(float(Dg::PI_d + m_camRotX)
                , 0.0f
                , float(m_camRotZ)
                , Dg::EulerOrder::ZYX);
  mat44 mv_matrix = rotate * translate;

  //Set up the viewport
  float ratio;
  int width, height;

  glfwGetFramebufferSize(m_window, &width, &height);
  ratio = width / (float)height;

  glViewport(0, 0, width, height);

  //Set up the perspective matrix;
  mat44 proj_matrix;
  float fov = 1.5f;
  proj_matrix.Perspective(fov, ratio, 0.1f, 1000.0f);

  float parScale = (static_cast<float>(width) * 0.5f) / std::tan(fov * 0.5f);

  m_renderer.Update(m_particleSystem.GetParticleData());
  m_renderer.Render(mv_matrix, proj_matrix, parScale);
}


void Application::OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset)
{
  s_dZoom += xOffset;
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
    fprintf(stderr, "Failed to open config file '%s'. Using defaults...\n", m_configFileName);
    return;
  }
  else if (result != Dg::ErrorCode::None)
  {
    fprintf(stderr, "Failed trying to parse config file '%s'. Using defaults...\n", m_configFileName);
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

void Application::Run(Application* the_app)
{
  bool running = true;
  s_app = the_app;

  if (!Init())
  {
    fprintf(stderr, "Failed to initialize Application\n");
    return;
  }

  // Setup ImGui binding
  bool show_test_window = false;

  double lastTick = glfwGetTime();
  m_particleSystem.StartAllEmitters();


  do
  {

	glfwPollEvents();
	ImGui_ImplGlfwGL3_NewFrame();

	{
    ImGui::Begin("Partice System");
    if (ImGui::Button("Blending")) UI::showAlphaBlendingWindow ^= 1; ImGui::SameLine();
    if (ImGui::Button("Example UI")) show_test_window ^= 1;
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::End();
  }

  {
    ImGui::Begin("Stats");
    ImGui::Text("FPS %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
  }

  double thisTick = glfwGetTime();
	double diff = thisTick - lastTick;
  m_dt = static_cast<float>(thisTick - lastTick);
  lastTick = thisTick;

  HandleInput();
  DoLogic();
  Render();

  // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
  if (show_test_window)
  {
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
    ImGui::ShowTestWindow(&show_test_window);
  }

	ImGui::Render();
  glfwSwapBuffers(m_window);

  } while (!glfwWindowShouldClose(m_window));

  Shutdown();

  ImGui_ImplGlfwGL3_Shutdown();
  glfwDestroyWindow(m_window);
  glfwTerminate();
}
