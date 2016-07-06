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
    static float windowW = 360.0f;
    static float windowH = 600.0f;

    ImGui::Begin("Partice System");
    ImGui::SetWindowSize(ImVec2(windowW, windowH), 0);

    static int emitter = 0;
    ImGui::RadioButton("Emitter 1", &emitter, 0); ImGui::SameLine();
    ImGui::RadioButton("Emitter 2", &emitter, 1); ImGui::SameLine();
    ImGui::RadioButton("Emitter 3", &emitter, 2);

    static int emitterType;
    ImGui::RadioButton("Linear", &emitterType, 0); ImGui::SameLine();
    ImGui::RadioButton("Poisson", &emitterType, 1);

    ImGui::BeginChild("Emitter attributes", ImVec2(ImGui::GetWindowWidth() - 20.0f, 200.0f), true);
    ImGui::Text("Position");
    static float posX = 0.0;
    static float posY = 0.0;
    static float posZ = 0.0;
    ImGui::PushItemWidth(int(ImGui::GetWindowWidth() * 0.27));
    ImGui::SliderFloat("X", &posX, -5.0f, 5.0f, "%.1f"); ImGui::SameLine();
    ImGui::SliderFloat("Y", &posY, -5.0f, 5.0f, "%.1f"); ImGui::SameLine();
    ImGui::SliderFloat("Z", &posZ, -5.0f, 5.0f, "%.1f");
    ImGui::PopItemWidth();
    ImGui::EndChild();
    /*
    Emitter selector:   3 x radio edit
      Emitter Type:       2 x radio edit (linear, poisson)
        Position Generator: radio edit for each generator
          Cone:
            Position:           3 x Slider (x, y, z)
            Orientation:        2 x slider (rz, rx)
            Spread:             1 x slider
        Rate:               1 x slider
        Life:               1 x slider
        Start velocity:     1 x slider
        Force:              1 x slider
        Start Color:        1 x color edit
        End Color:          1 x color edit
        Start point size:   1 x slider
        End point size:     1 x slider
    */
    //static float f = 0.0f;
    //ImGui::Text("Hello, world!");
    //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    //ImGui::ColorEdit3("clear color", (float*)&clear_color);
    //if (ImGui::Button("Another Window")) show_another_window ^= 1;
    ImGui::End();
  }

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
  bool show_another_window = false;
  ImVec4 clear_color = ImColor(114, 144, 154);

  double lastTick = glfwGetTime();
  m_particleSystem.StartAllEmitters();

  do
  {

	glfwPollEvents();
	ImGui_ImplGlfwGL3_NewFrame();

	{
    ImGui::Begin("Partice System");
		//static float f = 0.0f;
		//ImGui::Text("Hello, world!");
		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		//ImGui::ColorEdit3("clear color", (float*)&clear_color);
		if (ImGui::Button("Test Window")) show_test_window ^= 1;
		//if (ImGui::Button("Another Window")) show_another_window ^= 1;
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
