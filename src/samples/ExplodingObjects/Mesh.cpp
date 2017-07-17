
#include <iostream>
#include <fstream>

#include "Mesh.h"
#include "DgR3AABB.h"

typedef Dg::R3::AABB<float> AABB;

static void NormalizeData(std::vector<vec4> & a_points)
{
  AABB aabb(a_points.begin(), a_points.end());
  vec4 offset(-aabb.GetCenter().x(), 
              -aabb.GetCenter().y(),
              -aabb.GetCenter().z(), 
              0.0f);
  for (auto & p : a_points)
  {
    p += offset;
  }

  vec4 diag(aabb.Diagonal());
  float max_hl = (diag[0] > diag[1]) ? diag[0] : diag[1];
  if (diag[2] > max_hl) max_hl = diag[2];

  float scaleFactor = 1.0f / max_hl;
  for (auto & p : a_points)
  {
    p.x() *= scaleFactor;
    p.y() *= scaleFactor;
    p.z() *= scaleFactor;
  }
}

bool Mesh::LoadOBJ(std::string const & a_fileName)
{
  Clear();
  std::ifstream ifs;
  ifs.open(a_fileName);

  if (!ifs.good())
  {
    std::cout << "Can't open data file: " << a_fileName;
    return false;
  }

  struct Face
  {
    unsigned short v[3];
    unsigned short vn[3];
  };

  std::string firstWord;
  std::vector<vec4> v_list;
  std::vector<vec4> vn_list;
  std::vector<Face> f_list;

  while (ifs >> firstWord)
  {
    if (firstWord == "v")
    {
      float vx = 0.0f;
      float vy = 0.0f;
      float vz = 0.0f;
      ifs >> vx >> vy >> vz;
      v_list.push_back(vec4(vx, vy, vz, 1.f));
    }
    else if (firstWord == "vn")
    {
      float vx = 0.0f;
      float vy = 0.0f;
      float vz = 0.0f;
      ifs >> vx >> vy >> vz;
      vn_list.push_back(vec4(vx, vy, vz, 0.f));
    }
    else if (firstWord == "f")
    {
      Face f;
      char s = 0;
      for (int i = 0; i < 3; ++i)
      {
        ifs >> f.v[i] >> s >> s >> f.vn[i];
        --f.v[i];
        --f.vn[i];
      }

      f_list.push_back(f);
    }

    ifs.ignore(2048, '\n');
  }

  NormalizeData(v_list);

  for (auto const & face : f_list)
  {
    Facet f;
    for (int i = 0; i < 3; ++i)
    {
      f.points[i] = v_list[face.v[i]];
      f.normals[i] = vn_list[face.vn[i]];
    }
    m_facets.push_back(f);
  }

  return true;
}

void Mesh::Clear()
{
  m_facets.clear();
}