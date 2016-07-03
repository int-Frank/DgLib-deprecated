#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Dg
{
  template<typename Real>
  class ParticleData;
}

class Renderer
{
public:
  Renderer()
    : m_pParticleData(nullptr) {}

  bool Init(Dg::ParticleData<float> const *);
  void Update();
  void Render();
  void ShutDown();

private:
  GLuint CompileShaders();
  GLuint LoadShaderFromFile(std::string, GLenum shaderType);

private:
  Dg::ParticleData<float> * m_pParticleData;

  GLFWwindow*         m_window;
  GLuint              m_renderingProgram;
  GLuint              m_vao;
  GLuint              m_posBuffer;
  GLuint              m_indexBuffer;
};

#endif
