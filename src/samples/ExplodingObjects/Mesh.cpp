
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

  std::string firstWord;
  std::vector<vec4> vn_list;
  std::vector<Face> faceNormals;

  while (ifs >> firstWord)
  {
    if (firstWord == "v")
    {
      float vx = 0.0f;
      float vy = 0.0f;
      float vz = 0.0f;
      ifs >> vx >> vy >> vz;
      m_points.push_back(vec4(vx, vy, vz, 1.f));
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
      Face vFace, vnFace;
      char s = 0;
      for (int i = 0; i < 3; ++i)
      {
        ifs >> vFace[i] >> s >> s >> vnFace[i];
        vFace[i] -= 1.f;
        vnFace[i] -= 1.f;
      }

      m_faces.push_back(vFace);
      faceNormals.push_back(vnFace);
    }

    ifs.ignore(2048, '\n');
  }

  NormalizeData(m_points);

  m_normals.resize(m_points.size());
  for (size_t i = 0; i < m_faces.size(); i++)
  {
    for (int j = 0; j < 3; ++j)
    {
      auto f = m_faces[i];
      auto fn = faceNormals[i];
      m_normals[m_faces[i][j]] = vn_list[faceNormals[i][j]];
    }
  }

  return true;
}

void Mesh::Clear()
{
  m_faces.clear();
  m_normals.clear();
  m_points.clear();
}

void Mesh::GetData(std::vector<float> & a_vertices,
                   std::vector<float> & a_normals,
                   std::vector<unsigned short> & a_faces) const
{
  for (auto const & p : m_points)
  {
    for (int i = 0; i < 3; ++i)
    {
      a_vertices.push_back(p[i]);
    }
  }

  for (auto const & vn : m_normals)
  {
    for (int i = 0; i < 3; ++i)
    {
      a_normals.push_back(vn[i]);
    }
  }

  for (auto const & f : m_faces)
  {
    for (int i = 0; i < 3; ++i)
    {
      a_faces.push_back(unsigned short(f[i]));
    }
  }
}