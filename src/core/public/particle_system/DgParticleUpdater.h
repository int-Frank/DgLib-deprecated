//! @file DgParticleUpdater.h
//!
//! @author Frank Hart
//! @date 4/10/2015

#ifndef DGPARTICLEUPDATER_H
#define DGPARTICLEUPDATER_H

#include "DgObject.h"

namespace Dg
{
  template<typename Real, uint32_t>
  class ParticleData;

  template<typename Real>
  class ParticleUpdater : public Object<ParticleUpdater<Real>>
  {
  public:
    ParticleUpdater() {}
    virtual ~ParticleUpdater() {}

    ParticleUpdater(ParticleUpdater<Real> const & a_other) {}
    ParticleUpdater<Real> & operator=(ParticleUpdater<Real> const & a_other) { return *this; }

    virtual void Update(size_t start,
                        size_t finish,
                        Real dt,
                        ParticleData<Real> & data) {}

    virtual ParticleUpdater<Real> * Clone() const { return new ParticleUpdater<Real>(*this); }
  };
}

#endif
