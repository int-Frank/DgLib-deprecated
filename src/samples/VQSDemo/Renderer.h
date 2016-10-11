#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Types.h"
#include "DgMatrix44.h"

class Renderer
{
public:
  Renderer(){}

  bool Init();
  void Update();
  void Render(Dg::Matrix44<float> const & a_proj
            , Dg::Matrix44<float> const * a_pMV
            , int a_nObjects);
  void ShutDown();

private:
  GLuint CompileShaders(char const * vs, char const * fs);
  GLuint LoadShaderFromFile(std::string, GLenum shaderType);

private:
  GLuint              m_vao;
  GLuint              m_buffer[3];
  GLuint              m_shaderProgram;
};

#endif
