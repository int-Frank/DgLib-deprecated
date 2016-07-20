#ifndef TYPES_H
#define TYPES_H

#include <string>

#include "./particle_system/DgAttractor.h"
#include "DgMatrix44.h"

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

class AttractorData
{
public:

  AttractorData()
    : ID(0)
    , type(E_AttPoint)
    , appliedAccelType(Dg::Attractor<float>::Constant)
    , strength(1.0f)
    , maxAppliedAccelMag(10.0f)
    , transform{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }
    , show(true)
  {}

  void BuildFromString(std::string const &);

  int ID;
  int type;      //[None, Global, Point, Line, Plane]
  int appliedAccelType;  //[constant , linear, inverse square]
  float strength;
  float maxAppliedAccelMag;
  float transform[6]; // [x, y, z, heading, pitch, scale] 
  bool show;
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
    , velCone{ 0.0f, 0.0f, Dg::PI_f / 8.0f }
    , velocity(1.0f)
    , colors{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }
    , rate(10.0f)
    , life(5.0f)
    , sizes{ 0.1f, 0.1f }
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
};

struct LineRenderData
{
  int                   model;
  Dg::Matrix44<float>   mat;
  Dg::Vector4<float>    col;
};


struct ParSysOpts
{
  bool  useUpdaterRelativeForce;
  bool  useUpdaterColor;
  bool  useUpdaterSize;
};
#endif