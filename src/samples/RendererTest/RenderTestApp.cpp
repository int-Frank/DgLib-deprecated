
#include <cmath>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShapeFunctions.h"
#include "Mod_Renderer_LineMesh.h"
#include "RenderTestApp.h"
#include "imgui.h"

void Log(std::string const & a_message, int a_logLevel)
{
  DgApp::GetInstance()->LogToOutputWindow(a_message, a_logLevel);
}

RenderTestApp::~RenderTestApp()
{
  delete m_pRender;
  delete m_pWindow;
  delete m_pViewport;
}

RenderTestApp::RenderTestApp()
  : m_pRender(nullptr)
  , m_pWindow(nullptr)
  , m_pViewport(nullptr)
{
  this->ToggleOutputWindow(false);
  Renderer::Init(Log);

  m_pRender = new Renderer::Renderer();
  
  Window::InitData windowData;
  windowData.clearColor = (vec4(0.0f, 0.0f, 0.0f, 0.0f));
  windowData.width = 200;
  windowData.height = 200;
  windowData.indentX = 700;
  windowData.indentY = 100;

  Window::WindowFactory windowFactory;
  m_pWindow = windowFactory.GetNewWindow(windowData);

  windowData.isImGuiWindow = false;
  int appW(0), appH(0);
  GetWindowDimensions(appW, appH);
  windowData.width = appW;
  windowData.height = appH;
  windowData.clearColor = (vec4(0.0f, 0.0f, 0.0f, 1.0f));

  m_pViewport = windowFactory.GetNewWindow(windowData);

  std::vector<Renderer::LineMesh> lineMeshes;
  std::vector<Renderer::TriangleMesh> triangleMeshes;

  lineMeshes.push_back(GenerateLineSpiral());
  lineMeshes.push_back(GenerateLineBox());
  lineMeshes.push_back(GenerateLineStar());
  lineMeshes.push_back(GenerateLineWheel());

  triangleMeshes.push_back(GenerateFilledShape(3));
  triangleMeshes.push_back(GenerateFilledShape(4));
  triangleMeshes.push_back(GenerateFilledShape(5));
  triangleMeshes.push_back(GenerateFilledShape(9));
  triangleMeshes.push_back(GenerateFilledShape(32));

  std::vector<vec4> colors;

  colors.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
  colors.push_back(vec4(0.0f, 1.0f, 0.0f, 1.0f));
  colors.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));
  colors.push_back(vec4(1.0f, 1.0f, 0.0f, 1.0f));
  colors.push_back(vec4(1.0f, 0.0f, 1.0f, 1.0f));
  colors.push_back(vec4(0.0f, 1.0f, 1.0f, 1.0f));
  colors.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  size_t colorIndex = 0;

  float marginX = 0.0f;
  float marginY = 0.0f;
  float spacing = (2.0f - marginX) / 3.0f;
  Dg::R3::Vector<float> tvec(-spacing, spacing, 0.0, 0.0);
  Dg::R3::Matrix<float> scale;
  scale.Scaling(0.4f);

  int itemNo = 0;

  for (auto const & lineMesh : lineMeshes)
  {
    ScreenObject so;

    tvec[0] = -1.0f + marginX + spacing / 2.0f + spacing * (itemNo % 3);
    tvec[1] = 1.0f - marginY - spacing / 2.0f - spacing * (itemNo / 3);
    Dg::R3::Matrix<float> translation;
    translation.Translation(tvec);

    so.transform = scale * translation;

    so.show = true;
    so.color = colors[colorIndex];
    so.handle = m_pRender->AddObject(lineMesh);
    m_lineObjects.push_back(so);
    colorIndex = (colorIndex + 1) % colors.size();

    itemNo++;
  }

  for (auto const & triangleMesh : triangleMeshes)
  {
    ScreenObject so;

    tvec[0] = -1.0f + marginX + spacing / 2.0f + spacing * (itemNo % 3);
    tvec[1] = 1.0f - marginY - spacing / 2.0f - spacing * (itemNo / 3);
    Dg::R3::Matrix<float> translation;
    translation.Translation(tvec);

    so.transform = scale * translation;

    so.show = true;
    so.color = colors[colorIndex];
    so.handle = m_pRender->AddObject(triangleMesh);
    m_triangleObjects.push_back(so);
    colorIndex = (colorIndex + 1) % colors.size();

    itemNo++;
  }

  m_pRender->CommitLoadList();
}

void RenderTestApp::WindowSizeCallback(int w, int h)
{
  m_pViewport->Resize(w, h);
}

void RenderTestApp::DoFrame(double a_dt)
{
  m_pRender->ClearAppColorBuffer();

  m_pWindow->BeginDraw();
  m_pRender->SetContext(Renderer::E_Lines);
  for (auto const & obj : m_lineObjects)
  {
    if (!obj.show) continue;
    m_pRender->SetColor(obj.color);
    m_pRender->SetTransform(obj.transform);
    m_pRender->Draw(obj.handle);
  }

  m_pRender->SetContext(Renderer::E_Triangles);
  for (auto const & obj : m_triangleObjects)
  {
    if (!obj.show) continue;
    m_pRender->SetColor(obj.color);
    m_pRender->SetTransform(obj.transform);
    m_pRender->Draw(obj.handle);
  }
  m_pWindow->EndDraw();

  ////////////////////////////////////////////////////////////

  m_pViewport->BeginDraw();
  m_pRender->SetContext(Renderer::E_Lines);
  for (auto const & obj : m_lineObjects)
  {
    if (!obj.show) continue;
    m_pRender->SetColor(obj.color);
    m_pRender->SetTransform(obj.transform);
    m_pRender->Draw(obj.handle);
  }

  m_pRender->SetContext(Renderer::E_Triangles);
  for (auto const & obj : m_triangleObjects)
  {
    if (!obj.show) continue;
    m_pRender->SetColor(obj.color);
    m_pRender->SetTransform(obj.transform);
    m_pRender->Draw(obj.handle);
  }
  m_pViewport->EndDraw();
  m_pViewport->Update();
}

void RenderTestApp::BuildUI()
{
  m_pWindow->Update();

  int winWidth(0), winHeight(0);
  GetWindowDimensions(winWidth, winHeight);

  ImGui::Begin("Main", nullptr, 0);
  
  ImGui::Separator();
  
  static bool showLogWindow = false;
  if (ImGui::Checkbox("Show log window", &showLogWindow))
  {
    this->ToggleOutputWindow(showLogWindow);
  }
  ImGui::Separator();

  if (ImGui::Button("Clear"))
  {

  }

  if (ImGui::Button("Load"))
  {

  }
  ImGui::Separator();

  int objCount = 1;
  for (auto & obj : m_lineObjects)
  {
    std::stringstream ss;
    ss << "Object " << objCount++;
    ImGui::Checkbox(ss.str().c_str(), &obj.show);
  }
  for (auto & obj : m_triangleObjects)
  {
    std::stringstream ss;
    ss << "Object " << objCount++;
    ImGui::Checkbox(ss.str().c_str(), &obj.show);
  }
  
  ImGui::End();
}