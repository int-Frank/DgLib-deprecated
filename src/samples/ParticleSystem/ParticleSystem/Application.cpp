/*!
* @file Application.cpp
*
* @author Frank Hart
* @date 30/01/2014
*
* [description]
*/



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Application.h"
#include <string>
#include <cstring>
#include "DgMatrix44.h"
#include <math.h>

#include "DgParser_INI.h"
#include "DgStringFunctions.h"

Application* Application::s_app(nullptr);

double Application::s_mouseX(0.0);
double Application::s_mouseY(0.0);
double Application::s_prevX(0.0);
double Application::s_prevY(0.0);

typedef Dg::Matrix44<float> mat44;
typedef Dg::Vector4<float>  vec4;



/*
Pre Condition: -None
Post Condition:
-Returns the ID of a compiled shader of the specified
type from the specified file
-Reports error to console if file could not be found or compiled
Side Effects:
-None
*/
GLuint Application::LoadShaderFromFile(std::string path, GLenum shaderType)
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

GLuint Application::CompileShaders()
{
  GLuint vs = LoadShaderFromFile("vs.glsl", GL_VERTEX_SHADER);

  //Check for errors
  if (vs == 0)
    return 0;

  GLuint fs = LoadShaderFromFile("fs.glsl", GL_FRAGMENT_SHADER);

  //Check for errors
  if (fs == 0)
    return 0;

  //GLuint gs = LoadShaderFromFile("test_gs.glsl", GL_GEOMETRY_SHADER);

  ////Check for errors
  //if (gs == 0)
  //return 0;

  //Create program, attach shaders to it and link it.
  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  //glAttachShader(program, gs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  return program;
}



bool Application::Init()
{
  GetConfiguration();
  if (!InitGL())
  {
    return false;
  }
  InitControls();
  InitParticleSystem();
  return true;
}

bool Application::InitGL()
{
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_info.majorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_info.minorVersion);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, m_info.samples);

  if (m_info.flags.fullscreen)
  {
    if (m_info.windowWidth == 0 || m_info.windowHeight == 0)
    {
      const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

      m_info.windowWidth = mode->width;
      m_info.windowHeight = mode->height;
    }
    m_window = glfwCreateWindow(m_info.windowWidth,
      m_info.windowHeight,
      m_info.title,
      glfwGetPrimaryMonitor(),
      nullptr);
    glfwSwapInterval((int)m_info.flags.vsync);
  }
  else
  {
    m_window = glfwCreateWindow(m_info.windowWidth,
      m_info.windowHeight,
      m_info.title,
      nullptr,
      nullptr);
  }

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

void Application::InitControls()
{
  glfwSetKeyCallback(m_window, OnKey);
  glfwSetCursorPosCallback(m_window, OnMouseMove);
}


void Application::Shutdown()
{
  glDeleteVertexArrays(1, &m_vao);
  glDeleteProgram(m_renderingProgram);
  glDeleteVertexArrays(1, &m_vao);
}


void Application::Render(double currentTime)
{
  //Generate the cube transformation matrix
  float f = float(currentTime);

  mat44 translate, rotate, scale;
  translate.Translation(vec4(0.0f, 0.0f, -2.0f, 0.0f));
  rotate.Rotation(0.0f,
                  float(s_mouseX) / 100.0f,
                  float(s_mouseY) / 100.0f,
                  Dg::EulerOrder::XYZ);
  scale.Scaling(1.0f);
  mat44 mv_matrix = scale * rotate * translate;

  //printf("%d, %d\n", mouseX, mouseY);
  //Set up the viewport
  float ratio;
  int width, height;

  glfwGetFramebufferSize(m_window, &width, &height);
  ratio = width / (float)height;

  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT);

  //Set up the perspective matrix;
  mat44 proj_matrix;
  proj_matrix.Perspective(1.5f, ratio, 0.1f, 1000.0f);

  glUseProgram(m_renderingProgram);

  GLint mv_loc = glGetUniformLocation(m_renderingProgram, "mv_matrix");
  GLint proj_loc = glGetUniformLocation(m_renderingProgram, "proj_matrix");

  glUniformMatrix4fv(mv_loc, 1, GL_FALSE, mv_matrix.GetData());
  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj_matrix.GetData());

  //Clear the depth buffer
  glClear(GL_DEPTH_BUFFER_BIT);

  //Draw the triangle 
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

}


