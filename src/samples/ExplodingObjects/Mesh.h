#ifndef MESH_H
#define MESH_H

#include <string>

#include "Types.h"
#include <vector>

#include "Types.h"

struct Facet
{
  vec4 points[3];
  vec4 normals[3];
};

class Mesh
{
public:
  bool LoadOBJ(std::string const &);
  size_t NumberFacets() const { return m_facets.size(); }

  std::vector<Facet> const & Facets() const { return m_facets; }

  void Clear();

private:

  std::vector<Facet> m_facets;
};

#endif