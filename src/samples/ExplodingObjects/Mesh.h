#ifndef MODELBASE_H
#define MODELBASE_H

#include <vector>
#include <string>

#include "Types.h"

struct Face
{
  int verts[3];
};

class Mesh
{
public:

  Mesh() {}
  ~Mesh() {}

  Mesh(Mesh const &);
  Mesh & operator=(Mesh const &);

  void Clear();
  bool Load(std::string const &);

  void GetData(std::vector<float> & vertices,
               std::vector<float> & normals,
               std::vector<unsigned short> & faces) const;

  int NumberFaces() const;

  void RecenterData(vec4 const &);

  //Centers and scales data
  void NormalizeData(vec4 const &, float);

private:

  std::vector<vec4>   m_vertices;
  std::vector<vec4>   m_normals;
  std::vector<Face>   m_faces;

};

#endif