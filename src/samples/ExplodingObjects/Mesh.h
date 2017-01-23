#ifndef MESH_H
#define MESH_H

#include <string>

#include "Types.h"
#include "DgMeshBase.h"

struct _NO_DATA
{
};

struct vData
{
  vec4 point;
  vec4 normal;
};

class Mesh : public Dg::MeshBase<vData, _NO_DATA, _NO_DATA>
{
public:

  bool LoadOBJ(std::string const &);
  void CollateData(std::vector<float> & vertices,
                   std::vector<float> & normals,
                   std::vector<unsigned short> & faces) const;

  seg Segment(Dg::eHandle) const;
  triangle Triangle(Dg::fHandle) const;
  vec4 Centroid() const;
  sphere Sphere() const;
};

#endif