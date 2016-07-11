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
#include "DgParticleUpdater.h"

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
  class Attractor : public ParticleUpdater<Real>
  {
  public:

    Attractor();
    virtual ~Attractor() {}

    Attractor(Attractor<Real, Force> const &);
    Attractor<Real, Force> & operator=(Attractor<Real, Force> const &);

    //! Update the attractor based on some change in time. For example,
    //! Some attractors may pulse at some frequency.
    virtual void UpdateAttractor(Real dt) {}

    virtual void Update(ParticleData<Real> & data
				              , int start) {}

    virtual void Update(ParticleData<Real> & data
				              , int start
                      , Real dt) {}

    //! Set the location and size (if applicable) of the attractor.
    virtual void SetTransformation(VQS<Real> const &) {}

    //! Set the strength of the Attractor
    virtual void SetStrength(Real a_str) { m_strength = a_str; }

    //! Get the strength of the Attractor
    virtual Real GetStrength() const { return m_strength; }

    //! Set the maximum allowed magnitue of the acceleration vector this attractor applies to a particle.
    virtual void SetMaxAccelMagnitude(Real a_val);

    virtual Attractor<Real, Force> * Clone() const { return new Attractor<Real, Force>(*this); }
  
  protected:

    Vector4<Real> GetAccelBetweenPoints(Vector4<Real> const & p0
                                      , Vector4<Real> const & p1) const;

  protected:

    Real m_strength;
    Real m_maxAccelMag;
  };

  template<typename Real, unsigned Force>
  Attractor<Real, Force>::Attractor()
    : m_strength(static_cast<Real>(1.0)), m_maxAccelMag(static_cast<Real>(10.0))
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
  void Attractor<Real, Force>::SetMaxAccelMagnitude(Real a_val)
  {
    m_maxAccelMag = (a_val < static_cast<Real>(0.0)) ? static_cast<Real>(0.0) : a_val;
  }

  template<typename Real, unsigned Force>
  Vector4<Real> Attractor<Real, Force>::GetAccelBetweenPoints(Vector4<Real> const & a_p0
                                                            , Vector4<Real> const & a_p1) const
  {
    Vector4<Real> v(a_p1 - a_p0);

    if (Force == AttractorForce::Constant)
    {
      Real dist = v.Length();
      if (Dg::IsZero(dist))
      {
        v = GetRandomVector<Real>();
      }
      v = v / dist * m_strength;
    }
    else if (Force == AttractorForce::Linear)
    {
      Real sqDist = v.LengthSquared();
      Real invSqDist;
      if (Dg::IsZero(sqDist))
      {
        v = GetRandomVector<Real>();
        invSqDist = static_cast<Real>(999999999999999.0);
      }
      else
      {
        invSqDist = static_cast<Real>(1.0) / invSqDist;
      }

      v *= invSqDist * m_strength;
      if (v.LengthSquared() > m_maxAccelMag * m_maxAccelMag)
      {
        v.Normalize();
        v *= m_maxAccelMag;
      }
    }
    else
    {
      Real dist = v.Length();
      if (Dg::IsZero(dist))
      {
        v = GetRandomVector<Real>();
      }

      Real invDist;
      if (Dg::IsZero(dist))
      {
        v = GetRandomVector<Real>();
        invDist = static_cast<Real>(999999999999999.0);
      }
      else
      {
        invDist = static_cast<Real>(1.0) / dist;
      }

      Real mag = m_strength * invDist * invDist;
      if (mag > m_maxAccelMag)
      {
        mag = m_maxAccelMag;
      }
      v = v * invDist * mag;
    }

    return v;
  }
}

#endif