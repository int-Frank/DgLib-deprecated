#ifndef MOD_RENDERER_CONTEXTTRIANGLE_H
#define MOD_RENDERER_CONTEXTTRIANGLE_H

#include <vector>

#include "Mod_Renderer_Context.h"
#include "DgR3Matrix.h"

namespace Renderer
{
  class ContextTriangle : public Context
  {
  public:

    ContextTriangle();
    ~ContextTriangle();

    //Assumes this context is currently active
    void LoadData(std::vector<TriangleMesh> const &);
    void Draw(int);
    void Clear();
    void SetMatrix(Dg::R3::Matrix<float> const &);
    void SetColor(Dg::R3::Vector<float> const &);
    void Bind();
    void Unbind();

  private:

    class PIMPL;
    PIMPL * m_pimpl;
  };
}

#endif