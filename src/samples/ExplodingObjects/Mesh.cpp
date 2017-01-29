
#include <iostream>
#include <fstream>

#include "Mesh.h"


static void RecenterData(std::vector<vec4> & a_points)
{
  if (a_points.size() == 0) return;

  vec4 centroid(vec4::ZeroVector());
  for (auto const & v : a_points)
  {
    centroid += v;
  }

  centroid /= float(a_points.size());
  centroid.w() = 1.f;

  vec4 moveVector = vec4::Origin() - centroid;

  for (auto it = a_points.begin();
    it != a_points.end();
    ++it)
  {
    *it += moveVector;
  }
}

static void NormalizeData(std::vector<vec4> & a_points)
{
  RecenterData(a_points);

  float currentMax = 0.f;
  for (auto const & v : a_points)
  {
    for (int i = 0; i < 3; ++i)
    {
      if (abs(v[i]) > currentMax) currentMax = v[i];
    }
  }

  if (currentMax != 0.f)
  {
    float scaleFactor = 0.5f / currentMax;
    for (auto & v : a_points)
    {
      for (int i = 0; i < 3; ++i)
      {
        v[i] *= scaleFactor;
      }
    }
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
        f.v[i] -= 1.f;
        f.vn[i] -= 1.f;
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