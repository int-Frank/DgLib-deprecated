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
  //! @ingroup DgEngine_PS
  //!
  //! @class Attrator
  //!
  //! An attractor is a region in space which provides some force on particles.
  //! It could be defined as a point, line or surface.
  //!
  //! @author Frank Hart
  //! @date 29/06/2016
  template<typename Real>
  class Attractor : public ParticleUpdater<Real>
  {
  public:

    enum Force
    {
      Constant,
      Linear,
      InvSq
    };

  public:

    Attractor();
    virtual ~Attractor() {}

    Attractor(Attractor<Real> const &);
    Attractor<Real> & operator=(Attractor<Real> const &);

    //! Update the attractor based on some change in time. For example,
    //! Some attractors may pulse at some frequency.
    virtual void UpdateAttractor(Real dt) {}

    virtual void UpdateNew(ParticleData<Real> & data
				                 , int start
                         , Real dt) {}

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
    virtual void SetMaxAppliedAccelMagnitude(Real a_val);

    void SetAccelType(int);
    int GetAccelType() const { return m_accelType; }

    virtual Attractor<Real> * Clone() const { return new Attractor<Real>(*this); }
  
  protected:

    Vector4<Real> GetAccel_Constant(Vector4<Real> const & p0
                                  , Vector4<Real> const & p1) const;

    Vector4<Real> GetAccel_Linear(Vector4<Real> const & p0
                                , Vector4<Real> const & p1) const;

    Vector4<Real> GetAccel_InvSq(Vector4<Real> const & p0
                               , Vector4<Real> const & p1) const;
 
  protected:
    int   m_accelType;
    Real  m_strength;
    Real  m_maxAppliedAccel;
  };

  template<typename Real>
  Attractor<Real>::Attractor()
    : m_accelType(Constant)
    , m_strength(static_cast<Real>(1.0))
    , m_maxAppliedAccel(static_cast<Real>(10.0))
  {

  }

  template<typename Real>
  Attractor<Real>::Attractor(Attractor<Real> const & a_other)
    : m_accelType(a_other.m_accelType)
    , m_strength(a_other.m_strength)
    , m_maxAppliedAccel(a_other.m_maxAppliedAccel)
  {

  }

  template<typename Real>
  Attractor<Real> & Attractor<Real>::operator=(Attractor<Real> const & a_other)
  {
    m_accelType = a_other.m_accelType;
    m_strength = a_other.m_strength;
    m_maxAppliedAccel = a_other.m_maxAppliedAccel;

    return *this;
  }

  template<typename Real>
  void Attractor<Real>::SetMaxAppliedAccelMagnitude(Real a_val)
  {
    m_maxAppliedAccel = (a_val < static_cast<Real>(0.0)) ? static_cast<Real>(0.0) : a_val;
  }

  template<typename Real>
  void Attractor<Real>::SetAccelType(int a_val)
  {
    switch (a_val)
    {
    case Linear:
    {
      m_accelType = Linear;
      break;
    }
    case InvSq:
    {
      m_accelType = InvSq;
      break;
    }
    default: //Constant
    {
      m_accelType = Constant;
      break;
    }
    }
  }

  template<typename Real>
  Vector4<Real> Attractor<Real>::GetAccel_Constant(Vector4<Real> const & a_p0
                                                 , Vector4<Real> const & a_p1) const
  {
    Vector4<Real> v(a_p1 - a_p0);
    Real dist = v.Length();
    if (Dg::IsZero(dist))
    {
      v = GetRandomVector<Real>();
    }
    return v / dist * ((m_strength <= m_maxAppliedAccel) ? m_strength : m_maxAppliedAccel);
  }

  template<typename Real>
  Vector4<Real> Attractor<Real>::GetAccel_Linear(Vector4<Real> const & a_p0
                                               , Vector4<Real> const & a_p1) const
  {
    Vector4<Real> v(a_p1 - a_p0);

    Real sqDist = v.LengthSquared();
    Real invSqDist;
    if (Dg::IsZero(sqDist))
    {
      v = GetRandomVector<Real>();
      invSqDist = static_cast<Real>(999999999999999.0);
    }
    else
    {
      invSqDist = static_cast<Real>(1.0) / sqDist;
    }

    v *= invSqDist * m_strength;
    if (v.LengthSquared() > m_maxAppliedAccel * m_maxAppliedAccel)
    {
      v.Normalize();
      v *= m_maxAppliedAccel;
    }
    
    return v;
  }

  template<typename Real>
  Vector4<Real> Attractor<Real>::GetAccel_InvSq(Vector4<Real> const & a_p0
                                              , Vector4<Real> const & a_p1) const
  {
    Vector4<Real> v(a_p1 - a_p0);

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
    if (mag * mag > m_maxAppliedAccel * m_maxAppliedAccel)
    {
      mag = (mag < static_cast<Real>(0.0)) ? -m_maxAppliedAccel : m_maxAppliedAccel;
    }
    return v * invDist * mag;
  }
}

#endif