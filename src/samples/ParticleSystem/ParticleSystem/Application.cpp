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
#include <cmath>

#include "DgParser_INI.h"
#include "DgStringFunctions.h"

#include "imgui/imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "UI.h"

Application* Application::s_app(nullptr);

typedef Dg::Matrix44<float> mat44;
typedef Dg::Vector4<float>  vec4;

double Application::s_scrollOffset;

Application::Application() 
  : m_window(nullptr)
  , m_particleSystem(65536)
{
  m_IDManager.Init(E_UpdaterGeneric_begin, E_UpdaterGeneric_end);
}

bool Application::Init()
{
  GetConfiguration();

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
  s_scrollOffset = 0.0;
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


void Application::HandleInput()
{
  m_mousePrevX = m_mouseCurrentX;
  m_mousePrevY = m_mouseCurrentY;
  glfwGetCursorPos(m_window, &m_mouseCurrentX, &m_mouseCurrentY);

  //Mouse
  if (!ImGui::GetIO().WantCaptureMouse && !ImGui::GetIO().WantCaptureKeyboard)
  {
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
      //Handle mouse rotation
      m_camRotZ += (m_mouseCurrentX - m_mousePrevX) * m_mouseSpeed;
      Dg::WrapAngle(m_camRotZ);
      m_camRotX += (m_mouseCurrentY - m_mousePrevY) * m_mouseSpeed;
      Dg::ClampNumber(0.001, Dg::PI_d - 0.001, m_camRotX);
    }

    //Handle mouse zoom
    m_camZoomTarget -= s_scrollOffset;
    Dg::ClampNumber(1.0, 20.0, m_camZoomTarget);
  }

  s_scrollOffset = 0.0;
}

static void CreateSpacing(int a_n)
{
  for (int i = 0; i < a_n; ++i) { ImGui::Spacing(); }
}

