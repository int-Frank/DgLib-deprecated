#ifndef MESH_H
#define MESH_H

#include <string>

#include "Types.h"
#include <vector>

#include "Types.h"

class Mesh
{
  typedef Dg::Matrix<1, 3, unsigned short> Face;

public:
  bool LoadOBJ(std::string const &);
  size_t NumberTriangles() const { return m_faces.size(); }

  std::vector<vec4> const & Points() const { return m_points; }
  std::vector<vec4> const & Normals() const { return m_normals; }
  std::vector<Face> const & Faces() const { return m_faces; }

  void Clear();

private:

  std::vector<vec4> m_points;
  std::vector<vec4> m_normals;
  std::vector<Face> m_faces;
};

#endif