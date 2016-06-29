//! @file DgParticleGenerator.h
//!
//! @author Frank Hart
//! @date 4/10/2015

#ifndef DGPARTICLEGENERATOR_H
#define DGPARTICLEGENERATOR_H

#include "DgObject.h"

namespace Dg
{
  template<typename Real, uint32_t>
  class ParticleData;

  template<typename Real>
  class ParticleGenerator : public Object<ParticleGenerator<Real>>
  {
  public:
    ParticleGenerator() {}
    virtual ~ParticleGenerator() {}

    ParticleGenerator(ParticleGenerator<Real> const & a_other) {}
    ParticleGenerator<Real> & operator=(ParticleGenerator<Real> const & a_other) { return *this; }

    virtual void Generate(size_t, size_t, ParticleData<Real> &) {}
    virtual ParticleGenerator<Real> * Clone() const { return new ParticleGenerator<Real>(*this); }
  };
}

#endif
