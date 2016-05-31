//! @file DgLine.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Line

#ifndef DGLINE_H
#define DGLINE_H

#include "DgVector4.h"
#include "DgPlane.h"
#include "dgmath.h"

namespace Dg
{
  //! @ingroup Math_classes
  //!
  //! @class Line
  //!
  //! A Line is defined by a point and direction extending infinitely in both
  //! directions. It is represented by a point and a normalized vector. This file
  //! also declares methods to test Lines against other geometric entities.
  //!
  //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
  //!
  //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
  //! @date 29/05/2016
  template<typename Real>
  class Line
  {
  public:
    //! Default constructor
    Line();

    //! Construct from an origin and direction
    Line(Vector4<Real> const & origin, Vector4<Real> const & direction);
    ~Line() {}

    Line(Line const &);
    Line& operator=(Line const &);

    Vector4<Real> const & Origin() const { return m_origin; }
    Vector4<Real> const & Direction() const { return m_direction; }
    void Get(Vector4<Real>& origin, Vector4<Real>& direction) const;

    bool operator== (Line const &) const;
    bool operator!= (Line const &) const;

    //! Set line from an origin and direction
    void Set(Vector4<Real> const & origin, Vector4<Real> const & direction);

    //! Closest point on a line to a point.
    Vector4<Real> ClosestPoint(Vector4<Real> const & a_pIn) const;

  private:

    //Data members
    Vector4<Real> m_origin;
    Vector4<Real> m_direction;
  };


  //--------------------------------------------------------------------------------
  //	@	Line::Line()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Line<Real>::Line() : m_origin(Vector4<Real>::Origin())
                     , m_direction(Vector4<Real>::xAxis())
  {
  }	//End: Line::Line()


  //--------------------------------------------------------------------------------
  //	@	Line::Line()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Line<Real>::Line(Vector4<Real> const & a_origin, Vector4<Real> const & a_direction)
  {
    Set(a_origin, a_direction);
  }	//End: Line::Line()


  //--------------------------------------------------------------------------------
  //	@	Line::Line()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Line<Real>::Line(Line<Real> const & a_other): m_origin(a_other.m_origin)
                                              , m_direction(a_other.m_direction)
  {
  }	//End: Line::Line()


  //--------------------------------------------------------------------------------
  //	@	Line::operator=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Line<Real> & Line<Real>::operator=(Line<Real> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_direction = a_other.m_direction;

    return *this;
  }	//End: Line::operator=()


  //--------------------------------------------------------------------------------
  //	@	Line::Get()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Line<Real>::Get(Vector4<Real> & a_origin, Vector4<Real> & a_direction) const
  {
    a_origin = m_origin;
    a_direction = m_direction;
  }	//End: Line::Get()


  //--------------------------------------------------------------------------------
  //	@	Line::operator==()
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool Line<Real>::operator==(Line<Real> const & a_other) const
  {
    return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
  }	//End: Line::operator==()


  //--------------------------------------------------------------------------------
  //	@	Line::operator!=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool Line<Real>::operator!=(Line<Real> const & a_other) const
  {
    return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
  }	//End: Line::operator!=()


  //--------------------------------------------------------------------------------
  //	@	Line::Set()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Line<Real>::Set(Vector4<Real> const & a_origin, Vector4<Real> const & a_direction)
  {
    //Assign
    m_origin = a_origin;
    m_origin.w() = static_cast<Real>(1.0);

    m_direction = a_direction;
    m_direction.w() = static_cast<Real>(0.0);

    m_direction.Normalize();

  }	//End: Line::Set()


  //--------------------------------------------------------------------------------
  //	@	Line::ClosestPoint()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Line<Real>::ClosestPoint(Vector4<Real> const & a_pIn) const
  {
    Vector4<Real> w = a_pIn - m_origin;

    Real vsq = m_direction.Dot(m_direction);
    Real proj = w.Dot(m_direction);

    return m_origin + (proj / vsq) * m_direction;
  }	//End: Line::ClosestPoint()

  //! @ingroup Math_gTests
  //!
  //! Closest points between two lines.
  //!
  //! @param[in] a_line0 Input line
  //! @param[in] a_line1 Input line
  //! @param[out] a_p0 Point on a_line0 closest to a_line1 
  //! @param[out] a_p1 Point on a_line1 closest to a_line0
  //!
  //! @return 0: Success
  //! @return 1: Lines are parallel. Closest points are based off a_line0 origin.
  template<typename Real>
  int ClosestPointsLineLine(Line<Real> const & a_line0, Line<Real> const & a_line1,
                            Vector4<Real> & a_p0, Vector4<Real> & a_p1)
  {
    Vector4<Real> o0(a_line0.Origin());
    Vector4<Real> o1(a_line1.Origin());
    Vector4<Real> d0(a_line0.Direction());
    Vector4<Real> d1(a_line1.Direction());

    //compute intermediate parameters
    Vector4<Real> w0(o0 - o1);
    Real a = d0.Dot(d0);
    Real b = d0.Dot(d1);
    Real c = d1.Dot(d1);
    Real d = d0.Dot(w0);
    Real e = d1.Dot(w0);
    Real denom = a*c - b*b;
    if (Dg::IsZero(denom))
    {
      a_p0 = o0;
      a_p1 = o1 + (e / c) * d1;
      return 1;
    }
    else
    {
      a_p0 = o0 + ((b*e - c*d) / denom)*d0;
      a_p1 = o1 + ((a*e - b*d) / denom)*d1;
      return 0;
    }
  }	//End: Line::ClosestPointLineLine()

  //! @ingroup Math_gTests
  //!
  //! Intersection test between a plane and a line
  //!
  //! @param[in] a_plane Input plane
  //! @param[in] a_line Input line
  //! @param[out] a_point Point of intersection.
  //!
  //! @return 0: Success
  //! @return 1: No Intersection. Line is orthogonal to the plane normal. Output point not set.
  //! @return 2: Line lies on the plane. Output point not set.
  template<typename Real>
  int TestPlaneLine(Plane<Real> const & a_plane, Line<Real> const & a_line,
                    Vector4<Real> & a_point)
  {
    Vector4<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    Vector4<Real> lo(a_line.Origin());
    Vector4<Real> ld(a_line.Direction());

    Real denom = pn.Dot(ld);

    //check if line is parallel to plane
    if (Dg::IsZero(denom))
    {
      //check if line is on the plane
      if (Dg::IsZero(a_plane.Distance(lo)))
      {
        return 2;
      }
      return 1;
    }

    a_point = lo + ((lo.Dot(pn) + po) / denom) * ld;
    return 0;
  }	//End: Line::IntersectionPlaneLine()
}

#endif