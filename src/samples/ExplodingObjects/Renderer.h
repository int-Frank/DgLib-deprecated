#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Types.h"
#include "Mesh.h"

class SceneObject;

class Renderer
{
public:

  struct TransformData
  {
    vec4 translation;
    vec4 rotation;
  };

public:
  Renderer()
    : m_nFaces(0)
    , m_vao(GL_INVALID_VALUE)
    , m_ssboBuf(GL_INVALID_VALUE)
    , m_vBuf(GL_INVALID_VALUE)
    , m_fBuf(GL_INVALID_VALUE)
    , m_shaderProgram(GL_INVALID_VALUE)
  {}

  bool Init();
  bool SetMesh(Mesh const &);
  void SetTransformData(std::vector<TransformData> const &);
  void SetTime(float);
  void Clear();

  void Begin();
  void End();
  void Render(mat44 const &, 
              mat44 const &,
              mat44 const &);
  void ShutDown();

private:

  void CollateData(Mesh const &,
                   std::vector<char> &,
                   std::vector<unsigned short> &);

  GLuint CompileShaders(char const * vs, char const * fs);
  GLuint LoadShaderFromFile(std::string, GLenum shaderType);

private:

  GLsizei                 m_nFaces;
  GLuint                  m_vao;
  GLuint                  m_ssboBuf;
  GLuint                  m_vBuf; // Interleave vertices and normals.
  GLuint                  m_fBuf; // Pack all objects into one face buffer.
  GLuint                  m_shaderProgram;
};

#endif
