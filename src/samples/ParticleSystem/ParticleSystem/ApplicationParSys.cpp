#include <cstring>

#include "Application.h"

#include "AttractorPoint.h"
#include "AttractorLine.h"
#include "AttractorPlane.h"
#include "AttractorGlobal.h"
#include "EmitterLinear.h"
#include "EmitterRandom.h"
#include "GenPosPoint.h"
#include "GenPosSphere.h"
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

  v.Set(0.0f, 0.0f, 0.0f, 1.0f);
  vqs.SetV(v);
  AttractorPoint<float, AttractorForce::InverseSquare> attr;
  attr.SetTransformation(vqs);
  attr.SetStrength(-10.0f);
  attr.SetMaxAccelMagnitude(1.0);

  return attr;
}

//Helper function to initiate Attractors
template<unsigned Force>
static Attractor<float, Force> * CreateAttractor(AttractorData & a_data)
{

  unsigned attType = (a_data.type[1] << 16) 
                   | (a_data.pulse);
  
  Attractor<float, Force> * pAttractor(nullptr);
  switch (attType)
  {
  case (1 << 16 | false):
  {
    pAttractor = new AttractorGlobal<float, Force>();
    break;
  }
  case (2 << 16 | false):
  {
    Vqs vqs;
    vec4 v(0.0, 0.0, 5.0, 0.0);
    vqs.SetV(v);
    pAttractor = new AttractorPoint<float, Force>();
    pAttractor->SetTransformation(vqs);
    break;
  }
  case (3 << 16 | false):
  {
    break;
  }
  case (4 << 16 | false):
  {
    break;
  }
  case (0 << 16 | true):
  {
    break;
  }
  case (1 << 16 | true):
  {
    //pAttractor = new AttractorPointPulse<float, Force>();
    break;
  }
  case (2 << 16 | true):
  {
    break;
  }
  case (3 << 16 | true):
  {
    break;
  }
  case (4 << 16 | true):
  {
    break;
  }
  }

  if (pAttractor)
  {
    pAttractor->SetMaxAccelMagnitude(a_data.maxAccelMag);
    pAttractor->SetStrength(a_data.strength);
  }

  return pAttractor;
}

//Helper function to initialise our emitters
template<typename EmitterType>
static EmitterType CreateEmitter(EmitterData & a_data)
{
  EmitterType emitter;
  Vqs vqs;
  quat qy, qz;
  vec4 v;

  if (a_data.on) emitter.Start();
  else emitter.Stop();

  GenPosPoint<float> genPos;
  vqs.SetV(vec4(a_data.transform[0], a_data.transform[1], a_data.transform[2], 0.0));
  genPos.SetTransformation(vqs);
  a_data.posGenMethod = E_GenPosPoint;
  emitter.AddGenerator(E_GenPosPoint, genPos);

  GenVelCone<float>  genVel;
  qz.SetRotationZ(a_data.velCone[0]);
  qy.SetRotationY(a_data.velCone[1] - Dg::PI_f / 2.0f);
  vqs.SetQ(qy * qz);
  genVel.SetTransformation(vqs);
  genVel.SetAngle(a_data.velCone[2]);
  a_data.velGenMethod = E_GenVelCone;
  emitter.AddGenerator(E_GenVelCone, genVel);

  GenColor<float> genColor;
  genColor.SetColors(vec4(a_data.colors[0]
                        , a_data.colors[1]
                        , a_data.colors[2]
                        , a_data.colors[3])
                   , vec4(a_data.colors[4]
                        , a_data.colors[5]
                        , a_data.colors[6]
                        , a_data.colors[7]));
  emitter.AddGenerator(E_GenColor, genColor);

  GenLife<float> genLife;
  genLife.SetLife(a_data.life);
  emitter.AddGenerator(E_GenLife, genLife);

  GenSize<float> genSize;
  genSize.SetSizes(a_data.sizes[0], a_data.sizes[1]);
  emitter.AddGenerator(E_GenSize, genSize);

  a_data.emitterType = E_Emitter_Linear;
  emitter.SetRate(a_data.rate);


  return emitter;
}

