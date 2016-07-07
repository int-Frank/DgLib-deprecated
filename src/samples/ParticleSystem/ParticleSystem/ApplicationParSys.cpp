#include <cstring>

#include "Application.h"

#include "AttractorPoint.h"
#include "AttractorPointPulse.h"
#include "AttractorLine.h"
#include "AttractorPlane.h"
#include "AttractorPlaneRepel.h"
#include "AttractorGlobal.h"
#include "EmitterLinear.h"
#include "EmitterPoisson.h"
#include "GenPosPoint.h"
#include "GenVelCone.h"
#include "GenColor.h"
#include "GenLife.h"
#include "GenSize.h"
#include "UpdaterColor.h"
#include "UpdaterEuler.h"
#include "UpdaterLife.h"
#include "UpdaterSize.h"
#include "UpdaterZeroAccel.h"

using namespace Dg;

typedef Dg::Matrix44<float>     mat44;
typedef Dg::Vector4<float>      vec4;
typedef Dg::Quaternion<float>   quat;
typedef Dg::VQS<float>          Vqs;

static AttractorGlobal<float, AttractorForce::Constant> CreateAttractor_1()
{
  Vqs vqs;
  quat q;

  AttractorGlobal<float, AttractorForce::Constant> attr;
  attr.SetTransformation(vqs);
  attr.SetStrength(1.0f);

  return attr;
}

static AttractorPoint<float, AttractorForce::InverseSquare> CreateAttractor_2()
{
  Vqs vqs;
  vec4 v;

  v.Set(2.0f, 2.0f, 2.0f, 0.0f);
  vqs.SetV(v);
  AttractorPoint<float, AttractorForce::InverseSquare> attr;
  attr.SetTransformation(vqs);
  attr.SetStrength(-10.0f);
  attr.SetMaxAccelMagnitude(1.0);

  return attr;
}

//Helper function to initialise our emitters
template<typename EmitterType>
static EmitterType CreateEmitter(EmitterData & a_data)
{
  EmitterType emitter;
  Vqs vqs;
  quat qx, qz;
  vec4 v;

  if (a_data.on) emitter.Start();
  else emitter.Stop();

  GenPosPoint<float> genPos;
  vqs.SetV(vec4(a_data.pos[0], a_data.pos[1], a_data.pos[2], 0.0));
  genPos.SetTransformation(vqs);
  a_data.posGenMethod = a_data.prev_posGenMethod = Application::E_GenPosPoint;
  emitter.AddGenerator(Application::E_GenPosPoint, genPos);

  GenVelCone<float>  genVel;
  qz.SetRotationZ(a_data.velRot[0]);
  qx.SetRotationX(a_data.velRot[1]);
  vqs.SetQ(qz * qx);
  genVel.SetTransformation(vqs);
  genVel.SetAngle(a_data.spread);
  a_data.velGenMethod = a_data.prev_velGenMethod = Application::E_GenVelCone;
  emitter.AddGenerator(Application::E_GenVelCone, genVel);

  GenColor<float> genColor;
  genColor.SetColors(vec4(a_data.startColor[0]
                        , a_data.startColor[1]
                        , a_data.startColor[2]
                        , a_data.startColor[3])
                   , vec4(a_data.endColor[0]
                        , a_data.endColor[1]
                        , a_data.endColor[2]
                        , a_data.endColor[3]));
  emitter.AddGenerator(Application::E_GenColor, genColor);

  GenLife<float> genLife;
  genLife.SetLife(a_data.life);
  emitter.AddGenerator(Application::E_GenLife, genLife);

  GenSize<float> genSize;
  genSize.SetSizes(a_data.startSize, a_data.endSize);
  emitter.AddGenerator(Application::E_GenSize, genSize);

  emitter.SetRate(a_data.rate);

  return emitter;
}

