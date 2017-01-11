#ifndef RENDERER_H
#define RENDERER_H

#include "DgR3Matrix.h"

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Types.h"

namespace Dg
{
  template<typename Real>
  class ParticleData;
}

class Renderer
{
public:
  Renderer(){}

  bool Init(Dg::ParticleData<float> *);

  void Update(Dg::ParticleData<float> *);

  void Render(Dg::R3::Matrix<float> const & modelView
            , Dg::R3::Matrix<float> const & proj
            , float parScale
            , std::vector<LineRenderData> const & lineRenderData);

  void ShutDown();

private:
  GLuint CompileShaders(char const * vs, char const * fs);
  GLuint LoadShaderFromFile(std::string, GLenum shaderType);

private:
  GLuint              m_pt_vao;
  GLuint              m_ln_vao;
  static int const    s_nBuffers = 2;
  GLuint              m_buffers[s_nBuffers];
  GLuint              m_pt_shaderProgram;
  GLuint              m_ln_shaderProgram;
  int                 m_nCurrentParticles;

private:
  //Drawing lines...
  static int const    s_nLinesGlobal = 3;
  static int const    s_nLinesPoint = 3;
  static int const    s_nLinesLine = 3;
  static int const    s_gridDim = 8;
  static int const    s_nLinesPlane = 3 + (s_gridDim + 1) * 2;
};

#endif
