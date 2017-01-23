#ifndef MESHFACESELECTOR_H
#define MESHFACESELECTOR_H

#include "DgMeshBase.h"

class MeshFaceSelector
{
public:

  virtual ~MeshFaceSelector() {}
  virtual Dg::fHandle operator()() { return Dg::fHandle(); }
};

#endif