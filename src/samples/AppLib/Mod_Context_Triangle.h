#ifndef MOD_CONTEXT_TRIANGLE_H
#define MOD_CONTEXT_TRIANGLE_H

#include <vector>

#include "Mod_Context.h"
#include "DgR3Matrix.h"
#include "DgR3Vector.h"

namespace Context
{
  struct TriangleVertex
  {
    Dg::R3::Vector<float> point;
  };

  struct Triangle
  {
    int indices[3];
  };

  class TriangleMesh
  {
  public:

    TriangleMesh()
      : color(1.0f, 1.0f, 1.0f, 1.0f)
      , depthEnabled(true)
    {}

    std::vector<TriangleVertex>   verts;
    std::vector<Triangle>         triangles;
    Dg::R3::Vector<float>         color;
    bool                          depthEnabled;
  };

  class ContextTriangle : public Context
  {
  public:

    ContextTriangle();
    ~ContextTriangle();

    void ClearLoadList();
    ObjectHandle AddObject(TriangleMesh const &);
    void CommitLoadList();

    void Bind();
    void Draw(ObjectHandle);
    void Unbind();

    void Clear();
    void SetMatrix(Dg::R3::Matrix<float> const &);
    void SetColor(ObjectHandle, Dg::R3::Vector<float> const &);

  private:

    class PIMPL;
    PIMPL * m_pimpl;
  };
}

#endif