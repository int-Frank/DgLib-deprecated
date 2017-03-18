
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <limits>

#include "Application.h"
#include "DgStringFunctions.h"


static AABB GetAABB(std::vector<Vector> & a_points)
{
  float minx = std::numeric_limits<float>::max();
  float maxx = -std::numeric_limits<float>::max();
  float miny = std::numeric_limits<float>::max();
  float maxy = -std::numeric_limits<float>::max();

  for (auto const & p : a_points)
  {
    if      (p.x() < minx)  minx = p.x();
    else if (p.x() > maxx)  maxx = p.x();

    if      (p.y() < miny)  miny = p.y();
    else if (p.y() > maxy)  maxy = p.y();
  }

  float hl[2] = { (maxx - minx) / 2.0f,
                  (maxy - miny) / 2.0f };

  Vector center(minx + hl[0], miny + hl[1], 1.0f);

  return AABB(center, hl);
}


static void NormalizeData(std::vector<Vector> & a_points)
{
  AABB aabb = GetAABB(a_points);
  Vector offset(-aabb.GetCenter().x(), -aabb.GetCenter().y(), 0.0f);
  for (auto & p : a_points)
  {
    p += offset;
  }

  float hl[2] = {};
  aabb.GetHalfLengths(hl);
  float max_hl = (hl[0] > hl[1]) ? hl[0] : hl[1];

  float scaleFactor = 0.5f / max_hl;
  for (auto & p : a_points)
  {
    p.x() *= scaleFactor;
    p.y() *= scaleFactor;
  }
}

void Application::UpdateProjectTitle(std::string const & a_name)
{
  char pFile[32];
  _splitpath_s(
               a_name.c_str()
               , nullptr
               , 0
               , nullptr
               , 0
               , pFile
               , 32
               , nullptr
               , 0
  );

  m_appData.projName = std::string(pFile);
}

bool Application::LoadProject(std::string const & a_file)
{
  ClearProject();

  std::ifstream fs;
  fs.open(a_file);
  if (!fs.good())
  {
    printf("Failed to open file '%s'!\n", a_file.c_str());
    return false;
  }

  BSPTree::DataInput data;
  int key = 0;

  std::string word;

  while (fs >> word)
  {
    if (word == "v")
    {
      float vx = 0.0f;
      float vy = 0.0f;
      fs >> vx >> vy;
      data.points.push_back(Vector(vx, vy, 1.f));
      fs.ignore(2048, '\n');
    }
    else if (word == "p")
    {
      std::vector<size_t> polygon;
      std::string str;
      getline(fs, str);
      std::istringstream ss(str);
      size_t num;
      while (ss >> num)
      {
        polygon.push_back(num);
      }
      data.polygons.insert(std::pair<int, std::vector<size_t>>(key, polygon));
      ++key;
    }
  }

  NormalizeData(data.points);
  AABB aabb = GetAABB(data.points);

  m_polygons.clear();
  for (auto const & kv : data.polygons)
  {
    Polygon poly;
    for (auto i : kv.second)
    {
      poly.push_back(data.points[i]);
    }
    m_polygons.insert(std::pair<int, Polygon>(kv.first, poly));
  }



  printf("'%s' loaded!\n", a_file.c_str());
  UpdateProjectTitle(a_file);

  return true;
}

template<typename T>
static std::string ToString(T const * a_data, int a_size)
{
  std::stringstream ss;
  for (int i = 0; i < a_size; ++i)
  {
    ss << a_data[i];
    if (i != a_size - 1) ss << ',';
  }
  return ss.str();
}
