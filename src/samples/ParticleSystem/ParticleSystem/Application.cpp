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

#include "EmitterFactory.h"
#include "AttractorFactory.h"

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

  //Set options
  m_parSysOpts.useRelativeForce = false;
  m_parSysOpts.preset = 0;
  memcpy(&m_parSysOptsPrev, &m_parSysOpts, sizeof(ParSysOpts));

  m_attrFocus = -1;

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
  char const * vRotformats[2] = { "yaw = %.2f", "pitch = %.2f" };
  float powers2[2] = { 1.0f, 1.0f };
  float posMins[3] = { -10.0f, -10.0f, -10.0f };
  float posMaxs[3] = { 10.0f, 10.0f, 10.0f };
  char const * posFormats[3] = { "x = %.2f", "y = %.2f", "z = %.2f" };
  float powers3[3] = { 1.0f, 1.0f, 1.0f };

  static bool show_app_metrics = false;
  static bool show_app_about = false;

  ImGui::Begin("Partice System");

  // Menu
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("Menu"))
    {
      if (ImGui::MenuItem("New")) {}
      if (ImGui::MenuItem("Open")) {}
      if (ImGui::BeginMenu("Open Recent"))
      {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        ImGui::EndMenu();
      }
      if (ImGui::MenuItem("Save")) {}
      if (ImGui::MenuItem("Save As..")) {}
      ImGui::Separator();
      if (ImGui::BeginMenu("Options"))
      {
        ImGui::EndMenu();
      }
      if (ImGui::MenuItem("Quit", "Esc")) {}
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Options"))
    {
      ImGui::MenuItem("Example UI", NULL, &UI::showExampleWindow);
      ImGui::Separator();
      ImGui::MenuItem("Blending", NULL, &UI::showAlphaBlendingWindow);
      ImGui::MenuItem("Metrics", NULL, &UI::showMetrics);
      ImGui::Separator();
      ImGui::MenuItem("Enable rel force", NULL, &m_parSysOpts.useRelativeForce);
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Examples"))
    {
      if(ImGui::MenuItem("Basic 3 emitter")) {}
      if(ImGui::MenuItem("Global acceleration")) {}
      if(ImGui::MenuItem("Point attractor")) {}
      if(ImGui::MenuItem("Line attractor")) {}
      if(ImGui::MenuItem("Plane attractor")) {}
      if(ImGui::MenuItem("Cycle")) {}
      if(ImGui::MenuItem("Fire")) {}
      if(ImGui::MenuItem("Fountain")) {}
      if(ImGui::MenuItem("Rain")) {}
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help"))
    {
      ImGui::MenuItem("About ImGui", NULL, &show_app_about);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  if (ImGui::CollapsingHeader("Emitters"))
  {
    if (ImGui::Button("Add Emitter"))
    {
      EmitterData data;
      data.ID = m_IDManager.GetID();
      eDataItem item(data, data);
      m_eData.push_back(item);

      EmitterFactory eFact;
      m_particleSystem.AddEmitter(data.ID, eFact(data));
      curEm = (int)m_eData.size() - 1;
    }

    ImGui::SameLine();
    if (ImGui::Button("Kill Emitter") && m_eData.size())
    {
      m_particleSystem.RemoveEmitter(m_eData[curEm].first.ID);
      m_IDManager.ReturnID(m_eData[curEm].first.ID);
      m_eData.erase(m_eData.begin() + curEm);
      if (curEm == m_eData.size())
      {
        curEm--;
      }
    }

    //This doesn't work :( Have to use char**
    //typedef char inner_array_t[32];
    //inner_array_t * currentEmitters = new inner_array_t[4];

    char ** currentEmitters = new char*[m_eData.size()];
    for (int i = 0; i < m_eData.size(); ++i)
    {
      currentEmitters[i] = new char[32]();
      sprintf_s(currentEmitters[i], 32, "Emitter %i", m_eData[i].first.ID);
    }
    ImGui::PushItemWidth(sliderOffset);
    ImGui::ListBox("Emitter", &curEm, (char const **)currentEmitters, (int)m_eData.size(), 5);
    ImGui::PopItemWidth();
    ImGui::Separator();

    for (int i = 0; i < m_eData.size(); ++i)
    {
      delete[] currentEmitters[i];
    }
    delete[] currentEmitters;

    if (m_eData.size() > 0)
    {
      EmitterData & curEmData = m_eData[curEm].first;

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
      ImGui::SliderFloat("Rate", &curEmData.rate, 0.0f, 500.0f, "%.2f par/s");
      ImGui::SliderFloat("Velocity", &curEmData.velocity, 0.0f, 10.0f, "%.2f m/s");
      if (m_parSysOpts.useRelativeForce)
      {
        ImGui::SliderFloat("Rel force", &curEmData.relativeForce, 0.0f, 10.0f, "%.4f m/s", 3.0f);
      }
      ImGui::SliderFloat("Life", &curEmData.life, 0.0f, 60.0f, "%.2f s");
      ImGui::SliderFloat("Start size", curEmData.sizes, 0.0f, 1.0f, "%.2f m");
      ImGui::SliderFloat("End size", &curEmData.sizes[1], 0.0f, 1.0f, "%.2f m");

      ImGui::PopItemWidth();
    }
  }

  //Attractors
  if (ImGui::CollapsingHeader("Attractors"))
  {
    if (ImGui::Button("Add Attractor"))
    {
      AttractorData data;
      data.ID = m_IDManager.GetID();
      aDataItem item(data, data);
      m_aData.push_back(item);

      AttractorFactory aFact;
      m_particleSystem.AddUpdater(data.ID, aFact(data));
      m_attrFocus = (int)m_aData.size() - 1;
    }

    ImGui::SameLine();
    if (ImGui::Button("Kill Attractor") && m_aData.size())
    {
      m_particleSystem.RemoveUpdater(m_aData[m_attrFocus].first.ID);
      m_IDManager.ReturnID(m_aData[m_attrFocus].first.ID);
      m_aData.erase(m_aData.begin() + m_attrFocus);
      if (m_attrFocus == m_aData.size())
      {
        m_attrFocus--;
      }
    }


    char ** currentAttractors = new char*[m_aData.size()];
    for (int i = 0; i < m_aData.size(); ++i)
    {
      currentAttractors[i] = new char[32]();
      sprintf_s(currentAttractors[i], 32, "Attractor %i", m_aData[i].first.ID);
    }
    ImGui::PushItemWidth(sliderOffset);
    ImGui::ListBox("Attractor", &m_attrFocus, (char const **)currentAttractors, (int)m_aData.size(), 5);
    ImGui::PopItemWidth();
    ImGui::Separator();

    for (int i = 0; i < m_aData.size(); ++i)
    {
      delete[] currentAttractors[i];
    }
    delete[] currentAttractors;


    if (m_aData.size() > 0)
    {
      CreateSpacing(nSpacing);

      AttractorData & curAttData = m_aData[m_attrFocus].first;

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
      ImGui::SliderFloat("Max accel", &curAttData.maxAppliedAccelMag, 0.0f, 500.0f, "%.2f m/s");

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

void Application::DoLogic()
{
  //Get user input
  BuildMainUI();

  //Update particle system
  UpdateParSysAttr();
  m_particleSystem.Update(m_dt);

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
    m_camZoom = m_camZoomTarget - diff / (pow(1.3f, 26.0f * m_dt));
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

  LineRenderData * lineRenderData = new LineRenderData[m_aData.size()];
  int nShowAttr = 0;
  for (int i = 0; i < m_aData.size(); ++i)
  {
    if (m_aData[i].first.show)
    {
      lineRenderData[nShowAttr].model = m_aData[i].first.type;
      Dg::Vector4<float> lineCol(1.0f, 0.4588f, 0.102f, 1.0f);
      Dg::Vector4<float> lineColFocus(51.f / 255.f, 204.f / 255.f, 51.f / 255.f, 1.0f);
      lineRenderData[nShowAttr].col = (i == m_attrFocus) ? lineColFocus : lineCol;
      switch (m_aData[i].first.type)
      {
      case E_AttGlobal:
      {
        lineRenderData[nShowAttr].mat.Rotation(0.0f
                                 , m_aData[i].first.transform[4] - Dg::PI_f / 2.0f
                                 , m_aData[i].first.transform[3]
                                 , Dg::EulerOrder::XYZ);
        break;
      }
      case E_AttPoint:
      {
        lineRenderData[nShowAttr].mat.Translation
        (
          Dg::Vector4<float>(m_aData[i].first.transform[0]
                           , m_aData[i].first.transform[1]
                           , m_aData[i].first.transform[2]
                           , 0.0f)
        );
        break;
      }
      case E_AttLine:
      {
        mat44 trans, rot;
        trans.Translation
        (
          Dg::Vector4<float>(m_aData[i].first.transform[0]
                           , m_aData[i].first.transform[1]
                           , m_aData[i].first.transform[2]
                           , 0.0f)
        );

        rot.Rotation(0.0f
                   , m_aData[i].first.transform[4] - Dg::PI_f / 2.0f
                   , m_aData[i].first.transform[3]
                   , Dg::EulerOrder::XYZ);
        lineRenderData[nShowAttr].mat = rot * trans;
        break;
      }
      case E_AttPlane:
      {
        mat44 trans, rot;
        trans.Translation
        (
          Dg::Vector4<float>(m_aData[i].first.transform[0]
                           , m_aData[i].first.transform[1]
                           , m_aData[i].first.transform[2]
                           , 0.0f)
        );

        rot.Rotation(0.0f
                   , m_aData[i].first.transform[4] - Dg::PI_f / 2.0f
                   , m_aData[i].first.transform[3]
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

    double thisTick = glfwGetTime();
	  double diff = thisTick - lastTick;
    m_dt = static_cast<float>(thisTick - lastTick);
    lastTick = thisTick;

    HandleInput();
    DoLogic();
    Render();

    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
    if (UI::showExampleWindow)
    {
      ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
      ImGui::ShowTestWindow(&UI::showExampleWindow);
    }

	  ImGui::Render();
    glfwSwapBuffers(m_window);

  } while (!glfwWindowShouldClose(m_window));

  Shutdown();

  ImGui_ImplGlfwGL3_Shutdown();
  glfwDestroyWindow(m_window);
  glfwTerminate();
}
