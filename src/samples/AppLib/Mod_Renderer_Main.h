#ifndef MOD_RENDERER_MAIN_H
#define MOD_RENDERER_MAIN_H

#include <string>
#include <stdint.h>

#include "Mod_Renderer_TriangleMesh.h"
#include "Mod_Renderer_LineMesh.h"
#include "DgR3Matrix.h"

namespace Renderer
{
  void Init(void(*Log)(std::string const &, int));

  typedef uint32_t ObjectHandle;
  ObjectHandle const InvalidHandle = 0xFFFFFFFF;

  enum Contexts
  {
    E_Lines = 0,
    E_Triangles,
    E_Total
  };

  class Renderer
  {
  public:

    Renderer();

    ~Renderer();
    Renderer(Renderer const &) = delete;
    Renderer & operator=(Renderer const &) = delete;

    void Clear();

    void ClearLoadList();
    ObjectHandle  AddObject(TriangleMesh const &);
    ObjectHandle  AddObject(LineMesh const &);
    void CommitLoadList();

    void SetTransform(Dg::R3::Matrix<float> const &);
    void SetColor(Dg::R3::Vector<float> const &);
    void SetRenderTarget();

    void BeginDraw();
    void SetContext(Contexts);
    void Draw(ObjectHandle);
    void EndDraw();

  private:

    class PIMPL;
    PIMPL * m_pimpl;
  };

}

#endif