//! @file DgAttractor.h
//!
//! @author: Frank B. Hart
//! @date 29/06/2016
//!
//! Class declaration: ObjectWrapper

#ifndef DGATTRACTOR_H
#define DGATTRACTOR_H

#include "..\DgVector4.h"
#include "..\DgVQS.h"
#include "..\DgObject.h"

namespace Dg
{
  struct AttractorForce
  {
    enum
    {
      Constant,
      Linear,
      InverseSquare
    };
  };

  //! @ingroup DgEngine_PS
  //!
  //! @class Attrator
  //!
  //! An attractor is a region in space which provides some force on particles.
  //! It could be defined as a point, line or surface.
  //!
  //! @author Frank Hart
  //! @date 29/06/2016
  template<typename Real, unsigned Force = AttractorForce::Constant>
  class Attractor : public Object<Attractor<Real, Force>>
  {
  public:

    Attractor();
    virtual ~Attractor() {}

    Attractor(Attractor<Real, Force> const &);
    Attractor<Real, Force> & operator=(Attractor<Real, Force> const &);

    //! Update the attractor based on some change in time. For example,
    //! Some attractors may pulse at some frequency.
    virtual void Update(Real dt) {}

    //! Set the location and size (if applicable) of he attractor.
    virtual void SetTransformation(VQS<Real> const &) {}

    //! Get the accelleration of a particle to this attractor
    virtual Vector4<Real> GetAccelVector(Vector4<Real> const & a_particle) { return Vector4<Real>::ZeroVector(); }

    //! Set the strength of the Attractor
    virtual void SetStrength(Real a_str) { m_strength = a_str; }

    //! Set the maximum magnitue of the acceleration vector this attractor applies to a particle.
    virtual void SetMaxAccelMagnitude(Real a_val) { m_maxAccelMag = a_val; }

    virtual Attractor* Clone() const { return new Attractor<Real, Force>(*this); }
  protected:

    Vector4<Real> GetAccelVector(Vector4<Real> const & p0, Vector4<Real> const & p1) const;

  protected:

    Real m_strength;
  private:

    Real m_maxAccelMag;
  };

  template<typename Real, unsigned Force>
  Attractor<Real, Force>::Attractor()
    : m_strength(static_cast<Real>(0.0)), m_maxAccelMag(static_cast<Real>(10.0))
  {

  }

  template<typename Real, unsigned Force>
  Attractor<Real, Force>::Attractor(Attractor<Real, Force> const & a_other)
    : m_strength(a_other.m_strength), m_maxAccelMag(a_other.m_maxAccelMag)
  {

  }

  template<typename Real, unsigned Force>
  Attractor<Real, Force> & Attractor<Real, Force>::operator=(Attractor<Real, Force> const & a_other)
  {
    m_strength = a_other.m_strength;
    m_maxAccelMag = a_other.m_maxAccelMag;

    return *this;
  }


  template<typename Real, unsigned Force>
  Vector4<Real> Attractor<Real, Force>::GetAccelVector(Vector4<Real> const & p0, Vector4<Real> const & p1) const
  {
    if (Force == AttractorForce::Constant)
    {
      return static_cast<Real>(1.0);
    }

    Vector4<Real> v(p0 - p1);
    Real dist = v.Length();
    if (Dg::IsZero(dist))
    {
      return m_maxAccelMag;
    }

    Real invDist = static_cast<Real>(1.0) / dist;
    Real accelMag;

    if (Force == AttractorForce::Linear)
    {
      accelMag = m_strength * invDist;
    }
    else
    {
      accelMag = m_strength * invDist * invDist;
    }

    accelMag = (acclMag > m_maxAccelMag) ? m_maxAccelMag : accelMag;
    return v * (invDist * accelMag);
  }
}

#endif