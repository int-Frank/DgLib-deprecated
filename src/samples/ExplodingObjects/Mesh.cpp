
#include <iostream>
#include <fstream>

#include "Mesh.h"
#include "DgR3AABB.h"

typedef Dg::R3::AABB<float> AABB;


static AABB GetAABB(std::vector<vec4> & a_points)
{
  float minx = std::numeric_limits<float>::max();
  float maxx = -std::numeric_limits<float>::max();
  float miny = std::numeric_limits<float>::max();
  float maxy = -std::numeric_limits<float>::max();
  float minz = std::numeric_limits<float>::max();
  float maxz = -std::numeric_limits<float>::max();

  for (auto const & p : a_points)
  {
    if (p.x() < minx)  minx = p.x();
    else if (p.x() > maxx)  maxx = p.x();

    if (p.y() < miny)  miny = p.y();
    else if (p.y() > maxy)  maxy = p.y();

    if (p.z() < minz)  minz = p.z();
    else if (p.z() > maxz)  maxz = p.z();
  }

  float hl[3] = { (maxx - minx) / 2.0f,
                  (maxy - miny) / 2.0f, 
                  (maxz - minz) / 2.0f };

  vec4 center(minx + hl[0], 
              miny + hl[1],
              minz + hl[2], 
              1.0f);

  return AABB(center, hl);
}


static void NormalizeData(std::vector<vec4> & a_points)
{
  AABB aabb = GetAABB(a_points);
  vec4 offset(-aabb.GetCenter().x(), 
              -aabb.GetCenter().y(),
              -aabb.GetCenter().z(), 
              0.0f);
  for (auto & p : a_points)
  {
    p += offset;
  }

  float hl[3] = {};
  aabb.GetHalfLengths(hl);
  float max_hl = (hl[0] > hl[1]) ? hl[0] : hl[1];
  if (hl[2] > max_hl) max_hl = hl[2];

  float scaleFactor = 0.5f / max_hl;
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