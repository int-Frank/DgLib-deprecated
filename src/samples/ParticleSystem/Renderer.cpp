
#include <fstream>

#include "Renderer.h"
#include "particle_system/DgParticleData.h"
#include "DgR3Matrix.h"
#include "DgMakeGrid.h"
#include "imgui/imgui.h"
#include "Types.h"
#include "UI.h"

bool Renderer::Init(Dg::ParticleData<float> * a_parData)
{
  if (a_parData == nullptr)
  {
    return false;
  }

  int countMax = a_parData->GetCountMax();

  glGenBuffers(s_nBuffers, m_buffers);
  glGenVertexArrays(1, &m_pt_vao);
  glBindVertexArray(m_pt_vao);

  //--------------------------------------------------------------------------------
  //  Set up point rendering
  //--------------------------------------------------------------------------------
  
  m_pt_shaderProgram = CompileShaders("pt_vs.glsl", "pt_fs.glsl");

  glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * (4 + 4 + 1) * countMax), nullptr, GL_DYNAMIC_DRAW);

  GLuint ptPosition = glGetAttribLocation(m_pt_shaderProgram, "position");
  GLuint ptColor = glGetAttribLocation(m_pt_shaderProgram, "color");
  GLuint ptSize = glGetAttribLocation(m_pt_shaderProgram, "pointSize");

  glVertexAttribPointer(ptPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(ptColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * 4 * countMax));
  glVertexAttribPointer(ptSize, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * 8 * countMax));

  glEnableVertexAttribArray(ptPosition);
  glEnableVertexAttribArray(ptColor);
  glEnableVertexAttribArray(ptSize);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  //--------------------------------------------------------------------------------
  //  Set up line rendering
  //--------------------------------------------------------------------------------
  
  glGenVertexArrays(1, &m_ln_vao);
  glBindVertexArray(m_ln_vao);
  
  int const nLines = s_nLinesGlobal + s_nLinesPoint + s_nLinesLine + s_nLinesPlane;
  m_ln_shaderProgram = CompileShaders("ln_vs.glsl", "ln_fs.glsl");

  glBindBuffer(GL_ARRAY_BUFFER, m_buffers[1]);

  float const arrowLength = 5.0f;
  float const crossLength = 2.0f;
  float const lineLength = 100.0f;
  float const gridSize = 20.0f;

  float lines[nLines * 3 * 2] =
  {
    //Global - arrow
    0.0f , 0.0f, 0.0f,
    arrowLength , 0.0f, 0.0f,
    arrowLength , 0.0f, 0.0f,
    arrowLength * 0.8f , 0.0f, arrowLength * 0.2f,
    arrowLength , 0.0f, 0.0f,
    arrowLength * 0.8f , 0.0f, -arrowLength * 0.2f,

    //Point lines
    -crossLength * 0.5f, 0.0f, 0.0f,
    crossLength * 0.5f, 0.0f, 0.0f,
    0.0f, -crossLength * 0.5f, 0.0f,
    0.0f, crossLength * 0.5f, 0.0f,
    0.0f, 0.0f, -crossLength * 0.5f,
    0.0f, 0.0f, crossLength * 0.5f,

    //Line lines
    -lineLength * 0.5f, 0.0f, 0.0f,
    lineLength * 0.5f, 0.0f, 0.0f,
    0.0f, -crossLength * 0.5f, 0.0f,
    0.0f, crossLength * 0.5f, 0.0f,
    0.0f, 0.0f, -crossLength * 0.5f,
    0.0f, 0.0f, crossLength * 0.5f,

    //Grid lines
    0.0f , 0.0f, 0.0f,
    arrowLength , 0.0f, 0.0f,
    arrowLength , 0.0f, 0.0f,
    arrowLength * 0.8f , 0.0f, arrowLength * 0.2f,
    arrowLength , 0.0f, 0.0f,
    arrowLength * 0.8f , 0.0f, -arrowLength * 0.2f,
  };
  
  float const bounds[12] = 
  {
    0.0f, -gridSize * 0.5,  gridSize * 0.5,
    0.0f, gridSize * 0.5,  gridSize * 0.5,
    0.0f, -gridSize * 0.5,  -gridSize * 0.5,
    0.0f, gridSize * 0.5,  -gridSize * 0.5
  };

  Dg::DynamicArray<float> vLines;
  Dg::MakeLineGrid<float, 3>(vLines, s_gridDim, bounds);
  memcpy(&lines[(s_nLinesGlobal + s_nLinesPoint + s_nLinesLine + 3) * 3 * 2], vLines.data(), vLines.size() * sizeof(float));

  glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * nLines * 3 * 2), lines, GL_STATIC_DRAW);
  
  GLuint lnPosition = glGetAttribLocation(m_ln_shaderProgram, "position");
  glVertexAttribPointer(lnPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(lnPosition);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  //--------------------------------------------------------------------------------
  //  Other
  //--------------------------------------------------------------------------------

  glEnable(GL_ALPHA_TEST);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_POINT_SPRITE);
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  glDepthFunc(GL_LESS);

  return true;
}


