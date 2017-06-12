
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

  //We need to render the GUI a couple of times before it positions
  //itself correctly, for some reason.
  InitGUI();
  ImGui::Render();
  glfwSwapBuffers(m_window);

  InitGUI();
  GetCanvasBounds();
  ImGui::Render();
  glfwSwapBuffers(m_window);

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
  m_polygons.clear();
}

void Application::DrawSegment(Segment const & a_seg, ImColor const & a_clr)
{
  ImGui::Begin("CanvasWindow");

  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  ImVec2 p0(a_seg.GetP0().x(), a_seg.GetP0().y());
  ImVec2 p1(a_seg.GetP1().x(), a_seg.GetP1().y());

  draw_list->AddLine(p0, p1, a_clr, 1);

  ImGui::End();
}

void Application::DrawFilledPolygon(Polygon const & a_poly, ImColor const & a_clr)
{
  ImGui::Begin("CanvasWindow");

  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  int size = int(a_poly.size());
  ImVec2 * points = new ImVec2[a_poly.size()];
  auto it = a_poly.chead();
  for (int i = 0; i < a_poly.size(); i++, it++)
  {
    points[i].x = it->x();
    points[i].y = it->y();
  }

  draw_list->AddConvexPolyFilled(points, size, a_clr, true);

  ImGui::End();
}

void Application::DrawPolygons()
{
  for (auto const & kv : m_polygons)
  {
    auto it = kv.second.chead();
    Polygon polygon;
    for (size_t i = 0; i < kv.second.size(); i++, it++)
    {
      polygon.push_back(*it * m_T_model_screen);
    }

    DrawFilledPolygon(polygon, ImColor(82, 82, 112, 255));
  }

  for (auto const & kv : m_polygons)
  {
    auto it0 = kv.second.chead();
    for (size_t i = 0; i < kv.second.size(); i++, it0++)
    {
      auto it1 = it0;
      it1++;
      Segment seg(*it0 * m_T_model_screen, *it1 * m_T_model_screen);
      DrawSegment(seg, ImColor(180, 180, 200, 255));
    }
  }
}

void Application::DrawScene()
{
  DrawPolygons();
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

  for (int i = 0; i < parser.GetItems().size(); ++i)
  {
    
  }
}

void Application::InitGUI()
{
  glfwPollEvents();
  ImGui_ImplGlfwGL3_NewFrame();

  //Editor
  {
    ImGui::Begin("Editor", nullptr,
      ImGuiWindowFlags_NoTitleBar
      | ImGuiWindowFlags_MenuBar
      | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowPos(ImVec2(m_bufferWidth, m_bufferWidth));
    ImGui::SetWindowSize(ImVec2(m_editorWidth, m_editorHeight));
    ImGui::End();
  }

  //Canvas
  {
    ImGui::Begin("CanvasWindow", nullptr,
      ImGuiWindowFlags_NoTitleBar
      | ImGuiWindowFlags_MenuBar
      | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_NoCollapse
      | ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetWindowPos(ImVec2(m_editorWidth + m_bufferWidth * 2, m_bufferWidth));
    ImGui::SetWindowSize(ImVec2(m_canvasWidth, m_canvasHeight));
    ImGui::End();
  }
}

void Application::ShowGUI()
{
  InitGUI();

  //Example UI window
  if (UI::showExampleWindow)
  {
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
    ImGui::ShowTestWindow(&UI::showExampleWindow);
  }

  ShowGUI_Editor();
  ShowGUI_Canvas();
}

void Application::Run(Application* the_app)
{
  s_app = the_app;

  if (!Init())
  {
    fprintf(stderr, "Failed to initialize Application\n");
    return;
  }

  do
  {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    ShowGUI();
    DrawScene();

	  ImGui::Render();
    glfwSwapBuffers(m_window);

  } while (!glfwWindowShouldClose(m_window) && !m_shouldQuit);

  Shutdown();

  ImGui_ImplGlfwGL3_Shutdown();
  glfwDestroyWindow(m_window);
  glfwTerminate();
}
