#ifndef MESH_H
#define MESH_H

#include <string>

#include "Types.h"
#include <vector>

#include "Types.h"

//TODO Dow we even need this? all we need is a vector of triangles.
//TODO Do NOT need a separate triangles. 
class Mesh
{
  typedef Dg::Matrix<1, 3, unsigned short> Face;

public:

  bool LoadOBJ(std::string const &);
  void GetData(std::vector<float> & vertices,
               std::vector<float> & normals,
               std::vector<unsigned short> & faces) const;
  size_t NumberTriangles() const { return m_faces.size(); }

  void Clear();

private:

  std::vector<vec4> m_points;
  std::vector<vec4> m_normals;
  std::vector<Face> m_faces;
};

#endif