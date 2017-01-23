
#include <iostream>
#include <fstream>

#include "Mesh.h"

typedef Dg::DgArray<size_t, 3>          tri_ind;

seg Mesh::Segment(Dg::eHandle a_eh) const
{
  Dg::vHandle vh0 = Dg::MeshTools::Source(a_eh);
  Dg::vHandle vh1 = Dg::MeshTools::Target(a_eh);
  return seg(GetData(vh0).point,
             GetData(vh1).point);
}

triangle Mesh::Triangle(Dg::fHandle a_fh) const
{
  Dg::vHandle_3 vh3 = Vertices(a_fh);
  return triangle(GetData(vh3[0]).point, 
                  GetData(vh3[1]).point,
                  GetData(vh3[2]).point);
}

vec4 Mesh::Centroid() const
{
  vec4 result(vec4::ZeroVector());
  for (auto const & kv : m_vertexData)
  {
    result += kv.second.point;
  }
  result /= float(m_vertexData.size());
  result.w() = 1.f;
  return result;
}

sphere Mesh::Sphere() const
{
  vec4 centroid = Centroid();
  float maxSqDist = 0.0f;
  for (auto const & kv : m_vertexData)
  {
    float sqDist = centroid.SquaredDistance(kv.second.point);
    if (sqDist > maxSqDist)
    {
      maxSqDist = sqDist;
    }
  }

  return sphere(centroid, sqrt(maxSqDist));
}

void Mesh::CollateData(std::vector<float> & a_vertices,
                       std::vector<float> & a_normals,
                       std::vector<unsigned short> & a_faces) const
{
  std::map<Dg::vHandle, size_t> handleIndexMap;
  size_t ind = 0;
  for (auto const & kv : m_vertexData)
  {
    handleIndexMap.insert(std::pair<Dg::vHandle, size_t>(kv.first, ind));
    ++ind;
    for (int i = 0; i < 3; ++i)
    {
      a_vertices.push_back(kv.second.point[i]);
      a_normals.push_back(kv.second.normal[i]);
    }
  }

  for (auto const & kv : m_faceData)
  {
    for (int i = 0; i < 3; ++i)
    {
      unsigned short val = unsigned short(handleIndexMap.at(kv.second.first[i]));
      a_faces.push_back(val);
    }
  }
}


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

static bool __LoadOBJ(std::string const & a_fileName,
                      std::vector<vec4> & a_points,
                      std::vector<vec4> & a_normals,
                      std::vector<tri_ind> & a_faces)
{
  std::ifstream ifs;
  ifs.open(a_fileName);

  if (!ifs.good())
  {
    std::cout << "Can't open data file: " << a_fileName;
    return false;
  }

  std::string firstWord;
  std::vector<vec4> vn_list;
  std::vector<tri_ind> fvn_list;

  while (ifs >> firstWord)
  {
    if (firstWord == "v")
    {
      float vx = 0.0f;
      float vy = 0.0f;
      float vz = 0.0f;
      ifs >> vx >> vy >> vz;
      a_points.push_back(vec4(vx, vy, vz, 1.f));
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
      size_t vi0 = 0, vi1 = 0, vi2 = 0;
      size_t vni0 = 0, vni1 = 0, vni2 = 0;
      char s = 0;
      ifs >> vi0 >> s >> s >> vni0;
      ifs >> vi1 >> s >> s >> vni1;
      ifs >> vi2 >> s >> s >> vni2;

      tri_ind fv; 
      tri_ind fvn;

      fv[0] = vi0 - 1;
      fv[1] = vi1 - 1;
      fv[2] = vi2 - 1;
      fvn[0] = vni0 - 1;
      fvn[1] = vni1 - 1;
      fvn[2] = vni2 - 1;

      a_faces.push_back(fv);
      fvn_list.push_back(fvn);
    }

    ifs.ignore(2048, '\n');
  }

  //Make sure vertices and normals share the same index.
  a_normals.resize(a_points.size());
  for (size_t i = 0; i < a_faces.size(); i++)
  {
    for (int j = 0; j < 3; ++j)
    {
      a_normals[a_faces[i][j]] = vn_list[fvn_list[i][j]];
    }
  }

  NormalizeData(a_points);

  return true;
}

bool Mesh::LoadOBJ(std::string const & a_fileName)
{
  std::vector<vec4>     points;
  std::vector<vec4>     normals;
  std::vector<tri_ind>  faces;

  if (!__LoadOBJ(a_fileName, points, normals, faces)) return false;

  for (size_t i = 0; i < points.size(); ++i)
  {
    vData vd;
    vd.point = points[i];
    vd.normal = normals[i];
    m_vertexData.insert(std::pair<Dg::vHandle, vData>(Dg::vHandle(Dg::vHandleType(i)), vd));
  }

  for (size_t i = 0; i < faces.size(); ++i)
  {
    Dg::vHandle_3 fd;
    //Vertex Handle values are just their index. 
    fd[0] = Dg::vHandle(Dg::vHandleType(faces[i][0]));
    fd[1] = Dg::vHandle(Dg::vHandleType(faces[i][1]));
    fd[2] = Dg::vHandle(Dg::vHandleType(faces[i][2]));
    m_faceData.insert(std::pair<Dg::fHandle, std::pair<Dg::vHandle_3, _NO_DATA>>(Dg::fHandle(Dg::fHandleType(i)), std::pair<Dg::vHandle_3, _NO_DATA>(fd, _NO_DATA())));

    Dg::eHandle e0(Dg::MeshTools::GetEdgeHandle(fd[0], fd[1]));
    Dg::eHandle e1(Dg::MeshTools::GetEdgeHandle(fd[0], fd[2]));
    Dg::eHandle e2(Dg::MeshTools::GetEdgeHandle(fd[1], fd[2]));

    m_edgeData.insert(std::pair<Dg::eHandle, _NO_DATA>(Dg::eHandle(e0), _NO_DATA()));
    m_edgeData.insert(std::pair<Dg::eHandle, _NO_DATA>(Dg::eHandle(e1), _NO_DATA()));
    m_edgeData.insert(std::pair<Dg::eHandle, _NO_DATA>(Dg::eHandle(e2), _NO_DATA()));
  }
}