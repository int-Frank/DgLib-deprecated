//! @file DgParticleSystem.h
//!
//! @author Frank Hart
//! @date 4/10/2015

#ifndef DGPARTICLESYSTEM_H
#define DGPARTICLESYSTEM_H

#include "Dg_map.h"
#include "DgParticleEmitter.h"
#include "DgParticleUpdater.h"
#include "DgParticleData.h"
#include "DgAttractor.h"

//TODO Documentation
namespace Dg
{
  typedef unsigned int;

  template<typename Real>
  class ParticleSystem
  {
  public:
    ParticleSystem(int);
    ~ParticleSystem() {}

    ParticleSystem(ParticleSystem<Real> const & a_other);
    ParticleSystem<Real> & operator=(ParticleSystem<Real> const & a_other);

    //! This function will delete the input emitter
    void AddEmitter(int, ParticleEmitter<Real> *);

    //! This function will delete the input updater
    void AddUpdater(int, ParticleUpdater<Real> *);
    
    void RemoveEmitter(int);
    void RemoveUpdater(int);

    void InitParticleAttr(int);
    void DeinitParticleAttr(int);

    void InitAllParticleAttr();
    void DeinitAllParticleAttr();

    void StartAllEmitters();
    void StopAllEmitters();

    ParticleEmitter<Real> * GetEmitter(int);
    ParticleUpdater<Real> * GetUpdater(int);
    ParticleData<Real> * GetParticleData() { return &m_particleData; }

    void Update(Real dt);
    void Clear();

  private:
    Dg::map<int, ObjectWrapper<ParticleEmitter<Real>>>   m_emitters;
    ParticleData<Real>                                         m_particleData;
    Dg::map<int, ObjectWrapper<ParticleUpdater<Real>>>   m_updaters;
  };

  template<typename Real>
  ParticleSystem<Real>::ParticleSystem(int a_nPar)
    : m_particleData(a_nPar)
    , m_emitters(16)
    , m_updaters(32)
  {

  }


  template<typename Real>
  ParticleSystem<Real>::ParticleSystem(ParticleSystem<Real> const & a_other) 
    : m_particleData(a_other.m_particleData),
      m_emitters(a_other.m_emitters),
      m_updaters(a_other.m_updaters)
  {

  }


  template<typename Real>
  ParticleSystem<Real> & ParticleSystem<Real>::operator=(ParticleSystem<Real> const & a_other)
  {
    if (this == &a_other)
    {
      return *this;
    }

    m_emitters = a_other.m_emitters;
    m_updaters = a_other.m_updaters;
    m_particleData = a_other.m_particleData;

    return *this;
  }


  template<typename Real>
  void ParticleSystem<Real>::AddEmitter(int a_key, ParticleEmitter<Real> * a_pEmitter)
  {
    if (a_pEmitter)
    {
      ObjectWrapper<ParticleEmitter<Real>> newEmitter(a_pEmitter, true);
      m_emitters.insert(a_key, newEmitter);
    }
  }


  template<typename Real>
  void ParticleSystem<Real>::AddUpdater(int a_key, ParticleUpdater<Real> * a_pUpdater)
  {
    if (a_pUpdater)
    {
      ObjectWrapper<ParticleUpdater<Real>> newUpdater(a_pUpdater, true);
      m_updaters.insert(a_key, newUpdater);
    }
  }

  template<typename Real>
  void ParticleSystem<Real>::RemoveEmitter(int a_key)
  {
    m_emitters.erase(a_key);
  }

  template<typename Real>
  void ParticleSystem<Real>::RemoveUpdater(int a_key)
  {
    m_updaters.erase(a_key);
  }

  template<typename Real>
  ParticleEmitter<Real> * ParticleSystem<Real>::GetEmitter(int a_key)
  {
    int index(0);
    if (m_emitters.find(a_key, index))
    {
      return m_emitters[index];
    }
    return nullptr;
  }

  template<typename Real>
  ParticleUpdater<Real> * ParticleSystem<Real>::GetUpdater(int a_key)
  {
    int index(0);
    if (m_updaters.find(a_key, index))
    {
      return m_updaters[index];
    }
    return nullptr;
  }

  template<typename Real>
  void ParticleSystem<Real>::InitParticleAttr(int a_val)
  {
    m_particleData.InitAttribute(a_val);
  }

  template<typename Real>
  void ParticleSystem<Real>::DeinitParticleAttr(int a_val)
  {
    m_particleData.DeinitAttribute(a_val);
  }

  template<typename Real>
  void ParticleSystem<Real>::InitAllParticleAttr()
  {
    m_particleData.InitAll();
  }

  template<typename Real>
  void ParticleSystem<Real>::DeinitAllParticleAttr()
  {
    m_particleData.DeinitAll();
  }

  template<typename Real>
  void ParticleSystem<Real>::StartAllEmitters()
  {
    for (int i = 0; i < m_emitters.size(); ++i)
    {
      m_emitters[i]->Start();
    }
  }

  template<typename Real>
  void ParticleSystem<Real>::StopAllEmitters()
  {
    for (int i = 0; i < m_emitters.size(); ++i)
    {
      m_emitters[i]->Stop();
    }
  }

  template<typename Real>
  void ParticleSystem<Real>::Clear()
  {
    m_emitters.clear();
    m_updaters.clear();
    m_particleData.KillAll();
  }

  template<typename Real>
  void ParticleSystem<Real>::Update(Real a_dt)
  {
    //Update all particles
    for (int i = 0; i < m_updaters.size(); ++i)
    {
      m_updaters[i]->Update(m_particleData, 0,  a_dt);
    }

    //Emit new particles, keep tally of particles emitted
    //from the various emitters
    int nNewParticles = 0;
    for (int i = 0; i < m_emitters.size(); ++i)
    {
      nNewParticles += m_emitters[i]->EmitParticles(m_particleData, a_dt);
    }

    //Update all newly emitted particles. Since new particles are added to the end of the 
    //particle data, we simply update the last set of new particles.
    int startIndex = m_particleData.GetCountAlive() - nNewParticles;
    if (nNewParticles)
    {
      for (int u = 0; u < m_updaters.size(); ++u)
      {
        m_updaters[u]->UpdateNew(m_particleData, startIndex, a_dt);
      }
    }
  }
}

#endif
