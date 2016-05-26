//================================================================================
// @ Plane.h
// 
// Class: Plane
//
// A Plane is infinite flat surface. It is implemented by using the generalized
// plane equation: Ax + By + Cz + D = 0, or a vector m_normal to the plane
// and an m_offset.
//
// -------------------------------------------------------------------------------
//
// Original Authors: James M. Van Verth, Lars M. Bishop
// Retrieved From: Essential Mathematics for Games and Interactive Applications SE
// On Date: 2013
//
// Modified by: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef DGPLANE_H
#define DGPLANE_H

#include "DgVector4.h"
#include "dgmath.h"

namespace Dg
{

  //--------------------------------------------------------------------------------
  //	@	Plane
  //--------------------------------------------------------------------------------
  template<typename Real>
  class Plane
  { 
  public:
    //constructor/destructor
    Plane() : m_normal(Vector4<Real>::xAxis()),
              m_offset(static_cast<Real>(0.0)) {}
    Plane(Real a, Real b, Real c, Real d);
    Plane(Vector4 const & p0, Vector4 const & p1, Vector4 const & p2);
    Plane(Vector4 const & n, Real d);
    ~Plane() {}

    //copy operations
    Plane(Plane const &);
    Plane& operator= (Plane const &);

    //accessors
    Vector4 const & Normal() const { return m_normal; }
    Real Offset() const { return m_offset; }
    void Get(Vector4& normal, Real& offset) const;

    //comparison
    bool operator== (Plane const &) const;
    bool operator!= (Plane const &) const;

    //manipulators
    void Set(Real a, Real b, Real c, Real d);
    void Set(Vector4 const & n, Real d);
    void Set(Vector4 const & n, Vector4 const & p);
    void Set(Vector4 const & p0, Vector4 const & p1, Vector4 const & p2);

    //Signed distance from point to plane
    //Negative is behind the m_normal vector
    //Positive is in front of m_normal vector
    Real SignedDistance(Vector4 const & a_point) const;

    //Distance from point to plane
    Real Distance(Vector4 const & a_point) const;

    //! Test a vector against the plane normal
    Real NormalDot(Vector4 const & a_v) const;

    Vector4 ClosestPoint(Vector4 const &) const;

  private:
    Vector4 m_normal;
    Real m_offset;
  };

  //--------------------------------------------------------------------------------
  //		@ Plane::SignedDistance()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real Plane<Real>::SignedDistance(Vector4<Real> const & a_point) const
  {
    return point.Dot(m_normal) + m_offset;

  }	//End: Plane::SignedDistance()
  

  //--------------------------------------------------------------------------------
  //		@ Plane::Distance()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real Plane<Real>::Distance(Vector4<Real> const & a_point) const
  {
    return abs(point.Dot(m_normal) + m_offset);

  }	//End: Plane::Distance()

  
  //--------------------------------------------------------------------------------
  //		@ Plane::NormalDot()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real Plane<Real>::NormalDot(Vector4<Real> const & a_v) const
  {
    return m_normal.Dot(a_v);

  }	//End: Plane::NormalDot()


  //--------------------------------------------------------------------------------
  //		@ Plane::Plane()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real Plane<Real>::Plane<Real>(Real a, Real b, Real c, Real d)
  {
    Set(a, b, c, d);

  }	//End: Plane::Plane()


  //--------------------------------------------------------------------------------
  //		@ Plane::Plane()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real Plane<Real>::Plane<Real>(Vector4<Real> const & n, Real d)
  {
    Set(n.x(), n.y(), n.z(), d);

  }	//End: Plane::Plane()


