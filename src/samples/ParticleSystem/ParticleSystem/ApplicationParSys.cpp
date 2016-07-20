#include <cstring>

#include "Application.h"

#include "EmitterFactory.h"
#include "AttractorFactory.h"

#include "AttractorPoint.h"
#include "AttractorLine.h"
#include "AttractorPlane.h"
#include "AttractorGlobal.h"
#include "EmitterLinear.h"
#include "EmitterRandom.h"
#include "GenPosPoint.h"
#include "GenPosSphere.h"
#include "GenVelCone.h"
#include "GenRelativeForce.h"
#include "GenColor.h"
#include "GenLife.h"
#include "GenSize.h"
#include "UpdaterColor.h"
#include "UpdaterEuler.h"
#include "UpdaterRelativeForce.h"
#include "UpdaterLife.h"
#include "UpdaterSize.h"
#include "UpdaterResetAccel.h"

using namespace Dg;

typedef Dg::Vector4<float>      vec4;
typedef Dg::Quaternion<float>   quat;
typedef Dg::VQS<float>          Vqs;

void Application::InitParticleSystem()
{
  //Init Particle Data attributes
  Dg::ParticleData<float> * pData = m_particleSystem.GetParticleData();
  pData->InitAttribute(Dg::ParticleData<float>::Attr::Position);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::Velocity);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::Acceleration);
  pData->InitAttribute(Dg::ParticleData<float>::Attr::Force);
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

  //EmitterFactory    eFactory;
  //AttractorFactory  aFactory;

  //--------------------------------------------------------------------
  //  Set emitter 1 data
  //--------------------------------------------------------------------
  /*m_eData[0].first.ID = m_IDManager.GetID();
  if (m_eData[0].first.ID)
  {
    m_eData[0].first.type = E_Emitter_Linear;
    m_eData[0].first.on = true;
    m_eData[0].first.posGenMethod = E_GenPosPoint;
    m_eData[0].first.velGenMethod = E_GenVelCone;
    m_eData[0].first.transform[0] = 1.0f * 2.5f;
    m_eData[0].first.transform[1] = 0.0;
    m_eData[0].first.transform[2] = 0.0;
    m_eData[0].first.sizes[0] = 0.1f;
    m_eData[0].first.sizes[1] = 0.3f;
    m_eData[0].first.colors[4] = 1.0f;
    m_eData[0].first.colors[5] = 0.0f;
    m_eData[0].first.colors[6] = 0.0f;
    m_eData[0].first.colors[7] = 0.0f;

    m_particleSystem.AddEmitter(m_eData[0].first.ID, eFactory.Create(m_eData[0].first));
  }*/
  
  //--------------------------------------------------------------------
  //  Set emitter 2 data
  //--------------------------------------------------------------------
  //m_eData[1].ID = E_Emitter_2;
  //m_eData[1].transform[0] = -0.5f * 2.5f;
  //m_eData[1].transform[1] = 0.866f * 2.5f;
  //m_eData[1].transform[2] = 0.0;
  //m_eData[1].sizes[0] = 0.1f;
  //m_eData[1].sizes[1] = 0.3f;
  //m_eData[1].colors[4] = 0.0f;
  //m_eData[1].colors[5] = 1.0f;
  //m_eData[1].colors[6] = 0.0f;
  //m_eData[1].colors[7] = 0.0f;
  //

  ////--------------------------------------------------------------------
  ////  Set emitter 3 data
  ////--------------------------------------------------------------------
  //m_eData[2].ID = E_Emitter_3;
  //m_eData[2].transform[0] = -0.5 * 2.5f;
  //m_eData[2].transform[1] = -0.866f * 2.5f;
  //m_eData[2].transform[2] = 0.0;
  //m_eData[2].sizes[0] = 0.1f;
  //m_eData[2].sizes[1] = 0.3f;
  //m_eData[2].colors[4] = 0.0f;
  //m_eData[2].colors[5] = 0.0f;
  //m_eData[2].colors[6] = 1.0f;
  //m_eData[2].colors[7] = 0.0f;
  

  //Add Updaters
  m_particleSystem.AddUpdater(E_UpdaterLife, new UpdaterLife<float>());
  m_particleSystem.AddUpdater(E_UpdaterZeroAccel, new UpdaterResetAccel<float>());

  //--------------------------------------------------------------------
  //  Set attractor data
  //--------------------------------------------------------------------
  /*m_aData[0].ID = m_IDManager.GetID();
  if (m_aData[0].ID)
  {
    m_aData[0].type = E_AttPoint;
    m_aData[0].appliedAccelType = Dg::Attractor<float>::InvSq;
    m_aData[0].strength = -10.0f;
    m_aData[0].maxAppliedAccelMag = 1.0f;
    m_aData[0].transform[0] = 3.0f;
    m_aData[0].transform[1] = 3.0f;
    m_aData[0].transform[2] = 3.0f;
    m_aData[0].show = false;

    m_particleSystem.AddUpdater(m_aData[0].ID, aFactory.Create(m_aData[0]));
  }*/

  m_particleSystem.AddUpdater(E_UpdaterEuler, new UpdaterEuler<float>());
  if (m_parSysOpts[0].useUpdaterRelativeForce)
  {
    m_particleSystem.AddUpdater(E_UpdaterRelativeForce, new UpdaterRelativeForce<float>());
  }
  m_particleSystem.AddUpdater(E_UpdaterColor, new UpdaterColor<float>());
  m_particleSystem.AddUpdater(E_UpdaterSize, new UpdaterSize<float>());

  //copy current emitter data for ui callback checking
  for (int i = 0; i < m_eData.size(); ++i)
  {
    memcpy(&m_eData[i].second, &m_eData[i].first, sizeof(EmitterData));
  }
  for (int i = 0; i < m_aData.size(); ++i)
  {
    memcpy(&m_aData[i].second, &m_aData[i].first, sizeof(AttractorData));
  }
}

