
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mod_Renderer_LineMesh.h"
#include "RenderTestApp.h"
#include "imgui.h"

//void TEST()
//{
//  //Create shader ID
//  GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
//  GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);
//
//  char vsSource[] = "#version 330 core\n\
//in vec4 position;\
//void main(void)\
//{\
//  gl_Position = position;\
//}";
//
//  char fsSource[] = "#version 330\n\
//out vec4 color;\
//void main(void)\
//{\
//color = vec4(1.0, 0.0, 0.0, 1.0);\
//}";
//
//  const char *p_vsSrc = &vsSource[0];
//  const char *p_fsSrc = &fsSource[0];
//
//  glShaderSource(vsID, 1, &p_vsSrc, nullptr);
//  glShaderSource(fsID, 1, &p_fsSrc, nullptr);
//
//  //Compile shader source
//  glCompileShader(vsID);
//  glCompileShader(fsID);
//
//  //Check shader for errors
//  GLint vsCompiled = GL_FALSE;
//  GLint fsCompiled = GL_FALSE;
//  glGetShaderiv(vsID, GL_COMPILE_STATUS, &vsCompiled);
//  glGetShaderiv(fsID, GL_COMPILE_STATUS, &fsCompiled);
//  if (vsCompiled != GL_TRUE)
//  {
//    return;
//  }
//  if (fsCompiled != GL_TRUE)
//  {
//    return;
//  }
//
//  //Create program, attach shaders to it and link it.
//  GLuint shaderProgram = glCreateProgram();
//  glAttachShader(shaderProgram, vsID);
//  glAttachShader(shaderProgram, fsID);
//  glLinkProgram(shaderProgram);
//  GLuint vPosition = glGetAttribLocation(shaderProgram, "position");
//
//  char brk = 0;
//}

void Log(std::string const & a_message, int a_logLevel)
{
  DgApp::GetInstance()->LogToOutputWindow(a_message, a_logLevel);
}

static Renderer::LineMesh GenerateStar()
{
  float pi = 3.14159f;

  int n_points = 6;
  float radius = 0.5f;
  float depth = 0.2f;
  float spacing = 2.0f * pi / float(n_points);
  float halfSpacing = spacing / 2.0f;

  Renderer::LineMesh result;

  for (int i = 0; i < n_points; i++)
  {
    Renderer::LineVertex point, dimple;

    point.point[0] = radius * cos(spacing * float(i));
    point.point[1] = radius * sin(spacing * float(i));
    point.point[2] = 0.0f;

    dimple.point[0] = depth * cos(spacing * float(i) + halfSpacing);
    dimple.point[1] = depth * sin(spacing * float(i) + halfSpacing);
    dimple.point[2] = 0.0f;

    result.verts.push_back(point);
    result.verts.push_back(dimple);
  }

  for (int i = 0; i < n_points * 2; i++)
  {
    Renderer::Line line;
    line.indices[0] = i;
    line.indices[1] = (i + 1) % (n_points * 2);
    result.lines.push_back(line);
  }

  return result;
}

static Renderer::LineMesh GenerateBox()
{
  Renderer::LineMesh result;
  Renderer::LineVertex point;

  point.point[2] = 0.0f;

  point.point[0] = -0.5f;
  point.point[1] = -0.5f;
  result.verts.push_back(point);

  point.point[0] = 0.5f;
  point.point[1] = -0.5f;
  result.verts.push_back(point);

  point.point[0] = 0.5f;
  point.point[1] = 0.5f;
  result.verts.push_back(point);

  point.point[0] = -0.5f;
  point.point[1] = 0.5f;
  result.verts.push_back(point);

  Renderer::Line line;

  line.indices[0] = 0;
  line.indices[1] = 1;
  result.lines.push_back(line);

  line.indices[0] = 1;
  line.indices[1] = 2;
  result.lines.push_back(line);

  line.indices[0] = 2;
  line.indices[1] = 3;
  result.lines.push_back(line);

  line.indices[0] = 3;
  line.indices[1] = 0;
  result.lines.push_back(line);

  return result;
}

static Renderer::LineMesh GenerateSnowFlake()
{

}

static Renderer::LineMesh GenerateFace()
{

}

RenderTestApp::~RenderTestApp()
{
  delete m_pRender;
}

RenderTestApp::RenderTestApp()
{
  Renderer::Init(Log);

  m_pRender = new Renderer::Renderer();

  std::vector<Renderer::LineMesh> lineMeshes;
  std::vector<Renderer::TriangleMesh> triangleMeshes;

  lineMeshes.push_back(GenerateBox());
  lineMeshes.push_back(GenerateStar());

  std::vector<vec4> colors;

  colors.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
  colors.push_back(vec4(0.0f, 1.0f, 0.0f, 1.0f));
  colors.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));
  colors.push_back(vec4(1.0f, 1.0f, 0.0f, 1.0f));
  colors.push_back(vec4(1.0f, 0.0f, 1.0f, 1.0f));
  colors.push_back(vec4(0.0f, 1.0f, 1.0f, 1.0f));
  colors.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  size_t colorIndex = 0;

  for (auto const & lineMesh : lineMeshes)
  {
    ScreenObject so;
    so.color = colors[colorIndex];
    so.handle = m_pRender->AddObject(lineMesh);
    m_lineObjects.push_back(so);
    colorIndex = (colorIndex + 1) % colors.size();
  }

  for (auto const & triangleMesh : triangleMeshes)
  {
    ScreenObject so;
    so.color = colors[colorIndex];
    m_pRender->AddObject(triangleMesh);
    m_triangleObjects.push_back(so);
    colorIndex = (colorIndex + 1) % colors.size();
  }

  m_pRender->CommitLoadList();
}

void RenderTestApp::DoFrame(double a_dt)
{
  m_pRender->BeginDraw();

  m_pRender->SetContext(Renderer::E_Lines);
  for (auto const & obj : m_lineObjects)
  {
    m_pRender->SetColor(obj.color);
    m_pRender->Draw(obj.handle);
  }

  m_pRender->SetContext(Renderer::E_Triangles);
  for (auto const & obj : m_triangleObjects)
  {
    m_pRender->SetColor(obj.color);
    m_pRender->Draw(obj.handle);
  }

  m_pRender->EndDraw();
}

void RenderTestApp::BuildUI()
{
  //ImGui::Begin("Main", nullptr
  //  , ImGuiWindowFlags_MenuBar
  //  | ImGuiWindowFlags_NoTitleBar
  //);
  //
  //ImGui::Separator();
  //
  //if (ImGui::Checkbox("Show log window", &m_showLogWindow))
  //{
  //  this->ToggleOutputWindow(m_showLogWindow);
  //}
  //ImGui::Separator();
  //
  //int itemCount = 0;
  //ImGui::Checkbox("SPWH",   &m_showItem[itemCount++]);
  //ImGui::Checkbox("SPWHCP", &m_showItem[itemCount++]);
  //ImGui::Checkbox("MA",     &m_showItem[itemCount++]);
  //ImGui::Checkbox("Mesh",   &m_showItem[itemCount++]);
  //ImGui::SliderFloat("triSze", &m_meshSize, 0.02f, 0.3f);
  //ImGui::Separator();
  //
  //if (ImGui::Button("Recalculate"))
  //{
  //  PushEvent(Event_BuildData(m_data.spwh));
  //}
  //ImGui::End();
}