void Application::OnKey(GLFWwindow* m_window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(m_window, GL_TRUE);
}


void Application::OnMouseMove(GLFWwindow* m_window, double x, double y)
{
  s_mouseX = x;
  s_mouseY = y;

}

void Application::GetConfiguration()
{
  //Set defaults
  strcpy_s(m_info.title, "Dg Paricle System Example");
  m_info.windowWidth = 800;
  m_info.windowHeight = 600;
  m_info.majorVersion = 4;
  m_info.minorVersion = 3;
  m_info.samples = 0;
  m_info.flags.all = 0;
  m_info.flags.fullscreen = 0;

  Dg::Parser_INI parser;
  Dg::ErrorCode result = parser.Parse(m_configFileName);

  if (result == Dg::ErrorCode::FailedToOpenFile)
  {
    fprintf(stderr, "Failed to open config file '%s'. Using defaults...\n", m_configFileName);
    return;
  }
  else if (result != Dg::ErrorCode::None)
  {
    fprintf(stderr, "Failed trying to parse config file '%s'. Using defaults...\n", m_configFileName);
    return;
  }

  for (int i = 0; i < parser.GetItems().size(); ++i)
  {
    if (parser.GetItems().query_key(i) == "windowWidth")
    {
      int val = 0;
      if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
      {
        m_info.windowWidth = val;
      }
    }
    else if (parser.GetItems()[i] == "windowHeight")
    {
      int val = 0;
      if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
      {
        m_info.windowHeight = val;
      }
    }
    else if (parser.GetItems()[i] == "glMajorVersion")
    {
      int val = 0;
      if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
      {
        m_info.majorVersion = val;
      }
    }
    else if (parser.GetItems()[i] == "glMinorVersion")
    {
      int val = 0;
      if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
      {
        m_info.minorVersion = val;
      }
    }
    else if (parser.GetItems()[i] == "glSamples")
    {
      int val = 0;
      if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
      {
        m_info.samples = val;
      }
    }
    else if (parser.GetItems()[i] == "fullscreen")
    {
      int val = 0;
      if (Dg::StringToNumber(val, parser.GetItems()[i], std::dec))
      {
        m_info.flags.fullscreen = val;
      }
    }
  }
}

void Application::Run(Application* the_app)
{
  bool running = true;
  s_app = the_app;

  if (!Init())
  {
    fprintf(stderr, "Failed to initialize Application\n");
    return;
  }

  //This should go in the Rendere 
  m_renderingProgram = CompileShaders();
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  ////Use this for inverse depth buffer
  //glClearDepth(0.0f);
  //glDepthFunc(GL_GREATER);


  //Set up the cube geometry
  const GLfloat v_pos[] = {
    0.5, -0.5, -0.5,
    0.5, -0.5, 0.5,
    -0.5, -0.5, 0.5,
    -0.5, -0.5, -0.5,
    0.5, 0.5, -0.5,
    0.5, 0.5, 0.5,
    -0.5, 0.5, 0.5,
    -0.5, 0.5, -0.5
  };

  const GLushort v_ind[] = {
    0, 1, 2,
    0, 2, 3,
    4, 7, 6,
    4, 6, 5,
    0, 4, 5,
    0, 5, 1,
    1, 5, 6,
    1, 6, 2,
    2, 6, 7,
    2, 7, 3,
    4, 0, 3,
    4, 3, 7
  };

  //Generate some data and put it in a buffer object
  glGenBuffers(1, &m_posBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
  glBufferData(GL_ARRAY_BUFFER,
    sizeof(v_pos),
    v_pos,
    GL_STATIC_DRAW);

  //Set the vertex attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);

  //Add index data
  glGenBuffers(1, &m_indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    sizeof(v_ind),
    v_ind,
    GL_STATIC_DRAW);

  //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  do
  {
    /*
    //This section demonstrates getting mouse input.
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);

    double wc = double(width) / 2.0;
    double hc = double(height) / 2.0;

    double mouse_x, mouse_y;
    glfwGetCursorPos(m_window, &mouse_x, &mouse_y);
    glfwSetCursorPos(m_window, wc, hc);*/

    Render(glfwGetTime());

    glfwSwapBuffers(m_window);
    glfwPollEvents();

  } while (!glfwWindowShouldClose(m_window));

  Shutdown();

  glfwDestroyWindow(m_window);
  glfwTerminate();
}