void Application::InitParticleSystem()
{
  //--------------------------------------------------------------------
  //  Set emitter 1 data
  //--------------------------------------------------------------------
  m_eData[0].Init();
  m_eData[0].ID = E_Emitter_1;
  m_eData[0].pos[0] = m_eData[0].prev_pos[0] = 1.0;
  m_eData[0].pos[1] = m_eData[0].prev_pos[1] = 0.0;
  m_eData[0].pos[2] = m_eData[0].prev_pos[2] = 0.0;
  m_eData[0].velRot[1] = m_eData[0].prev_velRot[1] = PI_f * 0.5f;
  m_eData[0].startSize = m_eData[0].prev_startSize = 0.1f;
  m_eData[0].endSize = m_eData[0].prev_endSize = 0.3f;
  m_eData[0].endColor[0] = m_eData[0].prev_endColor[0] = 1.0f;
  m_eData[0].endColor[1] = m_eData[0].prev_endColor[1] = 0.0f;
  m_eData[0].endColor[2] = m_eData[0].prev_endColor[2] = 0.0f;
  m_eData[0].endColor[3] = m_eData[0].prev_endColor[3] = 0.0f;


  //--------------------------------------------------------------------
  //  Set emitter 2 data
  //--------------------------------------------------------------------
  m_eData[1].Init();
  m_eData[1].ID = E_Emitter_2;
  m_eData[1].pos[0] = m_eData[1].prev_pos[0] = -0.5f;
  m_eData[1].pos[1] = m_eData[1].prev_pos[1] = 0.866f;
  m_eData[1].pos[2] = m_eData[1].prev_pos[2] = 0.0;
  m_eData[1].velRot[1] = m_eData[1].prev_velRot[1] = PI_f * 0.5f;
  m_eData[1].startSize = m_eData[1].prev_startSize = 0.1f;
  m_eData[1].endSize = m_eData[1].prev_endSize = 0.3f;
  m_eData[1].endColor[0] = m_eData[1].prev_endColor[0] = 0.0f;
  m_eData[1].endColor[1] = m_eData[1].prev_endColor[1] = 1.0f;
  m_eData[1].endColor[2] = m_eData[1].prev_endColor[2] = 0.0f;
  m_eData[1].endColor[3] = m_eData[1].prev_endColor[3] = 0.0f;
  

  //--------------------------------------------------------------------
  //  Set emitter 3 data
  //--------------------------------------------------------------------
  m_eData[2].Init();
  m_eData[2].ID = E_Emitter_3;
  m_eData[2].pos[0] = m_eData[2].prev_pos[0] = -0.5;
  m_eData[2].pos[1] = m_eData[2].prev_pos[1] = -0.866f;
  m_eData[2].pos[2] = m_eData[2].prev_pos[2] = 0.0;
  m_eData[2].velRot[1] = m_eData[2].prev_velRot[1] = PI_f * 0.5f;
  m_eData[2].startSize = m_eData[2].prev_startSize = 0.1f;
  m_eData[2].endSize = m_eData[2].prev_endSize = 0.3f;
  m_eData[2].endColor[0] = m_eData[2].prev_endColor[0] = 0.0f;
  m_eData[2].endColor[1] = m_eData[2].prev_endColor[1] = 0.0f;
  m_eData[2].endColor[2] = m_eData[2].prev_endColor[2] = 1.0f;
  m_eData[2].endColor[3] = m_eData[2].prev_endColor[3] = 0.0f;
  

  //Init Particle Data attributes
  Dg::ParticleData<float> * pData = m_particleSystem.GetParticleData();
  pData->InitAttribute(Dg::ParticleData<float>::Attr::Position);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::Velocity);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::Acceleration);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::Size);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::StartSize);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::DSize);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::Life);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::LifeMax);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::DLife);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::TimeSinceBirth);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::Color);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::StartColor);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::DColor);

  //Add emitters
  m_particleSystem.AddEmitter(E_Emitter_1, CreateEmitter<EmitterLinear<float>>(m_eData[0]));
  m_particleSystem.AddEmitter(E_Emitter_2, CreateEmitter<EmitterLinear<float>>(m_eData[1]));
  m_particleSystem.AddEmitter(E_Emitter_3, CreateEmitter<EmitterLinear<float>>(m_eData[2]));

  //Add Updaters
  m_particleSystem.AddUpdater(E_UpdaterLife, UpdaterLife<float>());
  m_particleSystem.AddUpdater(E_UpdaterZeroAccel, UpdaterZeroAccel<float>());
  //m_particleSystem.AddUpdater(E_UpdaterAttractor_1, CreateAttractor_1());
  m_particleSystem.AddUpdater(E_UpdaterAttractor_2, CreateAttractor_2());
  m_particleSystem.AddUpdater(E_UpdaterEuler, UpdaterEuler<float>());
  m_particleSystem.AddUpdater(E_UpdaterColor, UpdaterColor<float>());
  m_particleSystem.AddUpdater(E_UpdaterSize, UpdaterSize<float>());

}

