
#include <iostream>
#include <fstream>

#include "Mesh.h"


Mesh::Mesh(Mesh const & a_other)
: m_vertices(a_other.m_vertices)
, m_normals(a_other.m_normals)
, m_faces(a_other.m_faces)
{

}


Mesh & Mesh::operator=(Mesh const & a_other)
{
  if (this != &a_other)
  {
    m_vertices = a_other.m_vertices;
    m_normals = a_other.m_normals;
    m_faces = a_other.m_faces;
  }
  return *this;
}


void Mesh::Clear()
{
  m_vertices.clear();
  m_normals.clear();
  m_faces.clear();
}


void Mesh::GetData(std::vector<float> & a_vertices,
                    std::vector<float> & a_normals,
                    std::vector<unsigned short> & a_faces) const
{
  for (auto const & v : m_vertices)
  {
    for (int i = 0; i < 3; ++i)
    {
      a_vertices.push_back(v[i]);
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
      a_faces.push_back(unsigned short(f.verts[i]));
    }
  }
}

int Mesh::NumberFaces() const
{
  return int(m_faces.size());
}

bool Mesh::Load(std::string const & a_fileName)
{
  std::ifstream ifs;
  ifs.open(a_fileName);

  if (!ifs.good())
  {
    std::cout << "Can't open data file: " << a_fileName;
    return false;
  }

  Clear();

  std::string firstWord;
  std::vector<vec4> vn_list;
  std::vector<Face> fvn_list;

  while (ifs >> firstWord)
  {
    if (firstWord == "v")
    {
      float vx = 0.0f;
      float vy = 0.0f;
      float vz = 0.0f;
      ifs >> vx >> vy >> vz;
      m_vertices.push_back(vec4(vx, vy, vz, 1.f));
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
      unsigned short vi0 = 0, vi1 = 0, vi2 = 0;
      unsigned short vni0 = 0, vni1 = 0, vni2 = 0;
      char s = 0;
      ifs >> vi0 >> s >> s >> vni0;
      ifs >> vi1 >> s >> s >> vni1;
      ifs >> vi2 >> s >> s >> vni2;

      Face fv = { vi0 - 1, vi1 - 1, vi2 - 1 };
      Face fvn = {vni0 - 1, vni1 - 1, vni2 - 1 };

      m_faces.push_back(fv);
      fvn_list.push_back(fvn);
    }

    ifs.ignore(2048, '\n');
  }

  //Make sure vertices and normals share the same index.
  m_normals.resize(m_vertices.size());
  for (size_t i = 0; i < m_faces.size(); i++)
  {
    for (int j = 0; j < 3; ++j)
    {
      m_normals[m_faces[i].verts[j]] = vn_list[fvn_list[i].verts[j]];
    }
  }
  return true;
}

void Mesh::RecenterData(vec4 const & a_center)
{
  if (m_vertices.size() == 0) return;

  vec4 centroid(vec4::ZeroVector());
  for (auto v : m_vertices)
  {
    centroid += v;
  }

  centroid /= float(m_vertices.size());
  centroid.w() = 1.f;

  vec4 moveVector = a_center - centroid;

  for (auto it = m_vertices.begin();
       it != m_vertices.end();
       ++it)
  {
    *it += moveVector;
  }
}

void Mesh::NormalizeData(vec4 const & a_center, float a_maxCoord)
{
  RecenterData(a_center);

  float currentMax = 0.f;
  for (auto const & v : m_vertices)
  {
    for (int i = 0; i < 3; ++i)
    {
      if (abs(v[i]) > currentMax) currentMax = v[i];
    }
  }

  if (currentMax != 0.f)
  {
    float scaleFactor = a_maxCoord / currentMax;
    for (auto & v : m_vertices)
    {
      for (int i = 0; i < 3; ++i)
      {
        //v[i] *= scaleFactor;
      }
    }
  }
}