
#include <cmath>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "DgRNG.h"
#include "ShapeFunctions.h"
#include "RenderTestApp.h"
#include "imgui.h"

void Log(std::string const & a_message, int a_logLevel)
{
  DgApp::GetInstance()->LogToOutputWindow(a_message, a_logLevel);
}

RenderTestApp::~RenderTestApp()
{
  delete m_pWindow;
  delete m_pViewport;
}

RenderTestApp::RenderTestApp()
  : m_pWindow(nullptr)
  , m_pViewport(nullptr)
{
  this->ToggleOutputWindow(false);
  Context::Init(Log);

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

  std::vector<Context::LineMesh> lineMeshes;
  std::vector<Context::TriangleMesh> triangleMeshes;

  lineMeshes.push_back(GenerateLineSpiral());
  lineMeshes.push_back(GenerateLineBox());
  lineMeshes.push_back(GenerateLineStar());
  lineMeshes.push_back(GenerateLineWheel());

  triangleMeshes.push_back(GenerateFilledShape(3));
  triangleMeshes.push_back(GenerateFilledShape(4));
  triangleMeshes.push_back(GenerateFilledShape(5));
  triangleMeshes.push_back(GenerateFilledShape(9));

  float marginX = 0.0f;
  float marginY = 0.0f;
  float spacing = (2.0f - marginX) / 3.0f;
  Dg::R3::Vector<float> tvec(-spacing, spacing, 0.0f, 0.0f);
  Dg::R3::Matrix<float> scale;
  scale.Scaling(Dg::R3::Vector<float>(0.4f, 0.4f, 1.0f, 0.0f));

  int itemNo = 0;

  for (auto lineMesh : lineMeshes)
  {
    ScreenObject so;

    tvec[0] = -1.0f + marginX + spacing / 2.0f + spacing * (itemNo % 3);
    tvec[1] = 1.0f - marginY - spacing / 2.0f - spacing * (itemNo / 3);
    Dg::R3::Matrix<float> translation;
    translation.Translation(tvec);

    so.transform = scale * translation;

    Dg::RNG rng;
    float r = rng.GetUniform(0.0f, 1.0f);
    float g = rng.GetUniform(0.0f, 1.0f);
    float b = rng.GetUniform(0.0f, 1.0f);
    lineMesh.color = vec4(r, g, b, 1.0f);

    lineMesh.thickness = rng.GetUniform(1.0f, 10.0f);

    so.show = true;
    so.handle = m_contextLine.AddObject(lineMesh);
    m_lineObjects.push_back(so);

    itemNo++;
  }

  itemNo++;
  for (auto triangleMesh : triangleMeshes)
  {
    ScreenObject so;

    tvec[0] = -1.0f + marginX + spacing / 2.0f + spacing * (itemNo % 3);
    tvec[1] = 1.0f - marginY - spacing / 2.0f - spacing * (itemNo / 3);

    Dg::R3::Matrix<float> translation;
    translation.Translation(tvec);

    so.transform = scale * translation;

    Dg::RNG rng;
    float r = rng.GetUniform(0.0f, 1.0f);
    float g = rng.GetUniform(0.0f, 1.0f);
    float b = rng.GetUniform(0.0f, 1.0f);
    triangleMesh.color = vec4(r, g, b, 1.0f);

    so.show = true;
    so.handle = m_contextTriangle.AddObject(triangleMesh);
    m_triangleObjects.push_back(so);

    itemNo++;
  }

  triangleMeshes.push_back(GenerateFilledShape(32));
  triangleMeshes.push_back(GenerateFilledShape(32));

  for (size_t i = triangleMeshes.size() - 2; i < triangleMeshes.size(); i++)
  {
    ScreenObject so;

    float delta = -0.1f;
    if (i == triangleMeshes.size() - 2) delta = -delta;

    tvec[0] = -1.0f + marginX + spacing / 2.0f + spacing + delta;
    tvec[1] = 1.0f - marginY - spacing / 2.0f - spacing;
    tvec[2] = delta;

    Dg::R3::Matrix<float> translation;
    translation.Translation(tvec);

    so.transform = scale * translation;

    Dg::RNG rng;
    float r = rng.GetUniform(0.0f, 1.0f);
    float g = rng.GetUniform(0.0f, 1.0f);
    float b = rng.GetUniform(0.0f, 1.0f);
    triangleMeshes[i].color = vec4(r, g, b, 0.5f);

    so.show = true;
    so.handle = m_contextTriangle.AddObject(triangleMeshes[i]);
    m_triangleObjects.push_back(so);
  }

  m_contextLine.CommitLoadList();
  m_contextTriangle.CommitLoadList();
}

void RenderTestApp::WindowSizeCallback(int w, int h)
{
  m_pViewport->Resize(w, h);
}

void RenderTestApp::DoFrame(double a_dt)
{
  m_pWindow->BeginDraw();
  m_contextLine.Bind();
  for (auto const & obj : m_lineObjects)
  {
    if (!obj.show) continue;
    m_contextLine.SetMatrix(obj.transform);
    m_contextLine.Draw(obj.handle);
  }
  m_contextLine.Unbind();

  m_contextTriangle.Bind();
  for (auto const & obj : m_triangleObjects)
  {
    if (!obj.show) continue;
    m_contextTriangle.SetMatrix(obj.transform);
    m_contextTriangle.Draw(obj.handle);
  }
  m_contextTriangle.Unbind();
  m_pWindow->EndDraw();

  ////////////////////////////////////////////////////////////

  m_pViewport->BeginDraw();
  m_contextLine.Bind();
  for (auto const & obj : m_lineObjects)
  {
    if (!obj.show) continue;
    m_contextLine.SetMatrix(obj.transform);
    m_contextLine.Draw(obj.handle);
  }
  m_contextLine.Unbind();

  m_contextTriangle.Bind();
  for (auto const & obj : m_triangleObjects)
  {
    if (!obj.show) continue;
    m_contextTriangle.SetMatrix(obj.transform);
    m_contextTriangle.Draw(obj.handle);
  }
  m_contextTriangle.Unbind();
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