void Application::UpdateParSysAttr()
{
  // Update particle system updaters
  if (m_parSysOpts[0].useUpdaterRelativeForce != m_parSysOpts[1].useUpdaterRelativeForce)
  {
    if (!m_parSysOpts[0].useUpdaterRelativeForce)
    {
      m_particleSystem.RemoveUpdater(E_UpdaterRelativeForce);
    }
    else
    {
      m_particleSystem.AddUpdater(E_UpdaterRelativeForce, new UpdaterRelativeForce<float>());
    }
    m_parSysOpts[1].useUpdaterRelativeForce = m_parSysOpts[0].useUpdaterRelativeForce;
  }


  for (int e = 0; e < m_eData.size(); ++e)
  {
    //We just do a dumb check to see if any of the data has changed,
    //and just remove the emitter and build a new one with the new data,
    //but it's very slow; there are noticable delays.
    EmitterData & data = m_eData[e].first;
    EmitterData & dataPrev = m_eData[e].second;
    int id = data.ID;

    if (data.type != dataPrev.type)
    {
      Dg::ParticleEmitter<float> * pOldEmitter = m_particleSystem.GetEmitter(dataPrev.ID);
      if (pOldEmitter)
      {
        ParticleEmitter<float> * pNewEmitter(nullptr);
        switch (data.type)
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

        // memory is cleared once we add to particle system
        m_particleSystem.AddEmitter(data.ID, pNewEmitter);
      }
      dataPrev.type = data.type;
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
        GenPosPoint<float> * pGen = new GenPosPoint<float>();
        pGen->SetOrigin(Dg::Vector4<float>(data.transform[0]
                                         , data.transform[1]
                                         , data.transform[2]
                                         , 0.0f));
        ptr->AddGenerator(E_GenPosPoint, pGen);
        break;
      }
      case E_GenPosSphere:
      {
        ptr->RemoveGenerator(E_GenPosBox);
        ptr->RemoveGenerator(E_GenPosPoint);
        GenPosSphere<float> * pGen = new GenPosSphere<float>();
        pGen->SetOrigin(Dg::Vector4<float>(data.transform[0]
                                         , data.transform[1]
                                         , data.transform[2]
                                         , 0.0f));
        pGen->SetRadius(data.transform[6]);
        ptr->AddGenerator(E_GenPosSphere, pGen);
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

    if (data.velCone[2] != dataPrev.velCone[2] && data.velGenMethod == E_GenVelCone)
    {
      GenVelCone<float> * pVelGen = (GenVelCone<float> *)ptr->GetGenerator(data.velGenMethod);
      if (pVelGen)
      {
        pVelGen->SetAngle(data.velCone[2]);
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

    if (memcmp(data.velCone, dataPrev.velCone, sizeof(float) * 2) != 0
      && data.velGenMethod == E_GenVelCone)
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
      memcpy(dataPrev.velCone, data.velCone, sizeof(float) * 2);
    }

    if (data.relativeForce != dataPrev.relativeForce)
    {
      GenRelativeForce<float> * pGen = dynamic_cast<GenRelativeForce<float>*>(ptr->GetGenerator(E_GenRelativeForce));
      pGen->SetValue(data.relativeForce);
      dataPrev.relativeForce = data.relativeForce;
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

  //Attractors
  for (int a = 0; a < m_aData.size(); ++a)
  {
    AttractorData & data = m_aData[a].first;
    AttractorData & dataPrev = m_aData[a].second;

    if (data.type != dataPrev.type)
    {
      m_particleSystem.RemoveUpdater(dataPrev.ID);
      AttractorFactory aFactory;

      if (data.type != E_AttNone)
      {
        m_particleSystem.AddUpdater(data.ID, aFactory(data));
      }
      dataPrev.type = data.type;
    }


    //Subsequent checks will not require us to remove the attrctor from the particle system
    Dg::Attractor<float> * ptr = dynamic_cast<Dg::Attractor<float>*>(m_particleSystem.GetUpdater(data.ID));

    if (ptr == nullptr)
    {
      //Error!
      continue;
    }

    if (data.appliedAccelType != dataPrev.appliedAccelType)
    {
      ptr->SetAccelType(data.appliedAccelType);
      dataPrev.appliedAccelType = data.appliedAccelType;
    }

    if (data.strength != dataPrev.strength)
    {
      ptr->SetStrength(data.strength);
      dataPrev.strength = data.strength;
    }

    if (data.maxAppliedAccelMag!= dataPrev.maxAppliedAccelMag)
    {
      ptr->SetMaxAppliedAccelMagnitude(data.maxAppliedAccelMag);
      dataPrev.maxAppliedAccelMag = data.maxAppliedAccelMag;
    }

    if (memcmp(data.transform, dataPrev.transform, sizeof(float) * 6) != 0)
    {
      Vqs vqs;
      quat qh, qp;
      qh.SetRotationZ(data.transform[3]);
      qp.SetRotationY(-Dg::PI_f / 2.0f + data.transform[4]);
      vqs.SetQ(qp * qh);
      vqs.SetV(vec4(data.transform[0], data.transform[1], data.transform[2], 0.0f));
      vqs.SetS(data.transform[5]);
      ptr->SetTransformation(vqs);
      memcpy(dataPrev.transform, data.transform, sizeof(float) * 6);
    }
  }

}