//! @file DgParticleUpdater.h
//!
//! @author Frank Hart
//! @date 23/07/2016
//!
//! Class declatation: ParticleUpdater

#ifndef DGPARTICLEUPDATER_H
#define DGPARTICLEUPDATER_H

#include "DgObject.h"

namespace Dg
{
  template<typename Real>
  class ParticleData;

  //! @ingroup DgEngine_ParticleSystem
  //!
  //! @class ParticleUpdater
  //!
  //! Particle updaters are iterate over active particles and update
  //! attributes.
  //!
  //! @author Frank Hart
  //! @date 23/07/2016
  template<typename Real>
  class ParticleUpdater : public Object<ParticleUpdater<Real>>
  {
  public:
    ParticleUpdater() {}
    virtual ~ParticleUpdater() {}

    ParticleUpdater(ParticleUpdater<Real> const & a_other) {}
    ParticleUpdater<Real> & operator=(ParticleUpdater<Real> const & a_other) { return *this; }
    

    //! A secondary update method, specially called for newy created particles.
    //!
    //! @param[in] data Particle data to modify
    //! @param[in] start Index in the particle data to start at. Updaters should
    //!            run to the end of active particles.
    //! @param[in] dt Time slice
    virtual void UpdateNew(ParticleData<Real> & data
				                 , int start
                         , Real dt) {}

    //! Primary update call.
    //!
    //! @param[in] data Particle data to modify
    //! @param[in] start Index in the particle data to start at. Updaters should
    //!            run to the end of active particles.
    //! @param[in] dt Time slice
    virtual void Update(ParticleData<Real> & data
				              , int start
                      , Real dt) {}

    //! Create a deep copy of this object.
    virtual ParticleUpdater<Real> * Clone() const { return new ParticleUpdater<Real>(*this); }
  };
}

#endif
