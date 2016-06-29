//! @file DgParticleSystem.h
//!
//! @author Frank Hart
//! @date 4/10/2015

#ifndef DGPARTICLESYSTEM_H
#define DGPARTICLESYSTEM_H

#include "Dg_map.h"
#include "DgParticleEmitter.h"
#include "DgParticleUpdater.h"
#include "DgAttractor.h"

namespace Dg
{
  typedef unsigned ParMapKey;

  template<typename Real>
  class ParticleSystem
  {
  public:
    ParticleSystem(size_t);
    ~ParticleSystem() {}

    ParticleSystem(ParticleSystem<Real> const & a_other);
    ParticleSystem<Real> & operator=(ParticleSystem<Real> const & a_other);

    void AddEmitter(ParMapKey, ParticleEmitter<Real> const &);
    void AddUpdater(ParMapKey, ParticleUpdater<Real> const &);
    void AddAttractor(ParMapKey, Attractor<Real> const &);

  private:
    Dg::map<ParMapKey, ObjectWrapper<ParticleEmitter<Real>>>   m_emitters;
    ParticleData<Real, 0>                                      m_particleData;
    Dg::map<ParMapKey, ObjectWrapper<ParticleUpdater<Real>>>   m_updaters;
    Dg::map<ParMapKey, ObjectWrapper<Attractor<Real>>>         m_attractors;
  };

  template<typename Real>
  ParticleSystem<Real>::ParticleSystem(size_t a_nPar): m_particleData(a_nPar)
  {

  }


  template<typename Real>
  ParticleSystem<Real>::ParticleSystem(ParticleSystem<Real> const & a_other) 
    : m_particleData(a_other.m_particleData),
      m_emitters(a_other.m_emitters),
      m_updaters(a_other.m_updaters),
      m_attractors(a_other.m_attractors)
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
    m_attractors = a_other.m_attractors;
    m_particleData = a_other.m_particleData;

    return *this;
  }


  template<typename Real>
  void ParticleSystem<Real>::AddEmitter(ParMapKey a_key, ParticleEmitter<Real> const & a_emitter)
  {
    ObjectWrapper<ParticleEmitter<Real>> newEmitter(a_emitter);
    m_emitters.insert(a_key, newEmitter);
  }


  template<typename Real>
  void ParticleSystem<Real>::AddUpdater(ParMapKey a_key, ParticleUpdater<Real> const & a_updater)
  {
    ObjectWrapper<ParticleUpdater<Real>> newUpdater(a_updater);
    m_updaters.insert(a_key, newUpdater);
  }


  template<typename Real>
  void ParticleSystem<Real>::AddAttractor(ParMapKey a_key, Attractor<Real> const & a_attractor)
  {
    ObjectWrapper<Attractor<Real>> newAttractor(a_attractor);
    m_attractors.insert(a_key, newAttractor);
  }
}

#endif
