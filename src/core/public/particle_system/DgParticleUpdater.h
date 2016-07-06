//! @file DgParticleUpdater.h
//!
//! @author Frank Hart
//! @date 4/10/2015

#ifndef DGPARTICLEUPDATER_H
#define DGPARTICLEUPDATER_H

#include "DgObject.h"

namespace Dg
{
  template<typename Real>
  class ParticleData;

  template<typename Real>
  class ParticleUpdater : public Object<ParticleUpdater<Real>>
  {
  public:
    ParticleUpdater() {}
    virtual ~ParticleUpdater() {}

    ParticleUpdater(ParticleUpdater<Real> const & a_other) {}
    ParticleUpdater<Real> & operator=(ParticleUpdater<Real> const & a_other) { return *this; }
    

    //A secondary update method, specially called for newy created particles.
    virtual void UpdateNew(ParticleData<Real> & data
				                 , int start
                         , Real dt) {}

    virtual void Update(ParticleData<Real> & data
				              , int start
                      , Real dt) {}

    virtual ParticleUpdater<Real> * Clone() const { return new ParticleUpdater<Real>(*this); }
  };
}

#endif
