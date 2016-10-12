
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>

#include "Renderer.h"

bool Renderer::LoadData(std::vector<float> & a_v
                      , std::vector<float> & a_vn
                      , std::vector<GLushort> & a_fv)
{
  std::string fileName = "./resources/segment.obj";
  std::ifstream ifs;
  ifs.open(fileName);

  if (!ifs.good())
  {
    std::cout << "Can't open data file './resources/segment.obj'\n";
    return false;
  }

  std::string firstWord;
  std::vector<float> vn;
  std::vector<GLushort> fvn;

  while (ifs >> firstWord)
  {
    if (firstWord == "v")
    {
      float vx = 0.0f;
      float vy = 0.0f;
      float vz = 0.0f;
      ifs >> vx >> vy >> vz;
      a_v.push_back(vx);
      a_v.push_back(vy);
      a_v.push_back(vz);
    }
    else if (firstWord == "vn")
    {
      float vx = 0.0f;
      float vy = 0.0f;
      float vz = 0.0f;
      ifs >> vx >> vy >> vz;
      vn.push_back(vx);
      vn.push_back(vy);
      vn.push_back(vz);
    }
    else if (firstWord == "f")
    {
      GLushort vi0 = 0, vi1 = 0, vi2 = 0;
      GLushort vni0 = 0, vni1 = 0, vni2 = 0;
      char s = 0;
      ifs >> vi0 >> s >> s >> vni0;
      ifs >> vi1 >> s >> s >> vni1;
      ifs >> vi2 >> s >> s >> vni2;
      a_fv.push_back(vi0 - 1);
      a_fv.push_back(vi1 - 1);
      a_fv.push_back(vi2 - 1);
      fvn.push_back(vni0 - 1);
      fvn.push_back(vni1 - 1);
      fvn.push_back(vni2 - 1);
    }

    ifs.ignore(2048, '\n');
  }

  a_vn.resize(a_v.size());
  for (size_t i = 0; i < a_fv.size(); i++)
  {
    a_vn[(a_fv[i]) * 3]     = vn[(fvn[i]) * 3];
    a_vn[(a_fv[i]) * 3 + 1] = vn[(fvn[i]) * 3 + 1];
    a_vn[(a_fv[i]) * 3 + 2] = vn[(fvn[i]) * 3 + 2];
  }

  return true;
}