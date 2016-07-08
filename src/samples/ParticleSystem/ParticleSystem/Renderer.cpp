
#include <fstream>

#include "Renderer.h"
#include "particle_system/DgParticleData.h"
#include "DgMatrix44.h"

#include "imgui/imgui.h"
#include "UI.h"

bool Renderer::Init(Dg::ParticleData<float> * a_parData)
{
  if (a_parData == nullptr)
  {
    return false;
  }

  int countMax = a_parData->GetCountMax();

  m_idShaderProgram = CompileShaders();

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glEnable(GL_ALPHA_TEST);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_POINT_SPRITE);
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

  glDepthFunc(GL_LESS);

  glGenBuffers(1, &m_idBufffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_idBufffer);
  glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * (4 + 4 + 1) * countMax), nullptr, GL_DYNAMIC_DRAW);

  GLuint idPosition = glGetAttribLocation(m_idShaderProgram, "position");
  GLuint idColor = glGetAttribLocation(m_idShaderProgram, "color");
  GLuint idSize = glGetAttribLocation(m_idShaderProgram, "pointSize");

  glVertexAttribPointer(idPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(idColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * 4 * countMax));
  glVertexAttribPointer(idSize, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * 8 * countMax));

  glEnableVertexAttribArray(idPosition);
  glEnableVertexAttribArray(idColor);
  glEnableVertexAttribArray(idSize);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return true;
}

void Renderer::Update(Dg::ParticleData<float> * a_parData)
{
  glBindBuffer(GL_ARRAY_BUFFER, m_idBufffer);

  m_nCurrentParticles = a_parData->GetCountAlive();
  int countMax = a_parData->GetCountMax();

  float * ptr = (a_parData->GetPosition()[0].GetData());
  glBufferSubData(GL_ARRAY_BUFFER, 0, m_nCurrentParticles * 4 * sizeof(float), ptr);

  ptr = (a_parData->GetColor()[0].GetData());
  glBufferSubData(GL_ARRAY_BUFFER, countMax * 4 * sizeof(float), m_nCurrentParticles * 4 * sizeof(float), ptr);

  ptr = (a_parData->GetSize());
  glBufferSubData(GL_ARRAY_BUFFER, countMax * 8 * sizeof(float), m_nCurrentParticles * 1 * sizeof(float), ptr);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static int GetEnumFromListVal(int a_val)
{
  switch (a_val)
  {
  case 0: return GL_ZERO;
  default:
  case 1: return GL_ONE;
  case 2: return GL_SRC_COLOR;
  case 3: return GL_ONE_MINUS_SRC_COLOR;
  case 4: return GL_DST_COLOR;
  case 5: return GL_ONE_MINUS_DST_COLOR;
  case 6: return GL_SRC_ALPHA;
  case 7: return GL_ONE_MINUS_SRC_ALPHA;
  case 8: return GL_DST_ALPHA;
  case 9: return GL_ONE_MINUS_DST_ALPHA;
  case 10: return GL_CONSTANT_COLOR;
  case 11: return GL_ONE_MINUS_CONSTANT_COLOR;
  case 12: return GL_CONSTANT_ALPHA;
  case 13: return GL_ONE_MINUS_CONSTANT_ALPHA;
  case 14: return GL_SRC_ALPHA_SATURATE;
  }
}

void Renderer::Render(Dg::Matrix44<float> const & a_modelView
                    , Dg::Matrix44<float> const & a_proj
                    , float a_parScale)
{
  glBindVertexArray(m_vao);

  static int sfactor_ind(6);
  static int dfactor_ind(7);

  if (UI::showAlphaBlendingWindow)
  {
    char const * sfactorItems[] = 
    { 
      "GL_ZERO",
      "GL_ONE",
      "GL_SRC_COLOR",
      "GL_ONE_MINUS_SRC_COLOR",
      "GL_DST_COLOR",
      "GL_ONE_MINUS_DST_COLOR",
      "GL_SRC_ALPHA",
      "GL_ONE_MINUS_SRC_ALPHA",
      "GL_DST_ALPHA",
      "GL_ONE_MINUS_DST_ALPHA",
      "GL_CONSTANT_COLOR",
      "GL_ONE_MINUS_CONSTANT_COLOR",
      "GL_CONSTANT_ALPHA",
      "GL_ONE_MINUS_CONSTANT_ALPHA",
      "GL_SRC_ALPHA_SATURATE" 
    };

    char const * dfactorItems[] =
    {
      "GL_ZERO",
      "GL_ONE",
      "GL_SRC_COLOR",
      "GL_ONE_MINUS_SRC_COLOR",
      "GL_DST_COLOR",
      "GL_ONE_MINUS_DST_COLOR",
      "GL_SRC_ALPHA",
      "GL_ONE_MINUS_SRC_ALPHA",
      "GL_DST_ALPHA",
      "GL_ONE_MINUS_DST_ALPHA,"
      "GL_CONSTANT_COLOR",
      "GL_ONE_MINUS_CONSTANT_COLOR",
      "GL_CONSTANT_ALPHA",
      "GL_ONE_MINUS_CONSTANT_ALPHA"
    };

    ImGui::Begin("Alpha blending", &UI::showAlphaBlendingWindow, 0);
    ImGui::Text("sfactor: Source rgba blending method");
    ImGui::Text("dfactor: Destination rgba blending method");
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.8f);
    ImGui::ListBox("sfactor", &sfactor_ind, sfactorItems, ((int)(sizeof(sfactorItems) / sizeof(*sfactorItems))), 15);
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    ImGui::ListBox("dfactor", &dfactor_ind, dfactorItems, ((int)(sizeof(dfactorItems) / sizeof(*dfactorItems))), 13);
    ImGui::PopItemWidth();
    ImGui::End();

    glBlendFunc(GetEnumFromListVal(sfactor_ind), GetEnumFromListVal(dfactor_ind));
  }

  glUseProgram(m_idShaderProgram);

  GLint mv_loc = glGetUniformLocation(m_idShaderProgram, "mv_matrix");
  GLint proj_loc = glGetUniformLocation(m_idShaderProgram, "proj_matrix");
  GLint parScale_loc = glGetUniformLocation(m_idShaderProgram, "parScale");

  glUniformMatrix4fv(mv_loc, 1, GL_FALSE, a_modelView.GetData());
  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, a_proj.GetData());
  glUniform1f(parScale_loc, a_parScale);

  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  
  //Draw points 
  glDrawArrays(GL_POINTS, 0, m_nCurrentParticles);

  glBindVertexArray(0);
}

