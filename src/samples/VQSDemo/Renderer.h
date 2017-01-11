#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Types.h"
#include "DgR3Matrix.h"

class Renderer
{
public:
  Renderer(){}

  bool Init();
  void Update();
  void Render(Dg::R3::Matrix<float> const & a_proj
            , Dg::R3::Matrix<float> const * a_pMV
            , int a_nObjects);
  void ShutDown();

private:
  GLuint CompileShaders(char const * vs, char const * fs);
  GLuint LoadShaderFromFile(std::string, GLenum shaderType);
  bool LoadData(std::vector<float> & a_v
              , std::vector<float> & a_vn
              , std::vector<GLushort> & a_fv);

private:
  int                 m_nFvs;
  GLuint              m_vao;
  GLuint              m_buffer[3];
  GLuint              m_shaderProgram;
};

#endif
