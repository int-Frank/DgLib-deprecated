
#include <fstream>

#include "Renderer.h"
#include "particle_system/DgParticleData.h"
#include "Types.h"
#include "UI.h"

bool Renderer::Init()
{
  m_shaderProgram = CompileShaders("vs.glsl", "fs.glsl");
  
  return true;
}


bool Renderer::SetMesh(std::vector<Facet> const & a_facets)
{
  Clear();

  /*
    Vertex buffers are interleaved as follows:
    positin/normal/transform index/positin/normal/transform index/...
  */

  glGenBuffers(1, &m_vBuf);
  glGenBuffers(1, &m_fBuf);

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  std::vector<VertexData> vertexData;
  std::vector<unsigned short> faceData;

  CollateData(a_facets, vertexData, faceData);
  m_nFaces = GLsizei(a_facets.size()) * 3;

  //Verts
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  //Vertices
  glBindBuffer(GL_ARRAY_BUFFER, m_vBuf);
  glBufferData(GL_ARRAY_BUFFER, 
               vertexData.size() * sizeof(VertexData), 
               vertexData.data(), GL_STATIC_DRAW);

  GLuint vPosition = glGetAttribLocation(m_shaderProgram, "position");
  GLuint vNormal = glGetAttribLocation(m_shaderProgram, "normal");
  GLuint vIndex = glGetAttribLocation(m_shaderProgram, "index");

  int stride = sizeof(VertexData);

  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, stride, 0);
  glVertexAttribPointer(vNormal,   4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 4));
  glVertexAttribIPointer(vIndex,   1, GL_INT, stride, (void*)(sizeof(float) * 8));
  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vNormal);
  glEnableVertexAttribArray(vIndex);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_fBuf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
               faceData.size() * sizeof(GLushort),
               faceData.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Renderer::CollateData(std::vector<Facet> const & a_facets,
                           std::vector<VertexData> & a_vertexData,
                           std::vector<unsigned short> & a_faceData)
{
  unsigned short index = 0;
  for (auto const & f : a_facets)
  {
    for (int i = 0; i < 3; ++i, ++index)
    {
      a_faceData.push_back(index);

      VertexData vd;
      vd.point = f.points[i];
      vd.normal = f.normals[i];
      vd.index = int(index / 3);
      a_vertexData.push_back(vd);
    }
  }
}

void  Renderer::SetTransformData(std::vector<TransformData> const & a_data)
{
  if (m_ssboBuf != GL_INVALID_VALUE)
  {
    glDeleteBuffers(1, &m_ssboBuf);
    m_ssboBuf = GL_INVALID_VALUE;
  }

  glGenBuffers(1, &m_ssboBuf);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssboBuf);
  GLuint vPD = glGetProgramResourceIndex(m_shaderProgram, GL_SHADER_STORAGE_BLOCK, "physics_data");
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, vPD, m_ssboBuf);
  glBufferData(GL_SHADER_STORAGE_BUFFER, a_data.size() * sizeof(TransformData), (void*)a_data.data(), GL_DYNAMIC_COPY);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Renderer::SetTime(float a_time)
{
  GLuint time_loc = glGetUniformLocation(m_shaderProgram, "totalTime");
  glUniform1f(time_loc, a_time);
}

void Renderer::Begin()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glBindVertexArray(m_vao);
  glUseProgram(m_shaderProgram);
}

void Renderer::End()
{
  glBindVertexArray(0);
}

void Renderer::Render(mat44 const & a_T_S_V,
                      mat44 const & a_T_V_W,
                      mat44 const & a_T_W_M)
{
  GLuint T_S_V_loc = glGetUniformLocation(m_shaderProgram, "T_S_V");
  GLuint T_V_W_loc = glGetUniformLocation(m_shaderProgram, "T_V_W");
  GLuint T_W_M_loc = glGetUniformLocation(m_shaderProgram, "T_W_M");

  glUniformMatrix4fv(T_S_V_loc, 1, GL_FALSE, a_T_S_V.GetData());
  glUniformMatrix4fv(T_V_W_loc, 1, GL_FALSE, a_T_V_W.GetData());
  glUniformMatrix4fv(T_W_M_loc, 1, GL_FALSE, a_T_W_M.GetData());

  glDrawElements(GL_TRIANGLES, m_nFaces, GL_UNSIGNED_SHORT, 0);
}

void Renderer::Clear()
{
  if (m_vBuf != GL_INVALID_VALUE)
  {
    glDeleteBuffers(1, &m_vBuf);
    m_vBuf = GL_INVALID_VALUE;
  }

  if (m_fBuf != GL_INVALID_VALUE)
  {
    glDeleteBuffers(1, &m_fBuf);
    m_fBuf = GL_INVALID_VALUE;
  }

  if (m_ssboBuf != GL_INVALID_VALUE)
  {
    glDeleteBuffers(1, &m_ssboBuf);
    m_ssboBuf = GL_INVALID_VALUE;
  }

  if (m_vao != GL_INVALID_VALUE)
  {
    glDeleteVertexArrays(1, &m_vao);
    m_vao = GL_INVALID_VALUE;
  }
}

void Renderer::ShutDown()
{
  Clear();

  if (m_shaderProgram)
  {
    glDeleteProgram(m_shaderProgram);
    m_shaderProgram = 0;
  }
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