void Application::InitParticleSystem()
{
  //--------------------------------------------------------------------
  //  Set emitter 1 data
  //--------------------------------------------------------------------
  m_eData[0].ID = E_Emitter_1;
  m_eData[0].transform[0] = 1.0f * 2.5f;
  m_eData[0].transform[1] = 0.0;
  m_eData[0].transform[2] = 0.0;
  m_eData[0].sizes[0] = 0.1f;
  m_eData[0].sizes[1] = 0.3f;
  m_eData[0].colors[4] = 1.0f;
  m_eData[0].colors[5] = 0.0f;
  m_eData[0].colors[6] = 0.0f;
  m_eData[0].colors[7] = 0.0f;


  //--------------------------------------------------------------------
  //  Set emitter 2 data
  //--------------------------------------------------------------------
  m_eData[1].ID = E_Emitter_2;
  m_eData[1].transform[0] = -0.5f * 2.5f;
  m_eData[1].transform[1] = 0.866f * 2.5f;
  m_eData[1].transform[2] = 0.0;
  m_eData[1].sizes[0] = 0.1f;
  m_eData[1].sizes[1] = 0.3f;
  m_eData[1].colors[4] = 0.0f;
  m_eData[1].colors[5] = 1.0f;
  m_eData[1].colors[6] = 0.0f;
  m_eData[1].colors[7] = 0.0f;
  

  //--------------------------------------------------------------------
  //  Set emitter 3 data
  //--------------------------------------------------------------------
  m_eData[2].ID = E_Emitter_3;
  m_eData[2].transform[0] = -0.5 * 2.5f;
  m_eData[2].transform[1] = -0.866f * 2.5f;
  m_eData[2].transform[2] = 0.0;
  m_eData[2].sizes[0] = 0.1f;
  m_eData[2].sizes[1] = 0.3f;
  m_eData[2].colors[4] = 0.0f;
  m_eData[2].colors[5] = 0.0f;
  m_eData[2].colors[6] = 1.0f;
  m_eData[2].colors[7] = 0.0f;
  

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

  //copy current emitter data for ui callback checking
  memcpy(m_eDataPrev, m_eData, sizeof(EmitterData) * s_nEmitters);

}