  //--------------------------------------------------------------------------------
  //		@ Plane::Plane()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real Plane<Real>::Plane<Real>(Vector4<Real> const & p0, 
                                Vector4<Real> const & p1, 
                                Vector4<Real> const & p2)
  {
    Set(p0, p1, p2);

  }	//End: Plane::Plane()


  //--------------------------------------------------------------------------------
  //		@ Plane::Plane()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real Plane<Real>::Plane<Real>(Plane<Real> const & a_other):
    m_normal(a_other.m_normal), m_offset(a_other.m_offset)
  {
  }	//End: Plane::Plane()


  //--------------------------------------------------------------------------------
  //		@ Plane::operator=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Plane<Real>& Plane<Real>::operator=(Plane<Real> const & a_other)
  {
    m_normal = a_other.m_normal;
    m_offset = a_other.m_offset;

    return *this;
  }	//End: Plane::operator=()


  //--------------------------------------------------------------------------------
  //		@ Plane::Get()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Plane<Real>::Get(Vector4<Real> & normal, Real & offset )
  {
    normal = m_normal;
    offset = m_offset;
  }	//End: Plane::Get()


  //--------------------------------------------------------------------------------
  //		@ Plane::operator==()
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool Plane<Real>::operator==(Plane<Real> const & a_other)
  {
    return (m_normal == a_other.m_normal && m_offset == a_other.m_offset);
  }	//End: Plane::operator==()


  //--------------------------------------------------------------------------------
  //		@ Plane::operator!=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool Plane<Real>::operator!=(Plane<Real> const & a_other)
  {
    return (m_normal != a_other.m_normal || m_offset != a_other.m_offset);
  }	//End: Plane::operator!=()


  //--------------------------------------------------------------------------------
  //	@	Plane::Set()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Plane<Real>::Set(Real a, Real b, Real c, Real d)
  {
	  //normalise for cheap distance checks
	  Real lensq = a*a + b*b + c*c;

	  //recover gracefully
	  if (Dg::IsZero(lensq))
	  {
      m_normal = Vector4<Real>::xAxis();
		  m_offset = static_cast<Real>(0.0);
	  }
	  else
	  {
		  Real recip = static_cast<Real>(1.0) / sqrt(lensq);
		  m_normal.Set(a*recip, b*recip, c*recip, static_cast<Real>(0.0));
		  m_offset = d*recip;
	  }

  }	//End: Plane4::Set()


  //--------------------------------------------------------------------------------
  //	@	Plane::Set()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Plane<Real>::Set(Vector4<Real> const & n, Real d)
  {
	  //normalise for cheap distance checks
	  Real lensq = n.x()*n.x() + n.y()*n.y() + n.z()*n.z();

	  //recover gracefully
	  if (Dg::IsZero(lensq))
	  {
      m_normal = Vector4<Real>::xAxis();
		  m_offset = static_cast<Real>(0.0);
	  }
	  else
	  {
		  Real recip = static_cast<Real>(1.0) / sqrt(lensq);
		  m_normal.Set(n.x()*recip, n.y()*recip, n.z()*recip, static_cast<Real>(0.0));
		  m_offset = d*recip;
	  }

  }	//End: Plane4::Set()


  //--------------------------------------------------------------------------------
  //	@	Plane::Set()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Plane<Real>::Set(Vector4<Real> const & n, Vector4<Real> const & p)
  {
	  //normalise for cheap distance checks
	  Real lensq = n.x()*n.x() + n.y()*n.y() + n.z()*n.z();

	  //recover gracefully
	  if (Dg::IsZero(lensq))
	  {
      m_normal = Vector4<Real>::xAxis();
		  m_offset = static_cast<Real>(0.0);
	  }
	  else
	  {
		  Real recip = static_cast<Real>(1.0) / sqrt(lensq);
		  m_normal.Set(n.x()*recip, n.y()*recip, n.z()*recip, static_cast<Real>(0.0));
		  m_offset = -(n.x()*p.x() + n.y()*p.y() + n.z()*p.z())*recip;
	  }

  }	//End: Plane4::Set()


  //--------------------------------------------------------------------------------
  //	@	Plane::Set()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Plane<Real>::Set(Vector4<Real> const & p0, 
                        Vector4<Real> const & p1,
                        Vector4<Real> const & p2)
  {
    //Get plane vector
    Vector4<Real> u = p1 - p0;
    Vector4<Real> v = p2 - p0;
    Vector4<Real> w = Cross(u, v);

	  //normalise for cheap distance checks
	  Real lensq = w.x()*w.x() + w.y()*w.y() + w.z()*w.z();

	  //recover gracefully
	  if (Dg::IsZero(lensq))
	  {
      m_normal = Vector4<Real>::xAxis();
		  m_offset = static_cast<Real>(0.0);
	  }
	  else
	  {
		  Real recip = static_cast<Real>(1.0) / sqrt(lensq);
		  m_normal.Set(w.x()*recip, w.y()*recip, w.z()*recip, static_cast<Real>(0.0));
      m_offset = -p0.Dot(m_normal);
	  }

  }	//End: Plane4::Set()


  //--------------------------------------------------------------------------------
  //		@ Plane::ClosestPoint()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Plane<Real>::ClosestPoint(Vector4<Real> const & p)
  {
    return p - SignedDistance(p) * m_normal;
  }	//End: Plane::ClosestPoint()
}

#endif