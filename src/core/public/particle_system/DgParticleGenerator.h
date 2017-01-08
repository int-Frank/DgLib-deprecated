//! @file DgParticleGenerator.h
//!
//! @author Frank Hart
//! @date 23/07/2016
//!
//! Class declaration: ParticleGenerator

#ifndef DGPARTICLEGENERATOR_H
#define DGPARTICLEGENERATOR_H

#include "DgObject.h"
#include "DgR3VQS.h"


namespace Dg
{
  template<typename Real>
  class ParticleData;

  //! @ingroup DgEngine_ParticleSystem
  //!
  //! @class ParticleGenerator
  //!
  //! Particle generators are responsible for initializing one 
  //! or more attributes for new particles. 
  //!
  //! @author Frank Hart
  //! @date 23/07/2016
  template<typename Real>
  class ParticleGenerator : public Object<ParticleGenerator<Real>>
  {
  public:
    ParticleGenerator() {}
    virtual ~ParticleGenerator() {}

    ParticleGenerator(ParticleGenerator<Real> const & a_other) {}
    ParticleGenerator<Real> & operator=(ParticleGenerator<Real> const & a_other) { return *this; }

    //! Set any transformation if needed
    virtual void SetTransformation(R3::VQS<Real> const &) {}

    //! Iterate over a set of particles in the particle data and
    //! initialize attributes
    virtual void Generate(ParticleData<Real> &, int, int) {}

    //! Create a deep copy of this object.
    virtual ParticleGenerator<Real> * Clone() const { return new ParticleGenerator<Real>(*this); }
  };
}

#endif
