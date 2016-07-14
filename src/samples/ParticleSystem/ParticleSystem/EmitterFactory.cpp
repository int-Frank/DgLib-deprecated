#include "EmitterFactory.h"
#include "EmitterLinear.h"
#include "EmitterRandom.h"
#include "GenPosPoint.h"
#include "GenPosSphere.h"
#include "GenVelCone.h"
#include "GenColor.h"
#include "GenLife.h"
#include "GenSize.h"
#include "Types.h"

typedef Dg::Vector4<float>      vec4;
typedef Dg::Quaternion<float>   quat;
typedef Dg::VQS<float>          Vqs;

Dg::ParticleEmitter<float> * EmitterFactory::Create(EmitterData const & a_data) const
{
  Dg::ParticleEmitter<float> * pEmitter(nullptr);
  switch (a_data.type)
  {
  case E_Emitter_Linear: pEmitter = new EmitterLinear<float>; break;
  case E_Emitter_Random: pEmitter = new EmitterRandom<float>; break;
  }

  if (pEmitter)
  {
    //Add in standard generators
    pEmitter->AddGenerator(E_GenColor, CreateGenColor(a_data));
    pEmitter->AddGenerator(E_GenLife, CreateGenLife(a_data));
    pEmitter->AddGenerator(E_GenSize, CreateGenSize(a_data));

    //Position
    switch (a_data.posGenMethod)
    {
    case E_GenPosPoint: pEmitter->AddGenerator(E_GenPosPoint, CreateGenPosPoint(a_data)); break;
    case E_GenPosSphere: pEmitter->AddGenerator(E_GenPosSphere, CreateGenPosSphere(a_data)); break;
    }

    //Velocity
    switch (a_data.velGenMethod)
    {
    case E_GenVelCone: pEmitter->AddGenerator(E_GenVelCone, CreateGenVelCone(a_data)); break;
    case E_GenVelOutwards: pEmitter->AddGenerator(E_GenVelOutwards, CreateGenVelOutwards(a_data)); break;
    }
  }

  return pEmitter;
}

Dg::ParticleGenerator<float> * EmitterFactory::CreateGenColor(EmitterData const & a_data) const
{
  GenColor<float> * pGen = new GenColor<float>;
  pGen->SetColors(vec4(a_data.colors[0]
                     , a_data.colors[1]
                     , a_data.colors[2]
                     , a_data.colors[3])
                , vec4(a_data.colors[4]
                     , a_data.colors[5]
                     , a_data.colors[6]
                     , a_data.colors[7]));
  return pGen;
}

Dg::ParticleGenerator<float> * EmitterFactory::CreateGenLife(EmitterData const & a_data) const
{
  GenLife<float> * pGen = new GenLife<float>;
  pGen->SetLife(a_data.life);
  return pGen;
}

Dg::ParticleGenerator<float> * EmitterFactory::CreateGenPosPoint(EmitterData const & a_data) const
{
  GenPosPoint<float> * pGen = new GenPosPoint<float>;
  Vqs vqs;
  vqs.SetV(vec4(a_data.transform[0], a_data.transform[1], a_data.transform[2], 0.0));
  pGen->SetTransformation(vqs);
  return pGen;
}

Dg::ParticleGenerator<float> * EmitterFactory::CreateGenPosSphere(EmitterData const & a_data) const
{
  GenPosSphere<float> * pGen = new GenPosSphere<float>;
  Vqs vqs;
  vqs.SetV(vec4(a_data.transform[0], a_data.transform[1], a_data.transform[2], 0.0));
  vqs.SetS(a_data.transform[6]);
  pGen->SetTransformation(vqs);
  return pGen;
}

Dg::ParticleGenerator<float> * EmitterFactory::CreateGenSize(EmitterData const & a_data) const
{
  GenSize<float> * pGen = new GenSize<float>;
  pGen->SetSizes(a_data.sizes[0], a_data.sizes[1]);
  return pGen;
}

Dg::ParticleGenerator<float> * EmitterFactory::CreateGenVelCone(EmitterData const & a_data) const
{
  GenVelCone<float> * pGen = new GenVelCone<float>;
  Vqs vqs;
  quat qy, qz;
  qz.SetRotationZ(a_data.velCone[0]);
  qy.SetRotationY(a_data.velCone[1] - Dg::PI_f / 2.0f);
  vqs.SetQ(qy * qz);
  pGen->SetTransformation(vqs);
  pGen->SetAngle(a_data.velCone[2]);
  return pGen;
}

Dg::ParticleGenerator<float> * EmitterFactory::CreateGenVelOutwards(EmitterData const & a_data) const
{
  //TODO ....
  return nullptr;
}