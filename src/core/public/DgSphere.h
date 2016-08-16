//! @file DgSphere.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Sphere

#ifndef DGSPHERE_H
#define DGSPHERE_H

#include "DgVector4.h"
#include "DgVQS.h"
#include "dgmath.h"
#include "DgRNG.h"

//TODO Queries
namespace Dg
{

  //! @ingroup DgMath_types
  //!
  //! @class Sphere
  //!
  //! A sphere can be defined by the set of all points p at distance <= r away from 
  //! a central point c. It is implemented simply by defining a point and radius
  //!
  //! @author: Frank Hart
  //! @date 20/06/2016
  template<typename Real>
  class Sphere
  {
  public:
    //! Default constructor.
    Sphere() : m_origin(Vector4<Real>::Origin()),
               m_radius(static_cast<Real>(1.0)) {}

    //! Construct sphere from origin and radius
    Sphere(Vector4<Real> const &, Real);
    ~Sphere() {}

    //! Copy constructor
    Sphere(Sphere const & a_other)
      : m_origin(a_other.m_origin), m_radius(a_other.m_radius) {}

    //! Assignment
    Sphere& operator= (Sphere const &);

    //! Get the sphere center.
    Vector4<Real> const & Center() const { return m_origin; }

    //! Get the sphere radius.
    Real Radius() const { return m_radius; }

    //! Get the plane center and .
    void Get(Vector4<Real>&, Real&) const;

    //! Are two planes equal?
    bool operator== (Sphere const &) const;

    //! Are two planes not equal?
    bool operator!= (Sphere const &) const;

    //! Set the sphere center
    void SetCenter(Vector4<Real> const &);

    //! Set the sphere radius
    void SetRadius(Real);

    //! Set the center and radius.
    void Set(Vector4<Real> const & n, Real);

    //! Transform the sphere
    Sphere GetTransformed(VQS<Real> const &) const;

    //! Transform the sphere, assign to self
    Sphere & TransformSelf(VQS<Real> const &);

    //! Get a random point inside the sphere
    Vector4<Real> GetRandomPointInside() const;

  private:
    Vector4<Real>   m_origin;
    Real            m_radius;
  };


  //--------------------------------------------------------------------------------
  //	@	Sphere::Sphere()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Sphere<Real>::Sphere(Vector4<Real> const & a_center, Real a_radius)
  {
    Set(a_center, a_radius);
  }	//End: Sphere::Sphere()


  //--------------------------------------------------------------------------------
  //	@	Sphere::operator=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Sphere<Real> & Sphere<Real>::operator=(Sphere<Real> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_radius = a_other.m_radius;
  }	//End: Sphere::operator=()


  //--------------------------------------------------------------------------------
  //	@	Sphere::Set()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Sphere<Real>::Set(Vector4<Real> const & a_center, Real a_radius)
  {
    m_origin = a_center;
    m_origin.w() = static_cast<Real>(1.0);

    if (a_radius < static_cast<Real>(0.0))
    {
      m_radius = static_cast<Real>(0.0);
    }
    else
    {
      m_radius = a_radius;
    }
  }	//End: Plane4::Set()


  //--------------------------------------------------------------------------------
  //	@	Sphere::operator==()
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool Sphere<Real>::operator==(Sphere<Real> const & a_other) const
  {
    return (m_origin == a_other.m_origin &&
            Dg::AreEqual(m_radius, a_other.m_radius));
  }	//End: Sphere::operator==()


  //--------------------------------------------------------------------------------
  //	@	Sphere::operator!=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool Sphere<Real>::operator!=(Sphere<Real> const & a_other) const
  {
    return (m_origin != a_other.m_origin ||
            !Dg::AreEqual(m_radius, a_other.m_radius));
  }	//End: Sphere::operator!=()


  //--------------------------------------------------------------------------------
  //	@	Sphere::SetCenter()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Sphere<Real>::SetCenter(Vector4<Real> const & a_center)
  {
    m_origin = a_center;
    m_origin.w() = static_cast<Real>(1.0);
  }	//End: Sphere::SetCenter()


  //--------------------------------------------------------------------------------
  //	@	Sphere::SetRadius()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Sphere<Real>::SetRadius(Real a_radius)
  {
    m_radius = (a_radius < static_cast<Real>(0.0)) ? static_cast<Real>(0.0) : a_radius;
  }	//End: Sphere::SetRadius()


  //--------------------------------------------------------------------------------
  //	@	Sphere::Get()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Sphere<Real>::Get(Vector4<Real> & a_center, Real & a_radius) const
  {
    a_center = m_origin;
    a_radius = m_radius;
  }	//End: Sphere::Get()


  //--------------------------------------------------------------------------------
  //	@	Sphere::GetTransformed()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Sphere<Real> Sphere<Real>::GetTransformed(VQS<Real> const & a_vqs) const
  {
    return Sphere<Real>(a_vqs.TransformPoint(m_origin), a_vqs.S() * m_radius);
  }	//End: Sphere::GetTransformed()


  //--------------------------------------------------------------------------------
  //	@	Sphere::TransformSelf()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Sphere<Real>& Sphere<Real>::TransformSelf(VQS<Real> const & a_vqs)
  {
    Set(a_vqs.TransformPoint(m_origin), a_vqs.S() * m_radius);
    return *this;
  }	//End: Sphere::TransformSelf()


  //--------------------------------------------------------------------------------
  //	@	Sphere::GetRandomPointInside()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Sphere<Real>::GetRandomPointInside() const
  {
    Vector4<Real> result(m_origin);
    if (!Dg::IsZero(m_radius))
    {
      Vector4<Real> diff;
      RNG rng;
      do
      {
        Real x = rng.GetUniform(m_origin.x() - m_radius, m_origin.x() + m_radius);
        Real y = rng.GetUniform(m_origin.y() - m_radius, m_origin.y() + m_radius);
        Real z = rng.GetUniform(m_origin.z() - m_radius, m_origin.z() + m_radius);
        result.Set(x, y, z, static_cast<Real>(1.0));
        diff = (result - m_origin);
      } while (diff.Dot(diff) > m_radius * m_radius);
    }
    return result;
  }	//End: Sphere::GetRandomPointInside()
}

#endif