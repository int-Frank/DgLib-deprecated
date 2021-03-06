#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

#include "./particle_system/DgAttractor.h"
#include "DgR3Matrix.h"

class EmitterData;
class AttractorData;

typedef std::pair<EmitterData, EmitterData>       EDataItem;
typedef std::vector<EDataItem>                    EData;
typedef std::pair<AttractorData, AttractorData>   ADataItem;
typedef std::vector<ADataItem>                    AData;

enum
{
  //------------------------------------------------------------------------------------------
  //Updaters. Number these in the order you want them to run - ie lowest first, highest last.
  E_UpdaterLife             = 0,
  E_UpdaterZeroAccel        = 1,

  //We need to keep any attractor id's in this range, so they will
  //update after the life and zero accel updaters, but before the Euler.
  //We'll also draw any other IDs we need from this pool.
  E_UpdaterGeneric_begin  = 2,
  E_UpdaterGeneric_end    = 65536,

  E_UpdaterRelativeForce,
  E_UpdaterEuler,
  E_UpdaterColor,
  E_UpdaterSize,
  //------------------------------------------------------------------------------------------
  
  //Generators. Number these in the order you want them to run - ie lowest first, highest last.
  E_GenPosPoint,
  E_GenPosBox,
  E_GenPosSphere,
  E_GenVelCone,
  E_GenVelOutwards,
  E_GenColor,
  E_GenLife,
  E_GenSize,
  E_GenRelativeForce,

  //Attractor types
  E_AttNone       = -1,
  E_AttGlobal     = 0,
  E_AttPoint      = 1,
  E_AttLine       = 2,
  E_AttPlane      = 3,

  //Emitter types
  E_Emitter_Linear,
  E_Emitter_Random,

  E_NULL
};

struct AppInfo
{
  char title[128];
  int windowWidth;
  int windowHeight;
  int majorVersion;
  int minorVersion;
  int samples;
  union
  {
    struct
    {
      unsigned int    fullscreen : 1;
      unsigned int    vsync : 1;
      unsigned int    debug : 1;
    };
    unsigned int        all;
  } flags;
};

class AttractorData
{
public:

  AttractorData()
    : ID(0)
    , type(E_AttPoint)
    , attenuationMethod(Dg::Attractor<float>::Constant)
    , strength(1.0f)
    , maxAppliedAccelMag(10.0f)
    , transform{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }
    , show(true)
    , shouldDie(false)
  {}

  void BuildFromString(std::string const &);

  int ID;
  int type;      //[None, Global, Point, Line, Plane]
  int attenuationMethod;  //[constant , linear, inverse square]
  float strength;
  float maxAppliedAccelMag;
  float transform[6]; // [x, y, z, heading, pitch, scale] 
  bool show;
  bool shouldDie;
};

class EmitterData
{
public:

  EmitterData()
    : ID(0)
    , type(E_Emitter_Linear)
    , on(true)
    , posGenMethod(E_GenPosPoint)
    , velGenMethod(E_GenVelCone)
    , transform{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }
    , relativeForce(1.0f)
    , boxDim{ 0.5f, 0.5f, 0.5f }
    , velCone{ 0.0f, 0.0f, Dg::Constants<float>::PI / 8.0f }
    , velocity(1.0f)
    , colors{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }
    , rate(10.0f)
    , life(5.0f)
    , sizes{ 0.1f, 0.1f }
    , shouldDie(false)
  {}

  void BuildFromString(std::string const &);

  int ID;
  int type;
  bool on;
  int posGenMethod; // point, box, sphere
  int velGenMethod; // direction, repel from center     
  float transform[7]; // [x, y, z, rx, ry, rz, scale] 
  float relativeForce;
  float boxDim[3];
  float velCone[3]; // [heading, pitch, spread]    
  float velocity;
  float colors[8]; //[sr, sg, sb, sa, er, eg, eb, ea]
  float rate;
  float life;
  float sizes[2]; //[start, end]
  bool shouldDie;
};

struct LineRenderData
{
  int                   model;
  Dg::R3::Matrix<float>   mat;
  Dg::R3::Vector<float>    col;
};

class ParSysOpts
{
public:

  ParSysOpts()
    : useUpdaterColor(true)
    , useUpdaterSize(true)
    , useUpdaterRelativeForce(true)
  {}

public:
  bool  useUpdaterRelativeForce;
  bool  useUpdaterColor;
  bool  useUpdaterSize;
};

class ProjectData
{
public:
  ProjectData()
    : newEmitters(0)
    , newAttractors(0)
    , dirty(false)
    , emitterFocus(-1)
    , attrFocus(-1)
  {}

  bool dirty;

  std::string   name;

  int newEmitters;
  int newAttractors;
  int emitterFocus;
  int attrFocus;

  EData         eData;
  AData         aData;
  ParSysOpts    parSysOpts[2];
};
#endif