
#include <fstream>

#include "Renderer.h"
#include "particle_system/DgParticleData.h"
#include "Types.h"
#include "UI.h"

#include "SceneObject.h"

bool Renderer::Init()
{
  m_shaderProgram = CompileShaders("vs.glsl", "fs.glsl");
  
  return true;
}


bool Renderer::SetModels(std::vector<Mesh> const & a_models)
{
  Clear();

  glGenBuffers(1, &m_vBuf);
  glGenBuffers(1, &m_fBuf);

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_vBuf);

  std::vector<float> vertexData;
  std::vector<unsigned short> faceData;

  CollateData(a_models, vertexData, faceData);

  //Verts
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  //Vertices
  glBindBuffer(GL_ARRAY_BUFFER, m_vBuf);
  glBufferData(GL_ARRAY_BUFFER, 
               vertexData.size() * sizeof(float), 
               vertexData.data(), GL_STATIC_DRAW);

  GLuint vPosition = glGetAttribLocation(m_shaderProgram, "position");
  GLuint vNormal = glGetAttribLocation(m_shaderProgram, "normal");
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
  glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vNormal);

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

bool Renderer::CollateData(std::vector<Mesh> const & a_models,
                           std::vector<float> & a_vertexData,
                           std::vector<unsigned short> & a_faceData)
{
  m_objectData.clear();
  bool result = true;
  for (auto const & model : a_models)
  {
    std::vector<float> vertices, normals;

    ObjectData od;
    od.faceOffset = a_faceData.size() * sizeof(GLshort) * 3;
    od.vertexOffset = a_vertexData.size() / 3 / 2;

    size_t oldFaceDataSize = a_faceData.size();

    model.CollateData(vertices, normals, a_faceData);

    if (vertices.size() != normals.size())
    {
      result = false;
      break;
    }

    od.faceCount = (a_faceData.size() - oldFaceDataSize);
    m_objectData.push_back(od);

    for (size_t i = 0; i < vertices.size(); i += 3)
    {
      a_vertexData.push_back(vertices[i + 0]);
      a_vertexData.push_back(vertices[i + 1]);
      a_vertexData.push_back(vertices[i + 2]);
      a_vertexData.push_back(normals[i + 0]);
      a_vertexData.push_back(normals[i + 1]);
      a_vertexData.push_back(normals[i + 2]);
    }
  }
  return result;
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

void Renderer::Render(mat44 const & a_projection,
                      mat44 const & a_T_world_view,
                      SceneObject const & a_object)
{
  int id = a_object.GetModelReference();
  if (id < 0 || id >= m_objectData.size())
  {
    return;
  }

  GLuint proj_loc = glGetUniformLocation(m_shaderProgram, "proj_matrix");
  GLuint mv_loc = glGetUniformLocation(m_shaderProgram, "mv_matrix");

  mat44 T_model_view = a_T_world_view * a_object.GetMatrix();

  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, a_projection.GetData());
  glUniformMatrix4fv(mv_loc, 1, GL_FALSE, T_model_view.GetData());

  glDrawElementsBaseVertex(GL_TRIANGLES, 
                           m_objectData[id].faceCount,
                           GL_UNSIGNED_SHORT,
                           (void*)m_objectData[id].faceOffset,
                           m_objectData[id].vertexOffset);
}

void Renderer::Clear()
{
  if (m_vBuf)
  {
    glDeleteBuffers(1, &m_vBuf);
    m_vBuf = 0;
  }

  if (m_fBuf)
  {
    glDeleteBuffers(1, &m_fBuf);
    m_fBuf = 0;
  }

  if (m_vao)
  {
    glDeleteVertexArrays(1, &m_vao);
    m_vao = 0;
  }

  m_objectData.clear();
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