void Application::BuildMainUI()
{
  ImVec4 headingClr(1.0f, 0.0f, 1.0f, 1.0f);
  static int curEm = 0;
  float sliderOffset = -90.0f;
  int nSpacing = 3;

  //Common values
  float vRotMins[2] = { 0.0f, 0.0f };
  float vRotMaxs[2] = { Dg::PI_f * 2.0f, Dg::PI_f };
  char const * vRotformats[2] = { "rz = %.2f", "ry = %.2f" };
  float powers2[2] = { 1.0f, 1.0f };
  float posMins[3] = { -10.0f, -10.0f, -10.0f };
  float posMaxs[3] = { 10.0f, 10.0f, 10.0f };
  char const * posFormats[3] = { "x = %.2f", "y = %.2f", "z = %.2f" };
  float powers3[3] = { 1.0f, 1.0f, 1.0f };

  ImGui::Begin("Partice System");
  ImGui::Separator();
  if (ImGui::CollapsingHeader("Emitters"))
  {
    if (s_nEmitters > 1)
    {
      for (int i = 0; i < s_nEmitters; ++i)
      {
        char buf[16];
        sprintf_s(buf, 16, "Emitter %i", i + 1);
        ImGui::RadioButton(buf, &curEm, i);
        if (i != s_nEmitters - 1) ImGui::SameLine();
      }
    }

    EmitterData & curEmData = m_eData[curEm];

    CreateSpacing(nSpacing);
    ImGui::Checkbox("Turn emitter off/on", &curEmData.on);

    CreateSpacing(nSpacing);
    ImGui::TextColored(headingClr, "Emission method");
    ImGui::RadioButton("Linear", &curEmData.type, E_Emitter_Linear); ImGui::SameLine();
    ImGui::RadioButton("Random", &curEmData.type, E_Emitter_Random);

    CreateSpacing(nSpacing);
    ImGui::TextColored(headingClr, "Define emitter shape");
    ImGui::RadioButton("Point", &curEmData.posGenMethod, E_GenPosPoint); ImGui::SameLine();
    ImGui::RadioButton("Box", &curEmData.posGenMethod, E_GenPosBox); ImGui::SameLine();
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

    //ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1 / 7.0f, 0.6f, 0.6f));
    //ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1 / 7.0f, 0.7f, 0.7f));
    //ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1 / 7.0f, 0.8f, 0.8f));
    //ImGui::Button("Drag Me", ImVec2(100.0f, 100.0f));
    //ImGui::PopStyleColor(3);
    //if (ImGui::IsItemActive())
    //{
    //  // Draw a line between the button and the mouse cursor
    //  ImDrawList* draw_list = ImGui::GetWindowDrawList();
    //  draw_list->PushClipRectFullScreen();
    //  draw_list->AddLine(ImGui::CalcItemRectClosestPoint(ImGui::GetIO().MousePos, true, -2.0f), ImGui::GetIO().MousePos, ImColor(ImGui::GetStyle().Colors[ImGuiCol_Button]), 4.0f);
    //  draw_list->PopClipRect();
    //  ImVec2 value_raw = ImGui::GetMouseDragDelta(0, 0.0f);
    //  ImVec2 value_with_lock_threshold = ImGui::GetMouseDragDelta(0);
    //  ImVec2 mouse_delta = ImGui::GetIO().MouseDelta;
    //  ImGui::SameLine(); ImGui::Text("Raw (%.1f, %.1f), WithLockThresold (%.1f, %.1f), MouseDelta (%.1f, %.1f)", value_raw.x, value_raw.y, value_with_lock_threshold.x, value_with_lock_threshold.y, mouse_delta.x, mouse_delta.y);
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
      char const * formats[3] = { "rz = %.2f", "ry = %.2f", "spread = %.2f" };
      float powers[3] = { 1.0f, 1.0f, 1.0f };
      ImGui::PushItemWidth(sliderOffset);
      ImGui::SliderFloatNi("rz, ry, sprd", &curEmData.velCone[0], 3, mins, maxs, formats, powers);
      ImGui::PopItemWidth();
    }

    CreateSpacing(nSpacing);
    ImGui::TextColored(headingClr, "Other attributes");
    ImGui::PushItemWidth(sliderOffset);
    ImGui::ColorEdit4("Start color", curEmData.colors);
    ImGui::ColorEdit4("End color", &curEmData.colors[4]);
    ImGui::SliderFloat("Rate", &curEmData.rate, 0.0f, 500.0f, "%.2f par/s");
    ImGui::SliderFloat("Velocity", &curEmData.velocity, 0.0f, 10.0f, "%.2f m/s");
    ImGui::SliderFloat("Life", &curEmData.life, 0.0f, 60.0f, "%.2f s");
    ImGui::SliderFloat("Start size", curEmData.sizes, 0.0f, 1.0f, "%.2f m");
    ImGui::SliderFloat("End size", &curEmData.sizes[1], 0.0f, 1.0f, "%.2f m");
    
    ImGui::PopItemWidth();
  }
  

  //Attractors
  if (ImGui::CollapsingHeader("Attractors"))
  {
    static int curAtt = 0;
    for (int i = 0; i < s_nAttractors; ++i)
    {
      char buf[16];
      sprintf_s(buf, 16, "Attractor %i", i + 1);
      ImGui::RadioButton(buf, &curAtt, i);
      if (i != s_nAttractors - 1) ImGui::SameLine();
    }
    CreateSpacing(nSpacing);
    
    AttractorData & curAttData = m_aData[curAtt];

    ImGui::Checkbox("Show attractor", &curAttData.show);

    ImGui::TextColored(headingClr, "Attractor type");
    ImGui::PushItemWidth(sliderOffset);

    const char* attrForces[] = { "Force is constant", "Force is a function of distance", "Force is a function of sq distance"};
    static int listVal;
    ImGui::ListBox("Force", &listVal, attrForces, ((int)(sizeof(attrForces) / sizeof(*attrForces))), 3);
    switch (listVal)
    {
    case 0: {curAttData.forceType = Dg::Attractor<float>::Constant; break; }
    case 1: {curAttData.forceType = Dg::Attractor<float>::Linear; break; }
    case 2: {curAttData.forceType = Dg::Attractor<float>::InvSq; break; }
    }

    const char* attrShapes[] = { "None", "Global", "Point", "Line", "Plane" };
    ImGui::ListBox("Shape", &listVal, attrShapes, ((int)(sizeof(attrShapes) / sizeof(*attrShapes))), 5);
    switch (listVal)
    {
    case 0: {curAttData.type = E_AttNone; break; }
    case 1: {curAttData.type = E_AttGlobal; break; }
    case 2: {curAttData.type = E_AttPoint; break; }
    case 3: {curAttData.type = E_AttLine; break; }
    case 4: {curAttData.type = E_AttPlane; break; }
    }

    CreateSpacing(nSpacing);
    ImGui::TextColored(headingClr, "Strength");
    ImGui::SliderFloat("Strength", &curAttData.strength, -100.0f, 100.0f, "%.2f m/s");
    ImGui::SliderFloat("Max accel", &curAttData.maxAccelMag, 0.0f, 500.0f, "%.2f m/s");
    
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
      ImGui::SliderFloat("Offset", &curAttData.transform[5], 0.0f, 10.0f, "offset = %.2f", 1.0f);
      break;
    }
    default: //E_AttNone:
    {
      break;
    }
    }
    
    ImGui::PopItemWidth();
  }
  ImGui::End();
}

