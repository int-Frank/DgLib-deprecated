#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct LineRenderData;

namespace Dg
{
  template<typename Real>
  class ParticleData;

  template<typename Real>
  class Matrix44;
}

class Renderer
{
public:
  Renderer(){}

  bool Init(Dg::ParticleData<float> *);

  void Update(Dg::ParticleData<float> *);

  void Render(Dg::Matrix44<float> const & a_modelView
            , Dg::Matrix44<float> const & a_proj
            , float parScale
            , int nlineModels
            , LineRenderData const *);

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
