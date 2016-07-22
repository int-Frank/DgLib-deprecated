
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "Application.h"
#include "EmitterFactory.h"
#include "AttractorFactory.h"
#include "./json/json.h"
#include "DgStringFunctions.h"

#define ARRAY_SIZE(arr) (int)((sizeof(arr))/(sizeof(*arr)))

bool Application::LoadProject(std::string a_file)
{
  ClearProject();

  std::ifstream fs;
  fs.open(a_file);
  if (!fs.good())
  {
    printf("Failed to open file '%s'!\n", a_file.c_str());
    return false;
  }

  Json::Value root;
  fs >> root;

  Json::Value node = root["State"];
  if (!node.isNull())
  {
    Json::Value val = node["useUpdaterColor"];
    if (!val.isNull()) m_projData.parSysOpts[0].useUpdaterColor = val.asBool();

    val = node["useUpdaterRelativeForce"];
    if (!val.isNull()) m_projData.parSysOpts[0].useUpdaterRelativeForce = val.asBool();

    val = node["useUpdaterSize"];
    if (!val.isNull()) m_projData.parSysOpts[0].useUpdaterSize = val.asBool();
  }

  InitParticleSystem();

  node = root["Emitters"];
  if (!node.isNull())
  {
    for (int i = 0; i < (int)node.size(); ++i)
    {
      EDataItem e;
      Json::Value val = node[i]["ID"];
      if (!val.isNull()) e.first.ID = val.asInt();

      val = node[i]["type"];
      if (!val.isNull()) e.first.type = val.asInt();

      val = node[i]["on"];
      if (!val.isNull()) e.first.on = val.asBool();

      val = node[i]["posGenMethod"];
      if (!val.isNull()) e.first.posGenMethod = val.asInt();

      val = node[i]["velGenMethod"];
      if (!val.isNull()) e.first.velGenMethod = val.asInt();

      val = node[i]["relativeForce"];
      if (!val.isNull()) e.first.relativeForce = val.asFloat();

      val = node[i]["velocity"];
      if (!val.isNull()) e.first.velocity = val.asFloat();

      val = node[i]["rate"];
      if (!val.isNull()) e.first.rate = val.asFloat();

      val = node[i]["life"];
      if (!val.isNull()) e.first.life = val.asFloat();

      val = node[i]["transform"];
      if (!val.isNull())
      {
        std::vector<float> t;
        Dg::StringToNumberList(val.asString(), ',', std::dec, t);
        if (t.size() == 7) memcpy(e.first.transform, t.data(), 7 * sizeof(float));
      }

      val = node[i]["boxDim"];
      if (!val.isNull())
      {
        std::vector<float> t;
        Dg::StringToNumberList(val.asString(), ',', std::dec, t);
        if (t.size() == 3) memcpy(e.first.boxDim, t.data(), 3 * sizeof(float));
      }

      val = node[i]["velCone"];
      if (!val.isNull())
      {
        std::vector<float> t;
        Dg::StringToNumberList(val.asString(), ',', std::dec, t);
        if (t.size() == 3) memcpy(e.first.velCone, t.data(), 3 * sizeof(float));
      }

      val = node[i]["colors"];
      if (!val.isNull())
      {
        std::vector<float> t;
        Dg::StringToNumberList(val.asString(), ',', std::dec, t);
        if (t.size() == 8) memcpy(e.first.colors, t.data(), 8 * sizeof(float));
      }

      val = node[i]["sizes"];
      if (!val.isNull())
      {
        std::vector<float> t;
        Dg::StringToNumberList(val.asString(), ',', std::dec, t);
        if (t.size() == 2) memcpy(e.first.sizes, t.data(), 2 * sizeof(float));
      }

      memcpy(&e.second, &e.first, sizeof(EmitterData));
      m_projData.eData.push_back(e);
      m_IDManager.MarkAsUsed(e.first.ID);
      EmitterFactory eFact;
      m_particleSystem.AddEmitter(e.first.ID, eFact(e.first));
      m_focusEmitter = 0;
    }
  }

  node = root["Attractors"];
  if (!node.isNull())
  {
    for (int i = 0; i < (int)node.size(); ++i)
    {
      ADataItem a;
      Json::Value val;
      val = node[i]["ID"];
      if (!val.isNull()) a.first.ID = val.asInt();

      val = node[i]["appliedAccelType"];
      if (!val.isNull()) a.first.appliedAccelType = val.asInt();

      val = node[i]["maxAppliedAccelMag"];
      if (!val.isNull()) a.first.maxAppliedAccelMag = val.asFloat();

      val = node[i]["show"];
      if (!val.isNull()) a.first.show = val.asBool();

      val = node[i]["strength"];
      if (!val.isNull()) a.first.strength = val.asFloat();

      val = node[i]["type"];
      if (!val.isNull()) a.first.type = val.asInt();

      val = node[i]["transform"];
      if (!val.isNull())
      {
        std::vector<float> t;
        Dg::StringToNumberList(val.asString(), ',', std::dec, t);
        if (t.size() == 6) memcpy(a.first.transform, t.data(), 6 * sizeof(float));
      }

      memcpy(&a.second, &a.first, sizeof(AttractorData));
      m_projData.aData.push_back(a);
      m_IDManager.MarkAsUsed(a.first.ID);
      AttractorFactory aFact;
      m_particleSystem.AddUpdater(a.first.ID, aFact(a.first));
      m_focusAttr = 0;
    }
  }

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

  root["Emitters"] = Json::Value();
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

    root["Emitters"].append(emitter);
  }

  root["Attractors"] = Json::Value();
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

    root["Attractors"].append(attractor);
  }
  
  fs << root;
  printf("%s saved!\n", a_file.c_str());
  return true;
}