void Application::UpdateParSysAttr()
{
  for (int e = 0; e < s_nEmitters; ++e)
  {
    EmitterData & data = m_eData[e];
    int id = data.ID;

    if (data.emitterType != data.prev_emitterType)
    {
      //TODO Do work...
      //Here we have to remove the entire emitter and build a new one.
      data.prev_emitterType = data.emitterType;
    }

    //Subsequent checks will not require us to remove the emitter from the particle system
    Dg::ParticleEmitter<float> * ptr = m_particleSystem.GetEmitter(data.ID);
    
    if (ptr == nullptr)
    {
      //Error!
      continue;
    }

    if (data.on != data.prev_on)
    {
      if (data.on) ptr->Start();
      else ptr->Stop();
      data.prev_on = data.on;
    }

    if (data.posGenMethod != data.prev_posGenMethod)
    {
      //TODO DO work...
      data.prev_posGenMethod = data.posGenMethod;
    }

    if (memcmp(data.pos, data.prev_pos, sizeof(float) * 3) != 0)
    {
      Dg::ParticleGenerator<float> * pPosGen = ptr->GetGenerator(data.posGenMethod);
      if (pPosGen)
      {
        Vqs vqs;
        vec4 v(data.pos[0], data.pos[1], data.pos[2], 0.0f);
        vqs.SetV(v);
        pPosGen->SetTransformation(vqs);
      }
      memcpy(data.prev_pos, data.pos, sizeof(float) * 3);
    }

    if (memcmp(data.rot, data.prev_rot, sizeof(float) * 2) != 0)
    {
      //TODO Do work...
      memcpy(data.prev_rot, data.rot, sizeof(float) * 3);
    }

    if (data.spread != data.prev_spread)
    {
      if (data.velGenMethod == E_GenVelCone)
      {
        GenVelCone<float> * pVelGen = (GenVelCone<float> *)ptr->GetGenerator(data.velGenMethod);
        if (pVelGen)
        {
          pVelGen->SetAngle(data.spread);
        }
      }
      data.prev_spread = data.spread;
    }

    if (memcmp(data.boxDim, data.prev_boxDim, sizeof(float) * 3) != 0)
    {
      //TODO Do work...
      memcpy(data.prev_boxDim, data.boxDim, sizeof(float) * 3);
    }

    if (data.repelFromCenter != data.prev_repelFromCenter)
    {
      //TODO DO work... All pos generators should inherit a base
      // position gen with the RepelFromCenter method in the base.
      data.prev_repelFromCenter = data.repelFromCenter;
    }

    if (memcmp(data.velRot, data.prev_velRot, sizeof(float) * 2) != 0)
    {
      if (data.velGenMethod == E_GenVelCone)
      {
        Dg::ParticleGenerator<float> * pVelGen = ptr->GetGenerator(data.velGenMethod);
        if (pVelGen)
        {
          quat qz, qx;
          Vqs vqs;
          qz.SetRotationZ(data.velRot[0]);
          qx.SetRotationX(data.velRot[1]);
          vqs.SetQ(qz * qx);
          pVelGen->SetTransformation(vqs);
        }
      }
      memcpy(data.prev_velRot, data.velRot, sizeof(float) * 3);
    }

    if (data.sphereRadius != data.prev_sphereRadius)
    {
      //TODO DO work...
      data.prev_sphereRadius = data.sphereRadius;
    }

    if ( memcmp(data.startColor, data.prev_startColor, sizeof(float) * 4) != 0
      || memcmp(data.endColor, data.prev_endColor, sizeof(float) * 4) != 0)
    {
      GenColor<float> * pColorGen = (GenColor<float> *)ptr->GetGenerator(E_GenColor);
      if (pColorGen)
      {
        pColorGen->SetColors(vec4(data.startColor[0], data.startColor[1], data.startColor[2], data.startColor[3])
                           , vec4(data.endColor[0], data.endColor[1], data.endColor[2], data.endColor[3]));
      }
      memcpy(data.prev_startColor, data.startColor, sizeof(float) * 4);
      memcpy(data.prev_endColor, data.endColor, sizeof(float) * 4);
    }

    if (data.rate != data.prev_rate)
    {
      ptr->SetRate(data.rate);
      data.prev_rate = data.rate;
    }

    if (data.velocity != data.prev_velocity)
    {
      GenVelCone<float> * pVelGen = (GenVelCone<float> *)ptr->GetGenerator(data.velGenMethod);
      if (pVelGen)
      {
        pVelGen->SetVelocity(data.velocity);
      }
      data.prev_velocity = data.velocity;
    }

    if (data.life != data.prev_life)
    {
      GenLife<float> * pLifeGen = (GenLife<float> *)ptr->GetGenerator(E_GenLife);
      if (pLifeGen)
      {
        pLifeGen->SetLife(data.life);
      }
      data.prev_life = data.life;
    }

    if (data.force!= data.prev_force)
    {
      //TODO Do work...
      data.prev_force = data.force;
    }

    if ( data.startSize != data.prev_startSize
      || data.endSize != data.prev_endSize)
    {
      GenSize<float> * pSizeGen = (GenSize<float> *)ptr->GetGenerator(E_GenSize);
      if (pSizeGen)
      {
        pSizeGen->SetSizes(data.startSize, data.endSize);
      }
      data.prev_startSize = data.startSize;
      data.prev_endSize = data.endSize;
    }
  }
}