//! @file DgAttractor.h
//!
//! @author: Frank B. Hart
//! @date 22/07/2016
//!
//! Class declaration: Attractor

#ifndef DGATTRACTOR_H
#define DGATTRACTOR_H

#include "..\DgVector4.h"
#include "..\DgVQS.h"
#include "DgParticleUpdater.h"
#include "DgMath.h"

namespace Dg
{
  //! @ingroup DgEngine_ParticleSystem
  //!
  //! @class Attractor
  //!
  //! An attractor is a region in space which pulls or pushes particles.
  //! It could be defined as a point, line or surface.
  //!
  //! @author Frank Hart
  //! @date 29/06/2016
  template<typename Real>
  class Attractor : public ParticleUpdater<Real>
  {
  public:
    
    //! These options describe how the force applied particles
    //! diminishes over distance. 
    enum ForceAttenuation
    {
      Constant,       //The force does not diminish with distance.
      Inverse,        //The force diminishes at a rate proportional to the inverse of the distance from the attractor.
      InverseSquare   //The force diminishes at a rate proportional to the inverse square of the distance from the attractor.
    };

  public:

    Attractor();
    virtual ~Attractor() {}

    Attractor(Attractor<Real> const &);
    Attractor<Real> & operator=(Attractor<Real> const &);

    //! Update the attractor based on some change in time. For example,
    //! Some attractors may pulse at some frequency.
    virtual void UpdateAttractor(Real dt) {}

    //! A secondary update method, called for newy created particles.
    virtual void UpdateNew(ParticleData<Real> & data
				                 , int start
                         , Real dt) {}

    //! Apply force and update the particles.
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

    //! Set at what rate the force of the attractor attenuates with distance.
    void SetAttenuationMethod(int);

    //! Query the attenuation type.
    int GetAttenuationMethod() const { return m_attenuation; }

    //! Create a deep copy of this object.
    virtual Attractor<Real> * Clone() const { return new Attractor<Real>(*this); }
  
  protected:

    Vector4<Real> GetAccel_Constant(Vector4<Real> const & p0
                                  , Vector4<Real> const & p1) const;

    Vector4<Real> GetAccel_Inverse(Vector4<Real> const & p0
                                 , Vector4<Real> const & p1) const;

    Vector4<Real> GetAccel_InverseSquare(Vector4<Real> const & p0
                                       , Vector4<Real> const & p1) const;
 
  protected:
    int   m_attenuation;
    Real  m_strength;
    Real  m_maxAppliedAccel;
  };


  //--------------------------------------------------------------------------------
  //	@	Attractor::Attractor()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Attractor<Real>::Attractor()
    : m_attenuation(Constant)
    , m_strength(static_cast<Real>(1.0))
    , m_maxAppliedAccel(static_cast<Real>(10.0))
  {

  } //End: Attractor::Attractor()

  
  //--------------------------------------------------------------------------------
  //	@	Attractor::Attractor()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Attractor<Real>::Attractor(Attractor<Real> const & a_other)
    : m_attenuation(a_other.m_attenuation)
    , m_strength(a_other.m_strength)
    , m_maxAppliedAccel(a_other.m_maxAppliedAccel)
  {

  } //End: Attractor::Attractor()

  
  //--------------------------------------------------------------------------------
  //	@	Attractor::operator=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Attractor<Real> & Attractor<Real>::operator=(Attractor<Real> const & a_other)
  {
    m_attenuation = a_other.m_attenuation;
    m_strength = a_other.m_strength;
    m_maxAppliedAccel = a_other.m_maxAppliedAccel;

    return *this;
  } //End: Attractor::operator=()

  
  //--------------------------------------------------------------------------------
  //	@	Attractor::SetMaxAppliedAccelMagnitude()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Attractor<Real>::SetMaxAppliedAccelMagnitude(Real a_val)
  {
    m_maxAppliedAccel = (a_val < static_cast<Real>(0.0)) ? static_cast<Real>(0.0) : a_val;
  } //End: Attractor::SetMaxAppliedAccelMagnitude()


  //--------------------------------------------------------------------------------
  //	@	Attractor::SetAttenuationMethod()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Attractor<Real>::SetAttenuationMethod(int a_val)
  {
    if (a_val >= ForceAttenuation::Constant && a_val <= ForceAttenuation::InverseSquare)
    {
      m_attenuation = a_val;
    }
    else
    {
      m_attenuation = Inverse;
    }
  } //End: Attractor::SetAttenuationMethod()

  
  //--------------------------------------------------------------------------------
  //	@	Attractor::GetAccel_Constant()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Attractor<Real>::GetAccel_Constant(Vector4<Real> const & a_p0
                                                 , Vector4<Real> const & a_p1) const
  {
    Vector4<Real> v(a_p1 - a_p0);
    Real dist = v.Length();
    if (Dg::IsZero(dist))
    {
      v = GetRandomVector<Real>();
      dist = static_cast<Real>(1.0);
    }
    Real str(m_strength);
    ClampNumber(-m_maxAppliedAccel, m_maxAppliedAccel, str);
    return v / dist * str;
  } //End: Attractor::GetAccel_Constant()


  //--------------------------------------------------------------------------------
  //	@	Attractor::GetAccel_Inverse()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Attractor<Real>::GetAccel_Inverse(Vector4<Real> const & a_p0
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
  } //End: Attractor::GetAccel_Inverse()


  //--------------------------------------------------------------------------------
  //	@	Attractor::GetAccel_InverseSquare()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Attractor<Real>::GetAccel_InverseSquare(Vector4<Real> const & a_p0
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
  } //End: Attractor::GetAccel_InverseSquare()
}

#endif