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
    ParticleEmitter(): m_isOn(false), m_generators(32){}
    virtual ~ParticleEmitter() {}

    ParticleEmitter(ParticleEmitter<Real> const & a_other)
      : m_isOn(a_other.m_isOn)
      , m_generators(a_other.m_generators){}

    ParticleEmitter<Real> & operator=(ParticleEmitter<Real> const & a_other) 
    { 
      m_isOn = a_other.m_isOn;
      m_generators = a_other.m_generators;
      return *this; 
    }

    virtual void SetRate(Real) {}

    //! Returns number new particles created. These will be at the
    //! end of the particle data. 
    virtual int EmitParticles(ParticleData<Real> & data, Real dt) { return 0; }

    void Start() { m_isOn = true; }
    void Stop() { m_isOn = false; }
    void ToggleOn() { m_isOn = !m_isOn; }
    bool IsOn() const { return m_isOn; }

    void AddGenerator(ParMapKey, ParticleGenerator<Real> const &);
    void RemoveGenerator(ParMapKey);
    ParticleGenerator<Real> * GetGenerator(ParMapKey);

    virtual ParticleEmitter<Real> * Clone() const { return new ParticleEmitter<Real>(*this); }

  protected:
    Dg::map<ParMapKey, ObjectWrapper<ParticleGenerator<Real>>>   m_generators;

  private:
    bool m_isOn;
  };

  template<typename Real>
  void ParticleEmitter<Real>::AddGenerator(ParMapKey a_key, ParticleGenerator<Real> const & a_gen)
  {
    ObjectWrapper<ParticleGenerator<Real>> newGenerator(a_gen);
    m_generators.insert(a_key, newGenerator);
  }

  template<typename Real>
  void ParticleEmitter<Real>::RemoveGenerator(ParMapKey a_key)
  {
    m_generators.erase(a_key);
  }

  template<typename Real>
  ParticleGenerator<Real> * ParticleEmitter<Real>::GetGenerator(ParMapKey a_key)
  {
    int index = 0;
    if (!m_generators.find(a_key, index))
    {
      return nullptr;
    }
    return m_generators[index];
  }
}

#endif
