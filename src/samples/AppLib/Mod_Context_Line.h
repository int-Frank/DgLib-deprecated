#ifndef MOD_CONTEXT_LINE_H
#define MOD_CONTEXT_LINE_H


#include <vector>

#include "DgR3Vector.h"
#include "DgR3Matrix.h"
#include "Mod_Context.h"

namespace Context
{
  struct LineVertex
  {
    Dg::R3::Vector<float> point;
  };

  struct Line
  {
    unsigned indices[2];
  };

  class LineMesh
  {
  public:

    LineMesh()
      : color(1.0f, 1.0f, 1.0f, 1.0f)
      , thickness(1.0f)
      , depthEnabled(true)
    {}

    std::vector<LineVertex>   verts;
    std::vector<Line>     lines;
    Dg::R3::Vector<float> color;
    float                 thickness; //Not implemented
    bool                  depthEnabled;
  };

  class ContextLine : public Context
  {
  public:

    ContextLine();
    ~ContextLine();

    void ClearLoadList();
    ObjectHandle AddObject(LineMesh const &);
    void CommitLoadList();

    void Bind();
    void Draw(ObjectHandle);
    void Unbind();

    void Clear();
    void SetMatrix(Dg::R3::Matrix<float> const &);
    void SetColor(ObjectHandle, Dg::R3::Vector<float> const &);
    void SetThickness(ObjectHandle, float);

  private:

    class PIMPL;
    PIMPL * m_pimpl;
  };
}

#endif