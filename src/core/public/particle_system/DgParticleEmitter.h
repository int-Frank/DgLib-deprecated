//! @file DgParticleEmitter.h
//!
//! @author Frank Hart
//! @date 4/10/2015

#ifndef DGPARTICLEEMITTER_H
#define DGPARTICLEEMITTER_H

#include "Dg_map.h"
#include "DgObjectWrapper.h"
#include "DgParticleGenerator.h"

namespace Dg
{
  typedef unsigned ParMapKey;

  template<typename Real>
  class ParticleData;

  template<typename Real>
  class ParticleEmitter : public Object<ParticleEmitter<Real>>
  {
  public:
    ParticleEmitter(): m_isOn(false) {}
    virtual ~ParticleEmitter() {}

    ParticleEmitter(ParticleEmitter<Real> const & a_other): m_isOn(a_other.m_isOn) {}
    ParticleEmitter<Real> & operator=(ParticleEmitter<Real> const & a_other) 
    { 
      m_isOn = a_other.m_isOn;
      return *this; 
    }

    virtual void SetRate(Real) {}

    //! Returns number new particles created. These will be at the
    //! end of the particle data. 
    virtual int EmitParticles(Real dt, ParticleData<Real> & data) { return 0; }

    void Start() const { m_isOn = true; }
    void Stop() const { m_isOn = false; }
    void ToggleOn() const { m_isOn = !m_isOn; }
    void IsOn() const { return m_isOn; }

    void AddGenerator(ParMapKey, ParticleGenerator<Real> const &);
    ParticleGenerator<Real> * GetGenerator(ParMapKey) const;

    virtual ParticleEmitter<Real> * Clone() const { return new ParticleEmitter<Real>(*this); }

  private:
    bool m_isOn;
    Dg::map<ParMapKey, ObjectWrapper<ParticleGenerator<Real>>>   m_generators;
  
  };

  template<typename Real>
  void ParticleEmitter<Real>::AddGenerator(ParMapKey a_key, ParticleGenerator<Real> const & a_gen)
  {
    ObjectWrapper<ParticleEmitter<Real>> newGenerator(a_gen);
    m_generators.insert(a_key, newGenerator);
  }

  template<typename Real>
  ParticleGenerator<Real> * ParticleEmitter<Real>::GetGenerator(ParMapKey a_key) const
  {
    int index = 0;
    auto it = m_generators.find(a_key);
    if (!m_generators.find(a_key, index))
    {
      return nullptr;
    }
    return &m_generators[index];
  }
}

#endif
