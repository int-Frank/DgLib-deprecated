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
  Renderer(){}

  bool Init(Dg::ParticleData<float> *);
  void Update(Dg::ParticleData<float> *);
  void Render(Dg::Matrix44<float> const & a_modelView
            , Dg::Matrix44<float> const & a_proj);
  void ShutDown();

private:
  GLuint CompileShaders();
  GLuint LoadShaderFromFile(std::string, GLenum shaderType);

private:
  GLuint              m_vao;
  GLuint              m_idBufffer;
  GLuint              m_idShaderProgram;
  int                 m_nCurrentParticles;
};

#endif