void Application::UpdateParSysAttr()
{
  for (int e = 0; e < s_nEmitters; ++e)
  {
    EmitterData & data = m_eData[e];
    EmitterData & dataPrev = m_eDataPrev[e];
    int id = data.ID;

    if (data.emitterType != dataPrev.emitterType)
    {
      Dg::ParticleEmitter<float> * pOldEmitter = m_particleSystem.GetEmitter(dataPrev.ID);
      if (pOldEmitter)
      {
        ParticleEmitter<float> * pNewEmitter(nullptr);
        switch (data.emitterType)
        {
        case E_Emitter_Linear:
        {
          pNewEmitter = new EmitterLinear<float>(*pOldEmitter);
          break;
        }
        case E_Emitter_Random:
        {
          pNewEmitter = new EmitterRandom<float>(*pOldEmitter);
          break;
        }
        }
        pNewEmitter->SetRate(data.rate);
        m_particleSystem.RemoveEmitter(dataPrev.ID);
        m_particleSystem.AddEmitter(data.ID, *pNewEmitter);
        delete pNewEmitter;
      }
      dataPrev.emitterType = data.emitterType;
    }

    //Subsequent checks will not require us to remove the emitter from the particle system
    Dg::ParticleEmitter<float> * ptr = m_particleSystem.GetEmitter(data.ID);
    
    if (ptr == nullptr)
    {
      //Error!
      continue;
    }

    if (data.on != dataPrev.on)
    {
      if (data.on) ptr->Start();
      else ptr->Stop();
      dataPrev.on = data.on;
    }

    if (data.posGenMethod != dataPrev.posGenMethod)
    {
      switch (data.posGenMethod)
      {
      case E_GenPosPoint:
      {
        ptr->RemoveGenerator(E_GenPosBox);
        ptr->RemoveGenerator(E_GenPosSphere);
        GenPosPoint<float> gen;
        gen.SetOrigin(Dg::Vector4<float>(data.transform[0]
                                       , data.transform[1]
                                       , data.transform[2]
                                       , 0.0f));
        ptr->AddGenerator(E_GenPosPoint, gen);
        break;
      }
      case E_GenPosSphere:
      {
        ptr->RemoveGenerator(E_GenPosBox);
        ptr->RemoveGenerator(E_GenPosPoint);
        Dg::Vector4<float>(data.transform[0]
                           , data.transform[1]
                           , data.transform[2]
                           , 0.0f);
        GenPosSphere<float> gen;
        gen.SetOrigin(Dg::Vector4<float>(data.transform[0]
                                       , data.transform[1]
                                       , data.transform[2]
                                       , 0.0f));
        gen.SetRadius(data.transform[6]);
        ptr->AddGenerator(E_GenPosSphere, gen);
        break;
      }
      case E_GenPosBox:
      {
        break;
      }
      }

      dataPrev.posGenMethod = data.posGenMethod;
    }

    if (memcmp(data.transform, dataPrev.transform, sizeof(float) * 7) != 0)
    {
      Dg::ParticleGenerator<float> * pPosGen = ptr->GetGenerator(data.posGenMethod);
      if (pPosGen)
      {
        Vqs vqs;
        vec4 v(data.transform[0], data.transform[1], data.transform[2], 0.0f);
        quat q; q.SetRotation(data.transform[3], data.transform[4], data.transform[5], Dg::EulerOrder::ZXY);
        vqs.SetV(v);
        vqs.SetQ(q);
        vqs.SetS(data.transform[6]);
        pPosGen->SetTransformation(vqs);
      }
      memcpy(dataPrev.transform, data.transform, sizeof(float) * 7);
    }

    if (data.velCone[2] != dataPrev.velCone[2])
    {
      if (data.velGenMethod == E_GenVelCone)
      {
        GenVelCone<float> * pVelGen = (GenVelCone<float> *)ptr->GetGenerator(data.velGenMethod);
        if (pVelGen)
        {
          pVelGen->SetAngle(data.velCone[2]);
        }
      }
      dataPrev.velCone[2] = data.velCone[2];
    }

    if (memcmp(data.boxDim, dataPrev.boxDim, sizeof(float) * 3) != 0)
    {
      //TODO Do work...
      memcpy(dataPrev.boxDim, data.boxDim, sizeof(float) * 3);
    }

    if (data.velGenMethod != dataPrev.velGenMethod)
    {
      //TODO DO work... All pos generators should inherit a base
      // position gen with the RepelFromCenter method in the base.
      dataPrev.velGenMethod = data.velGenMethod;
    }

    if (memcmp(data.velCone, dataPrev.velCone, sizeof(float) * 2) != 0)
    {
      if (data.velGenMethod == E_GenVelCone)
      {
        Dg::ParticleGenerator<float> * pVelGen = ptr->GetGenerator(data.velGenMethod);
        if (pVelGen)
        {
          quat qz, qy;
          Vqs vqs;
          qz.SetRotationZ(data.velCone[0]);
          qy.SetRotationY(-Dg::PI_f / 2.0f + data.velCone[1]);
          vqs.SetQ(qy * qz);
          pVelGen->SetTransformation(vqs);
        }
      }
      memcpy(dataPrev.velCone, data.velCone, sizeof(float) * 2);
    }

    if ( memcmp(data.colors, dataPrev.colors, sizeof(float) * 8) != 0)
    {
      GenColor<float> * pColorGen = (GenColor<float> *)ptr->GetGenerator(E_GenColor);
      if (pColorGen)
      {
        pColorGen->SetColors(vec4(data.colors[0], data.colors[1], data.colors[2], data.colors[3])
                           , vec4(data.colors[4], data.colors[5], data.colors[6], data.colors[7]));
      }
      memcpy(dataPrev.colors, data.colors, sizeof(float) * 8);
    }

    if (data.rate != dataPrev.rate)
    {
      ptr->SetRate(data.rate);
      dataPrev.rate = data.rate;
    }

    if (data.velocity != dataPrev.velocity)
    {
      GenVelCone<float> * pVelGen = (GenVelCone<float> *)ptr->GetGenerator(data.velGenMethod);
      if (pVelGen)
      {
        pVelGen->SetVelocity(data.velocity);
      }
      dataPrev.velocity = data.velocity;
    }

    if (data.life != dataPrev.life)
    {
      GenLife<float> * pLifeGen = (GenLife<float> *)ptr->GetGenerator(E_GenLife);
      if (pLifeGen)
      {
        pLifeGen->SetLife(data.life);
      }
      dataPrev.life = data.life;
    }

    if (memcmp(data.sizes, dataPrev.sizes, sizeof(float) * 2) != 0)
    {
      GenSize<float> * pSizeGen = (GenSize<float> *)ptr->GetGenerator(E_GenSize);
      if (pSizeGen)
      {
        pSizeGen->SetSizes(data.sizes[0], data.sizes[1]);
      }
      memcpy(dataPrev.sizes, data.sizes, sizeof(float) * 2);
    }
  }
}