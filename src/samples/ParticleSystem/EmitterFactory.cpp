#include "EmitterFactory.h"
#include "EmitterLinear.h"
#include "EmitterRandom.h"
#include "GenPosPoint.h"
#include "GenPosSphere.h"
#include "GenVelCone.h"
#include "GenVelOutwards.h"
#include "GenRelativeForce.h"
#include "GenColor.h"
#include "GenLife.h"
#include "GenSize.h"
#include "Types.h"

typedef Dg::Vector4<float>      vec4;
typedef Dg::Quaternion<float>   quat;
typedef Dg::VQS<float>          Vqs;

Dg::ParticleEmitter<float> * EmitterFactory::operator()(EmitterData const & a_data) const
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
    pEmitter->AddGenerator(E_GenRelativeForce, CreateGenRelativeForce(a_data));

    //Position
    pEmitter->AddGenerator(a_data.posGenMethod, CreateGenPosition(a_data));
    
    //Velocity
    pEmitter->AddGenerator(a_data.velGenMethod, CreateGenVelocity(a_data));

    //Other attributes
    pEmitter->SetRate(a_data.rate);
    if (a_data.on)
    {
      pEmitter->Start();
    }
    else
    {
      pEmitter->Stop();
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

Dg::ParticleGenerator<float> * EmitterFactory::CreateGenPosition(EmitterData const & a_data) const
{
  switch (a_data.posGenMethod)
  {
  case E_GenPosPoint:
  {
    return CreateGenPosPoint(a_data);
  }
  case E_GenPosSphere:
  {
    return CreateGenPosSphere(a_data);
  }
  }
  return nullptr;
}

Dg::ParticleGenerator<float> * EmitterFactory::CreateGenVelocity(EmitterData const & a_data) const
{
  switch (a_data.velGenMethod)
  {
  case E_GenVelCone:
  {
    return CreateGenVelCone(a_data);
  }
  case E_GenVelOutwards:
  {
    return CreateGenVelOutwards(a_data);
  }
  }
  return nullptr;
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
  pGen->SetVelocity(a_data.velocity);
  return pGen;
}

Dg::ParticleGenerator<float> * EmitterFactory::CreateGenVelOutwards(EmitterData const & a_data) const
{
  switch (a_data.posGenMethod)
  {
  case E_GenPosPoint:
  case E_GenPosSphere:
  default:
  {
    Dg::Vector4<float> origin(a_data.transform[0]
                            , a_data.transform[1]
                            , a_data.transform[2]
                            , 1.0f);
    GenVelOutwards<float> * pGen = new GenVelOutwards<float>;
    pGen->SetOrigin(origin);
    pGen->SetVelocity(a_data.velocity);
    return pGen;
  }
  }
  return nullptr;
}

Dg::ParticleGenerator<float> * EmitterFactory::CreateGenRelativeForce(EmitterData const & a_data) const
{
  GenRelativeForce<float> * pGen = new GenRelativeForce<float>;
  pGen->SetValue(a_data.relativeForce);
  return pGen;
}