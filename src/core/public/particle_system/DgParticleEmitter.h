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

  template<typename Real, uint32_t>
  class ParticleData;

  template<typename Real>
  class ParticleEmitter : public Object<ParticleEmitter<Real>>
  {
  public:
    ParticleEmitter() {}
    virtual ~ParticleEmitter() {}

    ParticleEmitter(ParticleEmitter<Real> const & a_other) {}
    ParticleEmitter<Real> & operator=(ParticleEmitter<Real> const & a_other) { return *this; }

    //! Returns number new particles created. These will be at the
    //! end of the particle data. 
    virtual size_t EmitParticles(Real dt, ParticleData<Real> & data) { return 0; }

    void AddGenerator(ParMapKey, ParticleGenerator<Real> const &);
    ParticleGenerator<Real> * GetGenerator(ParMapKey) const;

    virtual ParticleEmitter<Real> * Clone() const { return new ParticleEmitter<Real>(*this); }

  private:
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
