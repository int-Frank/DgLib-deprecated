
#include <fstream>

#include "Renderer.h"
#include "particle_system/DgParticleData.h"
#include "DgMatrix44.h"
#include "DgMakeGrid.h"
#include "Types.h"
#include "UI.h"


bool Renderer::Init()
{
  m_shaderProgram = CompileShaders("vs.glsl", "fs.glsl");
  glGenBuffers(2, m_buffer);

  //Verts
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  float const tri[12] =
  {
    -1.0f,  -1.0f,   0.0f,
    1.0f,   -1.0f,   0.0f,
    0.0f,    1.0f,   0.0f,
  };

  GLushort inds[3] =
  {
    0, 1, 2
  };

  glBindBuffer(GL_ARRAY_BUFFER, m_buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);

  GLuint vPosition = glGetAttribLocation(m_shaderProgram, "position");
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vPosition);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return true;
}


void Renderer::Update()
{
}


void Renderer::Render()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glBindVertexArray(m_vao);
  glUseProgram(m_shaderProgram);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
  glBindVertexArray(0);
}

void Renderer::ShutDown()
{
  for (int i = 0; i < 2; ++i)
  {
    if (m_buffer[i] != 0)
    {
      glDeleteBuffers(1, &m_buffer[i]);
      m_buffer[i] = 0;
    }
  }

  glDeleteVertexArrays(1, &m_vao);
  glDeleteProgram(m_shaderProgram);
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