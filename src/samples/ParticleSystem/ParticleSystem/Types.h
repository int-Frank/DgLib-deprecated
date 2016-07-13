#ifndef TYPES_H
#define TYPES_H

enum
{
  //Updaters 
  E_UpdaterLife,
  E_UpdaterZeroAccel,
  E_UpdaterAttractor_1,
  E_UpdaterAttractor_2,
  E_UpdaterEuler,
  E_UpdaterColor,
  E_UpdaterSize,

  //Generators
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

  //Emitters
  E_Emitter_1,
  E_Emitter_2,
  E_Emitter_3,

  E_NULL
};

#endif