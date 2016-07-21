
#include <fstream>
#include <iostream>
#include <sstream>

#include "Application.h"
#include "./json/json.h"

#define ARRAY_SIZE(arr) (int)((sizeof(arr))/(sizeof(*arr)))

bool Application::LoadProject(std::string a_file)
{
  std::ifstream fs;
  fs.open(a_file);
  if (fs.good())
  {
    printf("Failed to open file '%s'!\n", a_file.c_str());
    return false;
  }

  Json::Value root;
  fs >> root;



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

bool Application::SaveProject(std::string a_file)
{
  std::ofstream fs;
  fs.open(a_file);
  if (!fs.good())
  {
    printf("Failed to open file '%s'!\n", a_file.c_str());
    return false;
  }

  Json::Value root;

  Json::Value state;
  state["useUpdaterRelativeForce"] = m_projData.parSysOpts[0].useUpdaterRelativeForce;
  state["useUpdaterColor"] = m_projData.parSysOpts[0].useUpdaterColor;
  state["useUpdaterSize"] = m_projData.parSysOpts[0].useUpdaterSize;
  root["State"] = state;

  root["Emitter"] = Json::Value();
  for (size_t i = 0; i < m_projData.eData.size(); ++i)
  {
    Json::Value emitter;
    emitter["ID"] = m_projData.eData[i].first.ID;
    emitter["type"] = m_projData.eData[i].first.type;
    emitter["on"] = m_projData.eData[i].first.on;
    emitter["posGenMethod"] = m_projData.eData[i].first.posGenMethod;
    emitter["velGenMethod"] = m_projData.eData[i].first.velGenMethod;
    emitter["relativeForce"] = m_projData.eData[i].first.relativeForce;
    emitter["velocity"] = m_projData.eData[i].first.velocity;
    emitter["rate"] = m_projData.eData[i].first.rate;
    emitter["life"] = m_projData.eData[i].first.life;

    emitter["sizes"] = ToString(m_projData.eData[i].first.sizes, ARRAY_SIZE(m_projData.eData[i].first.sizes));
    emitter["colors"] = ToString(m_projData.eData[i].first.colors, ARRAY_SIZE(m_projData.eData[i].first.colors));
    emitter["velCone"] = ToString(m_projData.eData[i].first.velCone, ARRAY_SIZE(m_projData.eData[i].first.velCone));
    emitter["boxDim"] = ToString(m_projData.eData[i].first.boxDim, ARRAY_SIZE(m_projData.eData[i].first.boxDim));
    emitter["transform"] = ToString(m_projData.eData[i].first.transform, ARRAY_SIZE(m_projData.eData[i].first.transform));

    root["Emitter"].append(emitter);
  }

  root["Attractor"] = Json::Value();
  for (size_t i = 0; i < m_projData.aData.size(); ++i)
  {
    Json::Value attractor;
    attractor["ID"] = m_projData.aData[i].first.ID;
    attractor["type"] = m_projData.aData[i].first.type;
    attractor["appliedAccelType"] = m_projData.aData[i].first.appliedAccelType;
    attractor["strength"] = m_projData.aData[i].first.strength;
    attractor["maxAppliedAccelMag"] = m_projData.aData[i].first.maxAppliedAccelMag;
    attractor["show"] = m_projData.aData[i].first.show;
    attractor["transform"] = ToString(m_projData.aData[i].first.transform, ARRAY_SIZE(m_projData.aData[i].first.transform));

    root["Attractor"].append(attractor);
  }
  
  fs << root;
  printf("%s saved!\n", a_file.c_str());
  return true;
}