void Application::DoLogic()
{
  //Get user input
  BuildMainUI();

  //Update particle system
  UpdateParSysAttr();
  m_particleSystem.Update(m_dt);

  //Update stats
  {
    ImGui::Begin("Stats");
    ImGui::Text("Live Particles: %i", m_particleSystem.GetParticleData()->GetCountAlive());
    ImGui::End();
  }

  //Zoom the camera
  if (abs(m_camZoomTarget - m_camZoom) > 0.1)
  {
    double dist = m_camZoomTarget - m_camZoom;
    double vec = (dist < 0.0) ? -1.0 : 1.0;
    double mag = vec * dist;
    double vel = 18.0 * pow(mag, 0.3);
    double dx = vec * vel * m_dt;
    m_camZoom += dx;
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

  int * pShowAttrTypes = new int[s_nAttractors];
  Dg::Matrix44<float> * pTAttr = new Dg::Matrix44<float>[s_nAttractors];
  int nShowAttr = 0;
  for (int i = 0; i < s_nAttractors; ++i)
  {
    if (m_aData[i].show)
    {
      pShowAttrTypes[nShowAttr] = m_aData[i].type;
      switch (m_aData[i].type)
      {
      case E_AttGlobal:
      {
        pTAttr[nShowAttr].Rotation(m_aData[i].transform[3]
                                 , m_aData[i].transform[4]
                                 , 0.0f
                                 , Dg::EulerOrder::ZXY);
        break;
      }
      case E_AttPoint:
      {
        pTAttr[nShowAttr].Translation
        (
          Dg::Vector4<float>(m_aData[i].transform[0]
                           , m_aData[i].transform[1]
                           , m_aData[i].transform[2]
                           , 0.0f)
        );
        break;
      }
      case E_AttLine:
      {
        pTAttr[nShowAttr].Identity();;
        break;
      }
      case E_AttPlane:
      {
        pTAttr[nShowAttr].Identity();;
        break;
      }
      }
      
      nShowAttr++;
    }
  }
  
  m_renderer.Update(m_particleSystem.GetParticleData());
  m_renderer.Render(mv_matrix, proj_matrix, parScale, nShowAttr, pShowAttrTypes, pTAttr);

  delete[] pShowAttrTypes;
  delete[] pTAttr;
}

void AppOnMouseScroll(double yOffset)
{
  Application::s_scrollOffset += yOffset;
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

    //Main window
	  {
      ImGui::Begin("Partice System");
      if (ImGui::Button("Blending")) UI::showAlphaBlendingWindow ^= 1; ImGui::SameLine();
      if (ImGui::Button("Example UI")) show_test_window ^= 1;
      ImGui::Spacing(); ImGui::Spacing();
      ImGui::End();
    }

    //FPS
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