void Renderer::ShutDown()
{
  if (m_idBufffer != 0)
  {
    glDeleteBuffers(1, &m_idBufffer);
    m_idBufffer = 0;
    glDeleteProgram(m_idShaderProgram);
  }

  glDeleteVertexArrays(1, &m_vao);
  glDeleteProgram(m_idShaderProgram);
}

GLuint Renderer::LoadShaderFromFile(std::string path, GLenum shaderType)
{
  //Open file
  GLuint shaderID = 0;
  std::string shaderString;
  std::ifstream sourceFile(path.c_str());

  //Source file loaded
  if (sourceFile)
  {
    //Get shader source
    shaderString.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());

    //Create shader ID
    shaderID = glCreateShader(shaderType);

    //Set shader source
    const GLchar* shaderSource = shaderString.c_str();
    glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, nullptr);

    //Compile shader source
    glCompileShader(shaderID);

    //Check shader for errors
    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != GL_TRUE)
    {
      printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource);
      glDeleteShader(shaderID);
      shaderID = 0;
    }
  }
  else
  {
    printf("Unable to open file %s\n", path.c_str());
  }

  return shaderID;
}

GLuint Renderer::CompileShaders()
{
  GLuint vs = LoadShaderFromFile("vs.glsl", GL_VERTEX_SHADER);

  //Check for errors
  if (vs == 0)
    return 0;

  GLuint fs = LoadShaderFromFile("fs.glsl", GL_FRAGMENT_SHADER);

  //Check for errors
  if (fs == 0)
    return 0;

  //Create program, attach shaders to it and link it.
  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  return program;
}