//! @file DgSegment2D.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Segment2D

#ifndef DGSEGMENT2D_H
#define DGSEGMENT2D_H

#include "DgVector3.h"
#include "dgmath.h"

namespace Dg
{
  //! @ingroup DgMath_types
  //!
  //! @class Segment2D
  //!
  //! A Linesgment is a line that connects two points. It is represented by a Point4 
  //! and a Vector3, the length of the vector being the distance between the points.
  //! This file also declares methods to test LineSegments against other 
  //! geometric entities.
  //!
  //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
  //!
  //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
  //! @date 29/05/2016
  template<typename Real>
  class Segment2D
  {
  public:
    //! Default constructor
    Segment2D();

    //! Construct from an origin and direction
    Segment2D(Vector3<Real> const & p0, Vector3<Real> const & p1);
    ~Segment2D() {}

    //! Copy constructor
    Segment2D(Segment2D const &);

    //! Assignment
    Segment2D& operator=(Segment2D const &);

    //! Get the origin of the line
    Vector3<Real> const & Origin() const { return m_origin; }

    //! Get the direction of the line
    Vector3<Real> const & Direction() const { return m_direction; }

    //! Get endpoint 0
    Vector3<Real> GetP0() const { return m_origin; }

    //! Get endpoint 1
    Vector3<Real> GetP1() const { return m_origin + m_direction; }

    //! Get the center of the line segment
    Vector3<Real> GetCenter() const { return m_origin + static_cast<Real>(0.5) * m_direction; }

    //! Get the endpoints of the line segment
    void Get(Vector3<Real>& a_p0, Vector3<Real>& a_p1) const;

    //! Comparison
    bool operator== (Segment2D const &) const;

    //! Comparison
    bool operator!= (Segment2D const &) const;

    //! Set line segment from endpoints
    void Set(Vector3<Real> const & p0, Vector3<Real> const & p1);

    //! Get the length of the line segment
    Real Length() const;

    //! Get the squared length of the line segment
    Real LengthSquared() const;

  private:

    //Data members
    Vector3<Real> m_origin;
    Vector3<Real> m_direction;
  };


  //--------------------------------------------------------------------------------
  //	@	Segment2D::Segment2D()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Segment2D<Real>::Segment2D() : m_origin(Vector3<Real>::Origin())
    , m_direction(Vector3<Real>::xAxis())
  {
  }	//End: Segment2D::Segment2D()


    //--------------------------------------------------------------------------------
    //  @ Segment2D::Segment2D()
    //--------------------------------------------------------------------------------
  template<typename Real>
  Segment2D<Real>::Segment2D(Vector3<Real> const & p0, Vector3<Real> const & p1)
  {
    Set(p0, p1);
  }	//End: Segment2D::Segment2D()


    //--------------------------------------------------------------------------------
    //  @ Segment2D::Segment2D()
    //--------------------------------------------------------------------------------
  template<typename Real>
  Segment2D<Real>::Segment2D(Segment2D<Real> const & a_other) :
    m_origin(a_other.m_origin), m_direction(a_other.m_direction)
  {
  }	//End: Segment2D::Segment2D()


    //--------------------------------------------------------------------------------
    //  @ Segment2D::operator=()
    //--------------------------------------------------------------------------------
  template<typename Real>
  Segment2D<Real> & Segment2D<Real>::operator=(Segment2D<Real> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_direction = a_other.m_direction;

    return *this;
  }	//End: Segment2D::operator=()


    //--------------------------------------------------------------------------------
    //  @ Segment2D::Get()
    //--------------------------------------------------------------------------------
  template<typename Real>
  void Segment2D<Real>::Get(Vector3<Real> & a_p0, Vector3<Real> & a_p1) const
  {
    a_p0 = m_origin;
    a_p1 = m_origin + m_direction;
  }	//End: Segment2D::Get()


    //--------------------------------------------------------------------------------
    //  @ Segment2D::operator==()
    //--------------------------------------------------------------------------------
  template<typename Real>
  bool Segment2D<Real>::operator==(Segment2D<Real> const & a_other) const
  {
    return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
  }	//End: Segment2D::operator==()


    //--------------------------------------------------------------------------------
    //  @ Segment2D::operator!=()
    //--------------------------------------------------------------------------------
  template<typename Real>
  bool Segment2D<Real>::operator!=(Segment2D<Real> const & a_other) const
  {
    return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
  }	//End: Segment2D::operator!=()


    //--------------------------------------------------------------------------------
    //  @ Segment2D::Set()
    //--------------------------------------------------------------------------------
  template<typename Real>
  void Segment2D<Real>::Set(Vector3<Real> const & a_p0, Vector3<Real> const & a_p1)
  {
    //Assign
    m_origin = a_p0;
    m_origin.w() = static_cast<Real>(1.0);

    m_direction = a_p1 - a_p0;
    m_direction.w() = static_cast<Real>(0.0);

  }	//End: Segment2D::Set()


    //--------------------------------------------------------------------------------
    //  @ Segment2D::Length()
    //--------------------------------------------------------------------------------
  template<typename Real>
  Real Segment2D<Real>::Length() const
  {
    return m_direction.Length();

  }	//End: Segment2D::Length()


    //--------------------------------------------------------------------------------
    //  @ Segment2D::LengthSquared()
    //--------------------------------------------------------------------------------
  template<typename Real>
  Real Segment2D<Real>::LengthSquared() const
  {
    return m_direction.LengthSquared();

  }	//End: Segment2D::LengthSquared()

}
#endif