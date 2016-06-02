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

    //! Get the center of the linesegment
    Vector4<Real> GetCenter() const { return m_origin + static_cast<Real>(0.5) * m_direction; }

    //! Get the endpoints of the linesegment
    void Get(Vector4<Real>& a_p0, Vector4<Real>& a_p1) const;

    //! Comparison
    bool operator== (LineSegment const &) const;

    //! Comparison
    bool operator!= (LineSegment const &) const;

    //! Set linesegment from endpoints
    void Set(Vector4<Real> const & p0, Vector4<Real> const & p1);

    //! Get the length of the linesegment
    Real Length() const;

    //! Get the squared length of the linesegment
    Real LengthSquared() const;

    //! Closest point on a linesegment to a point.
    Vector4<Real> ClosestPoint(Vector4<Real> const & a_pIn) const;

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
  //  @ LineSegment::ClosestPoint()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> LineSegment<Real>::ClosestPoint(Vector4<Real> const & a_pIn) const
  {
    Vector4<Real> w = a_pIn - m_origin;

    Real proj = w.Dot(m_direction);

    if (proj <= static_cast<Real>(0.0))
    {
      return m_origin;
    }
    
    Real vsq = m_direction.Dot(m_direction);

    if (proj >= vsq)
    {
      return m_origin + m_direction;
    }

    return m_origin + (proj / vsq) * m_direction;
  }	//End: LineSegment::ClosestPoint()


  //! @ingroup Math_gTests
  //!
  //! Closest points between two linesegments.
  //!
  //! @param[in] a_ls0 Input linesegment
  //! @param[in] a_ls1 Input linesegment
  //! @param[out] a_p0 Point on a_ls0 closest to a_ls1 
  //! @param[out] a_p1 Point on a_ls1 closest to a_ls0
  //!
  //! @return 0: Success
  //! @return 1: Lines are parallel.
  template<typename Real>
  int ClosestPointsLineLine(LineSegment<Real> const & a_line0, LineSegment<Real> const & a_line1,
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
  }	//End: LineSegment::ClosestPointLineLine()

    //! @ingroup Math_gTests
    //!
    //! Intersection test between a plane and a line
    //!
    //! @param[in] a_plane Input plane
    //! @param[in] a_line Input line
    //! @param[out] a_point Point of intersection.
    //!
    //! @return 0: LineSegment intersects plane
    //! @return 1: LineSegment lies on the plane. Output set to line origin.
    //! @return 2: No Intersection. LineSegment is orthogonal to the plane normal. Output point not set.
  template<typename Real>
  int TestPlaneLine(Plane<Real> const & a_plane, LineSegment<Real> const & a_line,
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
      a_point = lo;

      //check if line is on the plane
      if (Dg::IsZero(a_plane.Distance(lo)))
      {
        return 1;
      }
      return 2;
    }

    a_point = lo + ((lo.Dot(pn) + po) / denom) * ld;
    return 0;
  }	//End: LineSegment::IntersectionPlaneLine()
}

#endif