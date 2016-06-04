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

    //! Closest point on a line segment to a point.
    //!
    //! @return 0: Success
    //!
    //! @param[in] a_pIn Input point
    //! @param[out] a_pOut Closest point on the line segment
    //! @param[out] a_u Distance along the ray to the closest point: 0 < u < 1
    int ClosestPoint(Vector4<Real> const & a_pIn,
                     Vector4<Real> & a_pOut,
                     Real & a_u) const;

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
  int LineSegment<Real>::ClosestPoint(Vector4<Real> const & a_pIn,
                                      Vector4<Real> & a_pOut,
                                      Real & a_u) const
  {
    Vector4<Real> w = a_pIn - m_origin;

    Real proj = w.Dot(m_direction);

    if (proj <= static_cast<Real>(0.0))
    {
      a_u = static_cast<Real>(0.0);
      a_pOut = m_origin;
    }
    else
    {
      Real vsq = m_direction.Dot(m_direction);

      if (proj >= vsq)
      {
        a_u = static_cast<Real>(1.0);
        a_pOut = m_origin + m_direction;
      }
      else
      {
        a_u = (proj / vsq);
        a_pOut = m_origin + a_u * m_direction;
      }
    }
    
    return 0;
  }	//End: LineSegment::ClosestPoint()


  //! @ingroup Math_gTests
  //!
  //! Closest points between two line segments.
  //!
  //! @param[in] a_ls0 Input line segment
  //! @param[in] a_ls1 Input line segment
  //! @param[out] a_u0 Distance along a_ls0 to closest point
  //! @param[out] a_u1 Distance along a_ls0 to closest point
  //! @param[out] a_p0 Point on a_ls0 closest to a_ls1 
  //! @param[out] a_p1 Point on a_ls1 closest to a_ls0
  //!
  //! @return 0: Success
  //! @return 1: Line segments are parallel, potentially infinite closest points.
  template<typename Real>
  int ClosestPointsLineSegmentLineSegment(
    LineSegment<Real> const & a_ls0, LineSegment<Real> const & a_ls1,
    Real & a_u0, Real & a_u1,
    Vector4<Real> & a_p0, Vector4<Real> & a_p1)
  {
    Vector4<Real> o0(a_ls0.Origin());
    Vector4<Real> o1(a_ls1.Origin());
    Vector4<Real> d0(a_ls0.Direction());
    Vector4<Real> d1(a_ls1.Direction());

    //compute intermediate parameters
    Vector4<Real> w0(o0 - o1);
    Real a = d0.Dot(d0);
    Real b = d0.Dot(d1);
    Real c = d1.Dot(d1);
    Real d = d0.Dot(w0);
    Real e = d1.Dot(w0);
    Real denom = a*c - b*b;

    Real sn, sd, tn, td;
    int result = 0;

    // if denom is zero, try finding closest point on segment1 to origin0
    if (Dg::IsZero(denom))
    {
      // clamp a_u0 to 0
      sd = td = c;
      sn = static_cast<Real>(0.0);
      tn = e;
      result = 1;
    }
    else
    {
      // clamp a_u0 within [0,1]
      sd = td = denom;
      sn = b*e - c*d;
      tn = a*e - b*d;

      // clamp a_u0 to 0
      if (sn < static_cast<Real>(0.0))
      {
        sn = static_cast<Real>(0.0);
        tn = e;
        td = c;
      }
      // clamp a_u0 to 1
      else if (sn > sd)
      {
        sn = sd;
        tn = e + b;
        td = c;
      }
    }

    // clamp a_u1 within [0,1]
    // clamp a_u1 to 0
    if (tn < static_cast<Real>(0.0))
    {
      a_u1 = static_cast<Real>(0.0);
      // clamp a_u0 to 0
      if (-d < static_cast<Real>(0.0))
      {
        a_u0 = static_cast<Real>(0.0);
      }
      // clamp a_u0 to 1
      else if (-d > a)
      {
        a_u0 = static_cast<Real>(1.0);
      }
      else
      {
        a_u0 = -d / a;
      }
    }
    // clamp a_u1 to 1
    else if (tn > td)
    {
      a_u1 = static_cast<Real>(1.0);
      // clamp a_u0 to 0
      if ((-d + b) < static_cast<Real>(0.0))
      {
        a_u0 = static_cast<Real>(0.0);
      }
      // clamp a_u0 to 1
      else if ((-d + b) > a)
      {
        a_u0 = static_cast<Real>(1.0);
      }
      else
      {
        a_u0 = (-d + b) / a;
      }
    }
    else
    {
      a_u1 = tn / td;
      a_u0 = sn / sd;
    }

    // compute closest points
    a_p0 = o0 + a_u0*d0;
    a_p1 = o1 + a_u1*d1;

    return result;
  }	//End: LineSegment::ClosestPointLineSegmentLineSegment()


  //! @ingroup Math_gTests
  //!
  //! Closest points between a line segment and a ray.
  //!
  //! @param[in] a_ls Input line segment
  //! @param[in] a_ray Input ray
  //! @param[out] a_uls Distance along the line segment to closest point
  //! @param[out] a_ur Distance along the ray to closest point
  //! @param[out] a_pls Point on the line segment closest to the ray 
  //! @param[out] a_pr Point on the ray closest to the line segment
  //!
  //! @return 0: Success
  //! @return 1: Line segment is parallel to ray, potentially infinite closest points.
  template<typename Real>
  int ClosestPointsLineSegmentRay(
    LineSegment<Real> const & a_ls, Ray<Real> const & a_ray,
    Real & a_uls, Real & a_ur,
    Vector4<Real> & a_pls, Vector4<Real> & a_pr)
  {
    Vector4<Real> ols(a_ls.Origin());
    Vector4<Real> or(a_ray.Origin());
    Vector4<Real> dls(a_ls.Direction());
    Vector4<Real> dr(a_ray.Direction());

    //compute intermediate parameters
    Vector4<Real> w0(ols - or);
    Real a = dls.Dot(dls);
    Real b = dls.Dot(dr);
    Real c = dls.Dot(w0);
    Real d = dr.Dot(w0);
    Real denom = a - b*b;

    Real sn, sd, tn, td;
    int result = 0;

    // if denom is zero, try finding closest point on segment1 to origin0
    if (Dg::IsZero(denom))
    {
      // clamp a_uls to 0
      sd = td = static_cast<Real>(1.0);
      sn = static_cast<Real>(0.0);
      tn = d;
      result = 1;
    }
    else
    {
      // clamp a_uls within [0,1]
      sd = td = denom;
      sn = b*d - c;
      tn = a*d - b*c;

      // clamp a_uls to 0
      if (sn < static_cast<Real>(0.0))
      {
        sn = static_cast<Real>(0.0);
        tn = d;
        td = static_cast<Real>(1.0);
      }
      // clamp a_uls to 1
      else if (sn > sd)
      {
        sn = sd;
        tn = d + b;
        td = static_cast<Real>(1.0);
      }
    }

    // clamp a_ur within [0,+inf]
    // clamp a_ur to 0
    if (tn < static_cast<Real>(0.0))
    {
      a_ur = static_cast<Real>(0.0);
      // clamp a_uls to 0
      if (-c < static_cast<Real>(0.0))
      {
        a_uls = static_cast<Real>(0.0);
      }
      // clamp a_uls to 1
      else if (-c > a)
      {
        a_uls = static_cast<Real>(1.0);
      }
      else
      {
        a_uls = -c / a;
      }
    }
    else
    {
      a_ur = tn / td;
      a_uls = sn / sd;
    }

    // compute closest points
    a_pls = ols + a_uls*dls;
    a_pr = or + a_ur*dr;

    return result;
  }	//End: LineSegment::ClosestPointLineSegmentRay()


  //! @ingroup Math_gTests
  //!
  //! Closest points between a line segment and a line.
  //!
  //! @param[in] a_ls Input line segment
  //! @param[in] a_line Input line
  //! @param[out] a_uls Distance along the line segment to closest point
  //! @param[out] a_ul Distance along the line to closest point
  //! @param[out] a_pls Point on the line segment closest to the line 
  //! @param[out] a_pl Point on the line closest to the line segment
  //!
  //! @return 0: Success
  //! @return 1: Line segment is parallel to line.
  template<typename Real>
  int ClosestPointsLineSegmentLine(
    LineSegment<Real> const & a_ls, Line<Real> const & a_line,
    Real & a_uls, Real & a_ul,
    Vector4<Real> & a_pls, Vector4<Real> & a_pl)
  {
    Vector4<Real> ols(a_ls.Origin());
    Vector4<Real> ol (a_line.Origin());
    Vector4<Real> dls(a_ls.Direction());
    Vector4<Real> dl(a_line.Direction());

    //compute intermediate parameters
    Vector4<Real> w0(ols - ol );
    Real a = dls.Dot(dls);
    Real b = dls.Dot(dl);
    Real c = dls.Dot(w0);
    Real d = dl.Dot(w0);
    Real denom = a - b*b;

    // if denom is zero, try finding closest point on line to segment origin
    if (Dg::IsZero(denom))
    {
      // compute closest points
      a_pls = ols;
      a_pl = ol + d*dl;
      a_uls = static_cast<Real>(0.0);
      a_ul = d;
      return 1;
    }

    // clamp a_uls within [0,1]
    Real sn = b*d - c;

    // clamp a_uls to 0
    if (sn < static_cast<Real>(0.0))
    {
      a_uls = static_cast<Real>(0.0);
      a_ul = d;
    }
    // clamp a_uls to 1
    else if (sn > denom)
    {
      a_uls = static_cast<Real>(1.0);
      a_ul = (d + b);
    }
    else
    {
      a_uls = sn / denom;
      a_ul = (a*d - b*c) / denom;
    }

    // compute closest points
    a_pl = ol + a_ul*dl;
    a_pls = ols + a_uls*dls;
    return 0;
    
  }	//End: LineSegment::ClosestPointLineSegmentLine()


  //! @ingroup Math_gTests
  //!
  //! Intersection test between a plane and a line segment
  //!
  //! @param[in] a_plane Input plane
  //! @param[in] a_ls Input line segment
  //! @param[out] a_point Point of intersection.
  //!
  //! @return 0: Line segment intersects plane
  //! @return 1: Line segment lies on the plane. Output set to line segment p0.
  //! @return 2: No Intersection.
  template<typename Real>
  int TestPlaneLineSegment(Plane<Real> const & a_plane, 
                           LineSegment<Real> const & a_ls,
                           Real & a_u,
                           Vector4<Real> & a_point)
  {
    Vector4<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    Vector4<Real> lso(a_ls.Origin());
    Vector4<Real> lsd(a_ls.Direction());
    //Real lsLength = a_ls.Length();

    //Line segment of length 0
    if (Dg::IsZero(a_ls.LengthSquared()))
    {
      a_point = lso;
      a_u = static_cast<Real>(0.0);
      if (Dg::IsZero(a_plane.SignedDistance(lso)))
      {
        return 1;
      }
      return 2;
    }

    Real denom = pn.Dot(lsd);

    //check if line is parallel to plane
    if (Dg::IsZero(denom))
    {
      a_u = static_cast<Real>(0.0);
      a_point = lso;

      //check if line is on the plane
      if (Dg::IsZero(a_plane.Distance(lso)))
      {
        return 1;
      }
      return 2;
    }

    a_u = (-(lso.Dot(pn) + po) / denom);
    if (a_u < static_cast<Real>(0.0))
    {
      a_u = static_cast<Real>(0.0);
      a_point = lso;
      return 2;
    }
      
    if (a_u > static_cast<Real>(1.0))
    {
      a_u = static_cast<Real>(1.0);
      a_point = lso + lsd;
      return 2;
    }

    a_point = lso + a_u * lsd;
    return 0;
  }	//End: LineSegment::IntersectionPlaneLineSegment()
}

#endif