void Renderer::Update(Dg::ParticleData<float> * a_parData)
{
  glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);

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


static int GetAlphaEnumFromListVal(int a_val)
{
  switch (a_val)
  {
  case 0: return GL_ZERO;
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
  return GL_ONE;
}


void Renderer::Render(Dg::R3::Matrix<float> const & a_modelView
                    , Dg::R3::Matrix<float> const & a_proj
                    , float a_parScale
                    , std::vector<LineRenderData> const & a_lineRenderData)
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glBindVertexArray(m_ln_vao);
  glUseProgram(m_ln_shaderProgram);

  GLuint proj_loc = glGetUniformLocation(m_ln_shaderProgram, "proj_matrix");
  GLuint lineColor_loc = glGetUniformLocation(m_ln_shaderProgram, "lineColor");
  GLuint mv_loc = glGetUniformLocation(m_ln_shaderProgram, "mv_matrix");

  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, a_proj.GetData());
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (int i = 0; i < (int)a_lineRenderData.size(); ++i)
  {
    glUniformMatrix4fv(mv_loc, 1, GL_FALSE, (a_lineRenderData[i].mat * a_modelView).GetData());
    glUniform4fv(lineColor_loc, 1, a_lineRenderData[i].col.GetData());
    switch (a_lineRenderData[i].model)
    {
    case E_AttGlobal:
    {
      glDrawArrays(GL_LINES, 0, 3 * 2);
      break;
    }
    case E_AttPoint:
    {
      glDrawArrays(GL_LINES, s_nLinesGlobal * 2, 3 * 2);
      break;
    }
    case E_AttLine:
    {
      glDrawArrays(GL_LINES, (s_nLinesGlobal + s_nLinesPoint) * 2, 3 * 2);
      break;
    }
    case E_AttPlane:
    {
      glDrawArrays(GL_LINES, (s_nLinesGlobal + s_nLinesPoint + s_nLinesLine) * 2, s_nLinesPlane * 2);
      break;
    }
    }
  }
  glBindVertexArray(0);

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

    glBlendFunc(GetAlphaEnumFromListVal(sfactor_ind), GetAlphaEnumFromListVal(dfactor_ind));
  }

  glBindVertexArray(m_pt_vao);
  glUseProgram(m_pt_shaderProgram);

  mv_loc = glGetUniformLocation(m_pt_shaderProgram, "mv_matrix");
  proj_loc = glGetUniformLocation(m_pt_shaderProgram, "proj_matrix");
  GLint parScale_loc = glGetUniformLocation(m_pt_shaderProgram, "parScale");

  glUniformMatrix4fv(mv_loc, 1, GL_FALSE, a_modelView.GetData());
  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, a_proj.GetData());
  glUniform1f(parScale_loc, a_parScale);

  glDrawArrays(GL_POINTS, 0, m_nCurrentParticles);

  glBindVertexArray(0);
}

void Renderer::ShutDown()
{
  for (int i = 0; i < s_nBuffers; ++i)
  {
    if (m_buffers[i] != 0)
    {
      glDeleteBuffers(1, &m_buffers[i]);
      m_buffers[i] = 0;
    }
  }

  glDeleteVertexArrays(1, &m_pt_vao);
  glDeleteVertexArrays(1, &m_ln_vao);
  glDeleteProgram(m_pt_shaderProgram);
  glDeleteProgram(m_ln_shaderProgram);
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

GLuint Renderer::CompileShaders(char const * a_vs, char const * a_fs)
{
  GLuint vs = LoadShaderFromFile(a_vs, GL_VERTEX_SHADER);

  //Check for errors
  if (vs == 0)
    return 0;

  GLuint fs = LoadShaderFromFile(a_fs, GL_FRAGMENT_SHADER);

  //Check for errors
  if (fs == 0)
    return 0;

  //Create program, attach shaders to it and link it.
  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  //Detach and delete shaders afer we have successfully linked the program.
  glDetachShader(program, vs);
  glDetachShader(program, fs);
  glDeleteShader(vs);
  glDeleteShader(fs);
  return program;
}