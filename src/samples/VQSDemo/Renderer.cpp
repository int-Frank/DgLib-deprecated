
#include <fstream>

#include "Renderer.h"
#include "Types.h"
#include "UI.h"

//#include "Segment.inl"

bool Renderer::Init()
{
  glGenBuffers(3, m_buffer);

  std::vector<float> v, vn;
  std::vector<GLushort> fv;
  if (!LoadData(v, vn, fv))
  {
    return false;
  }
  m_nFvs = fv.size();

  //Verts
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  m_shaderProgram = CompileShaders("vs.glsl", "fs.glsl");

  //Vertices
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), v.data(), GL_STATIC_DRAW);

  GLuint vPosition = glGetAttribLocation(m_shaderProgram, "position");
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vPosition);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, fv.size() * sizeof(GLushort), fv.data(), GL_STATIC_DRAW);

  //Normals
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer[2]);
  glBufferData(GL_ARRAY_BUFFER, vn.size() * sizeof(float), vn.data(), GL_STATIC_DRAW);

  GLuint vNormal = glGetAttribLocation(m_shaderProgram, "normal");
  glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vNormal);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  return true;
}


void Renderer::Update()
{
}


void Renderer::Render(Dg::Matrix44<float> const & a_proj
                    , Dg::Matrix44<float> const * a_pMV
                    , int a_nObjects)
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glBindVertexArray(m_vao);
  glUseProgram(m_shaderProgram);

  GLuint proj_loc = glGetUniformLocation(m_shaderProgram, "proj_matrix");
  GLuint mv_loc = glGetUniformLocation(m_shaderProgram, "mv_matrix");

  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, a_proj.GetData());

  for (int i = 0; i < a_nObjects; ++i)
  {
    glUniformMatrix4fv(mv_loc, 1, GL_FALSE, a_pMV[i].GetData());
	  glDrawElements(GL_TRIANGLES, m_nFvs, GL_UNSIGNED_SHORT, 0);
  }

  glBindVertexArray(0);
}

void Renderer::ShutDown()
{
  glDeleteBuffers(3, m_buffer);

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