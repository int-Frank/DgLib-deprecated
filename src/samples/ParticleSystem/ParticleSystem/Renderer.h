#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
  Renderer()
    : m_pAttractorTypes(nullptr)
    , m_nCurrentAttractors(0)
  {}

  bool Init(Dg::ParticleData<float> *);
  void Update(Dg::ParticleData<float> *
            , int nAttractors
            , int * attractorTypes);
  void Render(Dg::Matrix44<float> const & a_modelView
            , Dg::Matrix44<float> const & a_proj
            , float parScale);
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
  int                 m_nCurrentAttractors;
  int *               m_pAttractorTypes;

private:
  //Drawing lines...
  static float const  s_lineColor[4];
  static int const    s_nLinesGlobal = 3;
  static int const    s_nLinesPoint = 3;
  static int const    s_nLinesLine = 3;
  static int const    s_gridDim = 10;
  static int const    s_nLinesPlane = 3 + (s_gridDim + 1) * 2;
};

#endif
