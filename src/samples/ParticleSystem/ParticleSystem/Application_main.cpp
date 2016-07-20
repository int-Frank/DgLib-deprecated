/*!
* @file Application.cpp
*
* @author Frank Hart
* @date 30/01/2014
*
* [description]
*/


#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "Application.h"
#include "UI.h"
#include "DgParser_INI.h"
#include "DgStringFunctions.h"
#include "Events.h"

#include "imgui/imgui.h"
#include "imgui_impl_glfw_gl3.h"

Application* Application::s_app(nullptr);

typedef Dg::Matrix44<float> mat44;
typedef Dg::Vector4<float>  vec4;

Application::Application() 
  : m_window(nullptr)
  , m_particleSystem(65536)
{
  m_IDManager.Init(E_UpdaterGeneric_begin, E_UpdaterGeneric_end);
  s_app = this;
}

void Application::PushEvent(Event const & a_event)
{
  m_eventManager.PushEvent(a_event);
}

void AppOnKeyEvent(int a_key, int a_action)
{
  Event_KeyEvent e;
  e.SetKey(a_key);
  e.SetAction(a_action);
  Application::GetInstance()->PushEvent(e);
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

bool Application::Init()
{
  GetConfiguration();

  //Set options
  m_attrFocus = -1;
  m_shouldQuit = false;
  m_camCanRotate = false;

  if (!InitGL())
  {
    return false;
  }

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

bool Application::LoadProject(std::string a_file)
{
  printf("%s loaded!\n", a_file.c_str());
  return true;
}

bool Application::SaveProject(std::string a_file)
{
  printf("%s saved!\n", a_file.c_str());
  return true;
}

void Application::UpdateScroll(double a_val)
{
  m_camZoomTarget -= a_val;
  Dg::ClampNumber(1.0, 20.0, m_camZoomTarget);
}

void Application::HandleEvents()
{
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

std::vector<std::string> Application::GetProjects()
{
  std::vector<std::string> result;
  WIN32_FIND_DATA ffd;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  std::string szDir = std::string(m_projectPath) + "*";

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
      if (ext == std::string(m_fileExt))
      {
        result.push_back(ffd.cFileName);
      }
    }
  } while (FindNextFile(hFind, &ffd) != 0);

  FindClose(hFind);
  return result;
}


void Application::DoLogic(double a_dt)
{
  //Get user input
  ShowMainGUIWindow();

  //Update particle system
  UpdateParSysAttr();
  m_particleSystem.Update((float)a_dt);

  //Update stats
  if (UI::showMetrics)
  {
    ImGui::Begin("Stats");
    ImGui::Text("Live Particles: %i", m_particleSystem.GetParticleData()->GetCountAlive());
    ImGui::End();
  }

  //Zoom the camera
  if (abs(m_camZoomTarget - m_camZoom) > 0.01)
  {
    double diff = m_camZoomTarget - m_camZoom;
    m_camZoom = m_camZoomTarget - diff / (pow(1.3f, 26.0f * a_dt));
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

  LineRenderData * lineRenderData = new LineRenderData[m_appData.aData.size()];
  int nShowAttr = 0;
  for (int i = 0; i < m_appData.aData.size(); ++i)
  {
    if (m_appData.aData[i].first.show)
    {
      lineRenderData[nShowAttr].model = m_appData.aData[i].first.type;
      Dg::Vector4<float> lineCol(1.0f, 0.4588f, 0.102f, 1.0f);
      Dg::Vector4<float> lineColFocus(51.f / 255.f, 204.f / 255.f, 51.f / 255.f, 1.0f);
      lineRenderData[nShowAttr].col = (i == m_attrFocus) ? lineColFocus : lineCol;
      switch (m_appData.aData[i].first.type)
      {
      case E_AttGlobal:
      {
        lineRenderData[nShowAttr].mat.Rotation(0.0f
                                 , m_appData.aData[i].first.transform[4] - Dg::PI_f / 2.0f
                                 , m_appData.aData[i].first.transform[3]
                                 , Dg::EulerOrder::XYZ);
        break;
      }
      case E_AttPoint:
      {
        lineRenderData[nShowAttr].mat.Translation
        (
          Dg::Vector4<float>(m_appData.aData[i].first.transform[0]
                           , m_appData.aData[i].first.transform[1]
                           , m_appData.aData[i].first.transform[2]
                           , 0.0f)
        );
        break;
      }
      case E_AttLine:
      {
        mat44 trans, rot;
        trans.Translation
        (
          Dg::Vector4<float>(m_appData.aData[i].first.transform[0]
                           , m_appData.aData[i].first.transform[1]
                           , m_appData.aData[i].first.transform[2]
                           , 0.0f)
        );

        rot.Rotation(0.0f
                   , m_appData.aData[i].first.transform[4] - Dg::PI_f / 2.0f
                   , m_appData.aData[i].first.transform[3]
                   , Dg::EulerOrder::XYZ);
        lineRenderData[nShowAttr].mat = rot * trans;
        break;
      }
      case E_AttPlane:
      {
        mat44 trans, rot;
        trans.Translation
        (
          Dg::Vector4<float>(m_appData.aData[i].first.transform[0]
                           , m_appData.aData[i].first.transform[1]
                           , m_appData.aData[i].first.transform[2]
                           , 0.0f)
        );

        rot.Rotation(0.0f
                   , m_appData.aData[i].first.transform[4] - Dg::PI_f / 2.0f
                   , m_appData.aData[i].first.transform[3]
                   , Dg::EulerOrder::XYZ);
        lineRenderData[nShowAttr].mat = rot * trans;
        break;
      }
      }
      
      nShowAttr++;
    }
  }
  
  m_renderer.Update(m_particleSystem.GetParticleData());
  m_renderer.Render(mv_matrix, proj_matrix, parScale, nShowAttr, lineRenderData);

  delete[] lineRenderData;
}

void AppOnMouseScroll(double yOffset)
{
  Event_MouseScroll e;
  e.SetOffset(yOffset);
  Application::GetInstance()->PushEvent(e);
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
  s_app = the_app;

  if (!Init())
  {
    fprintf(stderr, "Failed to initialize Application\n");
    return;
  }

  double lastTick = glfwGetTime();

  do
  {
	  glfwPollEvents();
	  ImGui_ImplGlfwGL3_NewFrame();

    //Main window
	  {
      ImGui::Begin("Partice System", nullptr, ImGuiWindowFlags_MenuBar);
      ImGui::End();
    }

    //FPS
    if (UI::showMetrics)
    {
      ImGui::Begin("Stats");
      ImGui::Text("FPS %.1f", ImGui::GetIO().Framerate);
      ImGui::End();
    }

    //Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
    if (UI::showExampleWindow)
    {
      ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
      ImGui::ShowTestWindow(&UI::showExampleWindow);
    }

    double thisTick = glfwGetTime();
	  double diff = thisTick - lastTick;
    double dt = static_cast<float>(thisTick - lastTick);
    lastTick = thisTick;

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
