#ifndef TYPES_H
#define TYPES_H

#include "./particle_system/DgAttractor.h"

enum
{
  //Updaters. Number these in the order you want them to run - ie lowest first, highest last.
  E_UpdaterLife             = -1,
  E_UpdaterZeroAccel        = 0,

  //We need to keep any attractor id's in this range, so they will
  //update after the life and zero accel updaters, but before the Euler.
  E_UpdaterGeneric_begin  = 1,
  E_UpdaterGeneric_end    = 64,

  E_UpdaterEuler,
  E_UpdaterColor,
  E_UpdaterSize,

  //Generators. Number these in the order you want them to run - ie lowest first, highest last.
  E_GenPosPoint,
  E_GenPosBox,
  E_GenPosSphere,
  E_GenVelCone,
  E_GenVelOutwards,
  E_GenColor,
  E_GenLife,
  E_GenSize,
  E_GenForce,

  //Attractor types
  E_AttNone,
  E_AttGlobal,
  E_AttPoint,
  E_AttLine,
  E_AttPlane,

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
    , shape(E_AttNone)
    , forceType(Dg::Attractor<float>::Constant)
    , strength(1.0f)
    , maxAccelMag(10.0f)
    , transform{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }
    , show(false)
  {}

  int ID;
  int shape;      //[None, Global, Point, Line, Plane]
  int forceType;  //[constant , linear, inverse square]
  float strength;
  float maxAccelMag;
  float transform[6]; // [x, y, z, rz, ry, scale] 
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
    , boxDim{ 0.5f, 0.5f, 0.5f }
    , velCone{ 0.0f, 0.0f, Dg::PI_f / 8.0f }
    , velocity(1.0f)
    , colors{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }
    , rate(10.0f)
    , life(5.0f)
    , sizes{ 0.1f, 0.1f }
  {}

  int ID;
  int type;
  bool on;
  int posGenMethod; // point, box, sphere
  int velGenMethod; // direction, repel from center     
  float transform[7]; // [x, y, z, rx, ry, rz, scale]         
  float boxDim[3];
  float velCone[3]; // [rz, rx, angle]    
  float velocity;
  float colors[8]; //[sr, sg, sb, sa, er, eg, eb, ea]
  float rate;
  float life;
  float sizes[2]; //[start, end]
};


#endif