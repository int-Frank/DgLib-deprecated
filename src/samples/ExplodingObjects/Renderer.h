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
  struct ObjectData
  {
    GLsizei faceCount;
    GLint   faceOffset;
    GLint   vertexOffset;
  };

public:
  Renderer()
    : m_vao(0)
    , m_vBuf(0)
    , m_fBuf(0)
    , m_shaderProgram(0)
  {}

  bool Init();
  bool SetModels(std::vector<Mesh> const &);
  void Clear();

  void Begin();
  void End();
  void Render(mat44 const &, mat44 const &, SceneObject const &);
  void ShutDown();

private:

  bool CollateData(std::vector<Mesh> const &,
                   std::vector<float> &,
                   std::vector<unsigned short> &);

  GLuint CompileShaders(char const * vs, char const * fs);
  GLuint LoadShaderFromFile(std::string, GLenum shaderType);

private:

  std::vector<ObjectData> m_objectData;
  GLuint                  m_vao;
  GLuint                  m_vBuf; // Interleave vertices and normals.
  GLuint                  m_fBuf; // Pack all objects into one face buffer.
  GLuint                  m_shaderProgram;
};

#endif
