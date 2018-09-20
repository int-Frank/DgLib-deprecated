#ifndef MOD_RENDERER_CONTEXT_H
#define MOD_RENDERER_CONTEXT_H

#include <vector>

#include "DgR3Matrix.h"
#include "DgR3Vector.h"

namespace Context
{
  void Init(void(*Log)(std::string const &, int));

  typedef uint32_t ObjectHandle;
  ObjectHandle const InvalidHandle = 0xFFFFFFFF;

  class Context
  {
  public:

    virtual ~Context() {}

    //Class specific methods
    virtual void ClearLoadList() {}
    virtual void CommitLoadList() {}

    //Call Bind() before drawing objects and Unbind() after
    virtual void Bind() {}
    virtual void Draw(ObjectHandle) {}
    virtual void Unbind() {}

    virtual void Clear() {}
    virtual void SetMatrix(Dg::R3::Matrix<float> const &) {}
    virtual void SetColor(ObjectHandle, Dg::R3::Vector<float> const &) {}
  };
}

#endif