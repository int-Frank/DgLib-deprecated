
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <limits>

#include "Application.h"
#include "DgStringFunctions.h"


static AABB GetAABB(std::vector<Vector> const & a_points)
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

  m_appState.projName = std::string(pFile);
}


bool Application::ParseFile(std::string const & a_filePath,
                            BSPTree::DataInput & a_out)
{
  std::ifstream fs;
  fs.open(a_filePath);
  if (!fs.good())
  {
    printf("Failed to open file '%s'!\n", a_filePath.c_str());
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
      a_out.points.push_back(Vector(vx, vy, 1.f));
      fs.ignore(2048, '\n');
    }
    else if (word == "p")
    {
      std::vector<uint32_t> polygon;
      std::string str;
      getline(fs, str);
      std::istringstream ss(str);
      uint32_t num;
      while (ss >> num)
      {
        polygon.push_back(num);
      }
      a_out.polygons.insert(std::pair<int, std::vector<uint32_t>>(key, polygon));
      ++key;
    }
  }
  return true;
}

void Application::SavePolygons(BSPTree::DataInput const & a_data)
{
  m_polygons.clear();
  for (auto const & kv : a_data.polygons)
  {
    Polygon poly;
    for (auto i : kv.second)
    {
      poly.push_back(a_data.points[i]);
    }
    m_polygons.insert(std::pair<int, Polygon>(kv.first, poly));
  }
}

void Application::SetModelToScreenTransform(BSPTree::DataInput const & a_data)
{
  Matrix T_t_mo; //mo: model to origin translation
  Matrix T_s;    //s: scaling matrix
  Matrix T_t_oc; //oc: origin to canvas translation

  AABB modelBounds = GetAABB(a_data.points);

  float hlm[2] = {};
  modelBounds.GetHalfLengths(hlm);

  float hlc[2] = {};
  m_canvasBounds.GetHalfLengths(hlc);

  float sx = hlc[0] / hlm[0];
  float sy = hlc[1] / hlm[1];

  float scale = (sx < sy) ? sx : sy;

  T_s.Scaling(scale);
  T_t_mo.Translation(Vector::Origin() - modelBounds.GetCenter());
  T_t_oc.Translation(m_canvasBounds.GetCenter() - Vector::Origin());

  m_T_model_screen = T_t_mo * T_s * T_t_oc;
}

bool Application::LoadProject(std::string const & a_file)
{
  ClearProject();

  BSPTree::DataInput data;
  if (!ParseFile(a_file, data))
  {
    return false;
  }

  SavePolygons(data);
  m_bspTree.Build(data);
  SetModelToScreenTransform(data);
  glfwSetWindowTitle(m_window, a_file.c_str());
  printf("'%s' loaded!\n", a_file.c_str());

  return true;
}