
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>

#include "Application.h"
#include "DgStringFunctions.h"


static void RecenterData(std::vector<Vector> & a_points)
{
  if (a_points.size() == 0) return;

  Vector centroidVector(Vector::ZeroVector());
  for (auto const & p : a_points)
  {
    centroidVector += Vector(p.x(), p.y(), 0.0f);
  }

  centroidVector /= float(a_points.size());

  for (auto & p : a_points)
  {
    p -= centroidVector;
  }
}

static void NormalizeData(std::vector<Vector> & a_points)
{
  RecenterData(a_points);

  float currentMax = 0.f;
  for (auto const & p : a_points)
  {
    for (int i = 0; i < 2; ++i)
    {
      if (abs(p[i]) > currentMax) currentMax = p[i];
    }
  }

  if (currentMax != 0.f)
  {
    float scaleFactor = 0.5f / currentMax;
    for (auto & v : a_points)
    {
      v.x() *= scaleFactor;
      v.y() *= scaleFactor;
    }
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
