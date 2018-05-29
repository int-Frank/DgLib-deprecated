#ifndef MOD_RENDERER_CONTEXT_H
#define MOD_RENDERER_CONTEXT_H

#include <vector>

#include "Mod_Renderer_LineMesh.h"
#include "Mod_Renderer_TriangleMesh.h"
#include "DgR3Matrix.h"
#include "DgR3Vector.h"

namespace Renderer
{
  class Context
  {
  public:

    virtual ~Context() {}
    virtual void LoadData(std::vector<LineMesh> const &);
    virtual void LoadData(std::vector<TriangleMesh> const &);
    virtual void Draw(int) {}
    virtual void Clear() {}
    virtual void SetMatrix(Dg::R3::Matrix<float> const &) {}
    virtual void SetColor(Dg::R3::Vector<float> const &) {}
    virtual void ActivateContext() {}
    virtual void DeactivateContext() {}

  };
}

#endif