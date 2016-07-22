//! @file DgParticleGenerator.h
//!
//! @author Frank Hart
//! @date 4/10/2015

#ifndef DGPARTICLEGENERATOR_H
#define DGPARTICLEGENERATOR_H

#include "DgObject.h"
#include "DgVQS.h"

//TODO Documentation
namespace Dg
{
  template<typename Real>
  class ParticleData;

  template<typename Real>
  class ParticleGenerator : public Object<ParticleGenerator<Real>>
  {
  public:
    ParticleGenerator() {}
    virtual ~ParticleGenerator() {}

    ParticleGenerator(ParticleGenerator<Real> const & a_other) {}
    ParticleGenerator<Real> & operator=(ParticleGenerator<Real> const & a_other) { return *this; }

    //! Set the location and size (if applicable) of the attractor.
    virtual void SetTransformation(VQS<Real> const &) {}

    virtual void Generate(ParticleData<Real> &, int, int) {}
    virtual ParticleGenerator<Real> * Clone() const { return new ParticleGenerator<Real>(*this); }
  };
}

#endif
