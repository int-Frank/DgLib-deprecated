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
}

#endif