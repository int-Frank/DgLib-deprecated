//! @file DgLineSegment.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: LineSegment

#ifndef DGLINESEGMENT_H
#define DGLINESEGMENT_H

#include "DgVector4.h"
#include "DgPlane.h"
#include "DgLine.h"
#include "DgRay.h"
#include "DgMatrix44.h"
#include "DgVQS.h"
#include "dgmath.h"

namespace Dg
{
  //! @ingroup DgMath_types
  //!
  //! @class LineSegment
  //!
  //! A Linesgment is a line that connects two points. It is represented by a Point4 
  //! and a Vector4, the length of the vector being the distance between the points.
  //! This file also declares methods to test LineSegments against other 
  //! geometric entities.
  //!
  //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
  //!
  //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
  //! @date 29/05/2016
  template<typename Real>
  class LineSegment
  {
  public:
    //! Default constructor
    LineSegment();

    //! Construct from an origin and direction
    LineSegment(Vector4<Real> const & p0, Vector4<Real> const & p1);
    ~LineSegment() {}

    //! Copy constructor
    LineSegment(LineSegment const &);

    //! Assignment
    LineSegment& operator=(LineSegment const &);

    //! Get the origin of the line
    Vector4<Real> const & Origin() const { return m_origin; }

    //! Get the direction of the line
    Vector4<Real> const & Direction() const { return m_direction; }

    //! Get endpoint 0
    Vector4<Real> GetP0() const { return m_origin; }

    //! Get endpoint 1
    Vector4<Real> GetP1() const { return m_origin + m_direction; }

    //! Get the center of the line segment
    Vector4<Real> GetCenter() const { return m_origin + static_cast<Real>(0.5) * m_direction; }

    //! Get the endpoints of the line segment
    void Get(Vector4<Real>& a_p0, Vector4<Real>& a_p1) const;

    //! Comparison
    bool operator== (LineSegment const &) const;

    //! Comparison
    bool operator!= (LineSegment const &) const;

    //! Set line segment from endpoints
    void Set(Vector4<Real> const & p0, Vector4<Real> const & p1);

    //! Get the length of the line segment
    Real Length() const;

    //! Get the squared length of the line segment
    Real LengthSquared() const;

    //! Transform the line segment
    LineSegment operator*(Matrix44<Real> const &) const;

    //! Transform the line segment, assign to self
    LineSegment & operator*=(Matrix44<Real> const &);

    //! Transform the line segment
    LineSegment operator*(VQS<Real> const &) const;

    //! Transform the line segment, assign to self
    LineSegment & operator*=(VQS<Real> const &);
  private:

    //Data members
    Vector4<Real> m_origin;
    Vector4<Real> m_direction;
  };


  //--------------------------------------------------------------------------------
  //	@	LineSegment::LineSegment()
  //--------------------------------------------------------------------------------
  template<typename Real>
  LineSegment<Real>::LineSegment() : m_origin(Vector4<Real>::Origin())
    , m_direction(Vector4<Real>::xAxis())
  {
  }	//End: LineSegment::LineSegment()


  //--------------------------------------------------------------------------------
  //  @ LineSegment::LineSegment()
  //--------------------------------------------------------------------------------
  template<typename Real>
  LineSegment<Real>::LineSegment(Vector4<Real> const & p0, Vector4<Real> const & p1)
  {
    Set(p0, p1);
  }	//End: LineSegment::LineSegment()


  //--------------------------------------------------------------------------------
  //  @ LineSegment::LineSegment()
  //--------------------------------------------------------------------------------
  template<typename Real>
  LineSegment<Real>::LineSegment(LineSegment<Real> const & a_other) :
    m_origin(a_other.m_origin), m_direction(a_other.m_direction)
  {
  }	//End: LineSegment::LineSegment()


  //--------------------------------------------------------------------------------
  //  @ LineSegment::operator=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  LineSegment<Real> & LineSegment<Real>::operator=(LineSegment<Real> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_direction = a_other.m_direction;

    return *this;
  }	//End: LineSegment::operator=()


  //--------------------------------------------------------------------------------
  //  @ LineSegment::Get()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void LineSegment<Real>::Get(Vector4<Real> & a_p0, Vector4<Real> & a_p1) const
  {
    a_p0 = m_origin;
    a_p1 = m_origin + m_direction;
  }	//End: LineSegment::Get()


  //--------------------------------------------------------------------------------
  //  @ LineSegment::operator==()
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool LineSegment<Real>::operator==(LineSegment<Real> const & a_other) const
  {
    return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
  }	//End: LineSegment::operator==()


  //--------------------------------------------------------------------------------
  //  @ LineSegment::operator!=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool LineSegment<Real>::operator!=(LineSegment<Real> const & a_other) const
  {
    return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
  }	//End: LineSegment::operator!=()


  //--------------------------------------------------------------------------------
  //  @ LineSegment::Set()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void LineSegment<Real>::Set(Vector4<Real> const & a_p0, Vector4<Real> const & a_p1)
  {
    //Assign
    m_origin = a_p0;
    m_origin.w() = static_cast<Real>(1.0);

    m_direction = a_p1 - a_p0;
    m_direction.w() = static_cast<Real>(0.0);

  }	//End: LineSegment::Set()


    //--------------------------------------------------------------------------------
    //  @ LineSegment::Length()
    //--------------------------------------------------------------------------------
  template<typename Real>
  Real LineSegment<Real>::Length() const
  {
    return m_direction.Length();

  }	//End: LineSegment::Length()


  //--------------------------------------------------------------------------------
  //  @ LineSegment::LengthSquared()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real LineSegment<Real>::LengthSquared() const
  {
    return m_direction.LengthSquared();

  }	//End: LineSegment::LengthSquared()


  //--------------------------------------------------------------------------------
  //	@	LineSegment::operator*()
  //--------------------------------------------------------------------------------
  template<typename Real>
  LineSegment<Real> LineSegment<Real>::operator*(Matrix44<Real> const & a_mat) const
  {
    Vector4<Real> p0(m_origin * a_mat);
    Vector4<Real> p1(p0 + m_direction * a_mat);
    return LineSegment<Real>(p0, p1);
  }	//End: LineSegment::operator*()


  //--------------------------------------------------------------------------------
  //	@	LineSegment::operator*=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  LineSegment<Real>& LineSegment<Real>::operator*=(Matrix44<Real> const & a_mat)
  {
    Vector4<Real> p0(m_origin * a_mat);
    Vector4<Real> p1(p0 + m_direction * a_mat);
    Set(p0, p1);
    return *this;
  }	//End: LineSegment::operator*=()


  //--------------------------------------------------------------------------------
  //	@	LineSegment::operator*()
  //--------------------------------------------------------------------------------
  template<typename Real>
  LineSegment<Real> LineSegment<Real>::operator*(VQS<Real> const & a_vqs) const
  {
    Vector4<Real> p0(a_vqs.TransformPoint(m_origin));
    Vector4<Real> p1(p0 + a_vqs.TransformVector(m_direction));
    return LineSegment<Real>(p0, p1);
  }	//End: LineSegment::operator*()


  //--------------------------------------------------------------------------------
  //	@	LineSegment::operator*=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  LineSegment<Real>& LineSegment<Real>::operator*=(VQS<Real> const & a_vqs)
  {
    Vector4<Real> p0(a_vqs.TransformPoint(m_origin));
    Vector4<Real> p1(p0 + a_vqs.TransformVector(m_direction));
    Set(p0, p1);
    return *this;
  }	//End: LineSegment::operator*=()
}
#endif