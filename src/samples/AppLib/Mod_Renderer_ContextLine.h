#ifndef MOD_RENDERER_CONTEXTLINE_H
#define MOD_RENDERER_CONTEXTLINE_H

#include "Mod_Renderer_Context.h"

namespace Renderer
{
  class ContextLine : public Context
  {
  public:

    ContextLine();
    ~ContextLine();

    void LoadData(std::vector<LineMesh> const &);
    void Draw(int);
    void Clear();
    void SetMatrix(Dg::R3::Matrix<float> const &);
    void MakeCurrent();

  private:

    class PIMPL;
    PIMPL * m_pimpl;
  };
}

#endif