//! @file DgRay.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Ray

#ifndef DGRAY_H
#define DGRAY_H

#include "DgVector4.h"
#include "DgPlane.h"
#include "DgLine.h"
#include "dgmath.h"

namespace Dg
{
  //! @ingroup DgMath_types
  //!
  //! @class Ray
  //!
  //! A Ray is defined by a point of origin and direction extending infinitely in one
  //! direction. It is represented by a Point4 and a normalized Vector4. This file
  //! also declares methods to test Rays against other geometric entities.
  //!
  //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
  //!
  //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
  //! @date 1/06/2016
  template<typename Real>
  class Ray
  {
  public:
    //! Default constructor
    Ray();

    //! Construct from an origin and direction
    Ray(Vector4<Real> const & origin, Vector4<Real> const & direction);
    ~Ray() {}

    //! Copy constructor
    Ray(Ray const &);

    //! Assignment
    Ray& operator=(Ray const &);

    //! Get the origin of the line
    Vector4<Real> const & Origin() const { return m_origin; }

    //! Get the direction of the line
    Vector4<Real> const & Direction() const { return m_direction; }

    //! Get the origin and direction of the line
    void Get(Vector4<Real>& origin, Vector4<Real>& direction) const;

    //! Comparison
    bool operator== (Ray const &) const;

    //! Comparison
    bool operator!= (Ray const &) const;

    //! Set line from an origin and direction
    void Set(Vector4<Real> const & origin, Vector4<Real> const & direction);

    //! Closest point on a line to a point.
    //!
    //! @return 0: Closest point lies on the ray
    //! @return 1: Closest point is the ray origin (point lies behind the ray)
    //!
    //! @param[in] a_pIn Input point
    //! @param[out] a_pOut Closest point on the ray
    //! @param[out] a_u Distance along the ray to the closest point.
    int ClosestPoint(Vector4<Real> const & a_pIn,
                     Vector4<Real> & a_pOut,
                     Real & a_u) const;

  private:

    //Data members
    Vector4<Real> m_origin;
    Vector4<Real> m_direction;
  };


  //--------------------------------------------------------------------------------
  //	@ Ray::Ray()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Ray<Real>::Ray() : m_origin(Vector4<Real>::Origin())
    , m_direction(Vector4<Real>::xAxis())
  {
  }	//End: Ray::Ray()


  //--------------------------------------------------------------------------------
  //  @ Ray::Ray()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Ray<Real>::Ray(Vector4<Real> const & a_origin, Vector4<Real> const & a_direction)
  {
    Set(a_origin, a_direction);
  }	//End: Ray::Ray()


  //--------------------------------------------------------------------------------
  //  @ Ray::Ray()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Ray<Real>::Ray(Ray<Real> const & a_other) : m_origin(a_other.m_origin)
    , m_direction(a_other.m_direction)
  {
  }	//End: Ray::Ray()


  //--------------------------------------------------------------------------------
  //  @ Ray::operator=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Ray<Real> & Ray<Real>::operator=(Ray<Real> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_direction = a_other.m_direction;

    return *this;
  }	//End: Ray::operator=()


  //--------------------------------------------------------------------------------
  //  @ Ray::Get()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Ray<Real>::Get(Vector4<Real> & a_origin, Vector4<Real> & a_direction) const
  {
    a_origin = m_origin;
    a_direction = m_direction;
  }	//End: Ray::Get()


  //--------------------------------------------------------------------------------
  //  @ Ray::operator==()
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool Ray<Real>::operator==(Ray<Real> const & a_other) const
  {
    return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
  }	//End: Ray::operator==()


  //--------------------------------------------------------------------------------
  //  @ Ray::operator!=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool Ray<Real>::operator!=(Ray<Real> const & a_other) const
  {
    return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
  }	//End: Ray::operator!=()


  //--------------------------------------------------------------------------------
  //  @ Ray::Set()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Ray<Real>::Set(Vector4<Real> const & a_origin, Vector4<Real> const & a_direction)
  {
    //Assign
    m_origin = a_origin;
    m_origin.w() = static_cast<Real>(1.0);

    m_direction = a_direction;
    m_direction.w() = static_cast<Real>(0.0);

    m_direction.Normalize();

  }	//End: Ray::Set()


  //--------------------------------------------------------------------------------
  //  @ Ray::ClosestPoint()
  //--------------------------------------------------------------------------------
  template<typename Real>
  int Ray<Real>::ClosestPoint(Vector4<Real> const & a_pIn,
                              Vector4<Real> & a_pOut,
                              Real & a_u) const
  {
    Vector4<Real> w(a_pIn - m_origin);
    Real proj = w.Dot(m_direction);

    if (proj < static_cast<Real>(0.0))
    {
      a_u = static_cast<Real>(0.0);
      a_pOut = m_origin;
      return 1;
    }

    a_u = proj / m_direction.Dot(m_direction);
    a_pOut = m_origin + a_u * m_direction;
    return 0;
  }	//End: Ray::ClosestPoint()


  //! @ingroup Math_gTests
  //!
  //! Closest points between two Rays.
  //!
  //! @param[in] a_ray0 Input ray
  //! @param[in] a_ray1 Input ray
  //! @param[out] a_u0 Distance along a_ray0 to the closest point to a_ray1
  //! @param[out] a_u1 Distance along a_ray1 to the closest point to a_ray0
  //! @param[out] a_p0 Point on a_ray0 closest to a_ray1 
  //! @param[out] a_p1 Point on a_ray1 closest to a_ray0
  //!
  //! @return 0: Success
  //! @return 1: Rays are parallel.
  template<typename Real>
  int ClosestPointsRayRay(Ray<Real> const & a_ray0, Ray<Real> const & a_ray1,
                          Real & a_u0, Real & a_u1,
                          Vector4<Real> & a_p0, Vector4<Real> & a_p1)
  {
    Vector4<Real> o0(a_ray0.Origin());
    Vector4<Real> o1(a_ray1.Origin());
    Vector4<Real> d0(a_ray0.Direction());
    Vector4<Real> d1(a_ray1.Direction());

    //compute intermediate parameters
    Vector4<Real> w0(o0 - o1);
    Real a = d0.Dot(d0);
    Real b = d0.Dot(d1);
    Real c = d1.Dot(d1);
    Real d = d0.Dot(w0);
    Real e = d1.Dot(w0);
    Real denom = a*c - b*b;

    //parameters to compute a_u0 and a_u1
    Real sn, sd, tn, td;

    int result = 0;

    //if denom is zero, try finding closest point on R2 to origin1
    if (Dg::IsZero(denom))
    {
      //clamp a_u0 to 0
      sd = td = c;
      sn = static_cast<Real>(0.0);
      tn = e;
      result++;
    }
    else
    {
      //clamp a_u0 within[0, +inf]
      sd = td = denom;
      sn = b*e - c*d;
      tn = a*e - b*d;

      //clamp a_u0 to 0
      if (sn < static_cast<Real>(0.0))
      {
        sn = static_cast<Real>(0.0);
        tn = e;
        td = c;
      }
    }

    //clamp a_u1 within [0, +inf]
    //clamp a_u1 to 0
    if (tn < static_cast<Real>(0.0))
    {
      a_u1 = static_cast<Real>(0.0);
      //clamp a_u0 to 0
      if (-d < static_cast<Real>(0.0))
      {
        a_u0 = static_cast<Real>(0.0);
      }
      else
      {
        a_u0 = -d / a;
      }
    }
    else
    {
      a_u1 = tn / td;
      a_u0 = sn / sd;
    }

    a_p0 = o0 + a_u0*d0;
    a_p1 = o1 + a_u1*d1;
    return result;
  }	//End: Ray::ClosestPointRayRay()


  //! @ingroup Math_gTests
  //!
  //! Closest points between a line and a ray.
  //!
  //! @param[in] a_ray Input ray
  //! @param[in] a_line Input line
  //! @param[out] a_ur Distance along a_ray to the closest point to a_line
  //! @param[out] a_ul Distance along a_line to the closest point to a_ray
  //! @param[out] a_pr Point on a_ray closest to a_line 
  //! @param[out] a_pl Point on a_line closest to a_ray
  //!
  //! @return 0: Success
  //! @return 1: Ray and line are parallel.
  template<typename Real>
  int ClosestPointsRayLine(Ray<Real> const & a_ray, Line<Real> const & a_line,
                           Real & a_ur, Real & a_ul,
                           Vector4<Real> & a_pr, Vector4<Real> & a_pl)
  {
    Vector4<Real> or(a_ray.Origin());
    Vector4<Real> ol(a_line.Origin());
    Vector4<Real> dr(a_ray.Direction());
    Vector4<Real> dl(a_line.Direction());

    //compute intermediate parameters
    Vector4<Real> w0(or - ol);
    Real a = dr.Dot(dr);
    Real b = dr.Dot(dl);
    Real c = dl.Dot(dl);
    Real d = dr.Dot(w0);
    Real e = dl.Dot(w0);
    Real denom = a*c - b*b;

    int result = 0;

    //if denom is zero, try finding closest point on R to L.origin
    if (Dg::IsZero(denom))
    {
      //compute closest points
      a_ur = static_cast<Real>(0.0);
      a_ul = e / c;

      a_pr = or ;
      a_pl = ol + a_ul*dl;
      result = 1;
    }
    else
    {
      // parameters to compute a_ur, a_ul
      Real sn;

      // clamp a_ur within [0,1]
      sn = b*e - c*d;

      // clamp a_ur to 0
      if (sn < static_cast<Real>(0.0))
      {
        a_ur = static_cast<Real>(0.0);
        a_ul = e / c;
      }
      // clamp a_ur to 1
      else if (sn > denom)
      {
        a_ur = static_cast<Real>(1.0);
        a_ul = (e + b) / c;
      }
      else
      {
        a_ur = sn / denom;
        a_ul = (a*e - b*d) / denom;
      }

      //compute closest points
      a_pr = or +a_ur*dr;
      a_pl = ol +a_ul*dl;
    }

    return result;

  }	//End: Ray::ClosestPointRayLine()



  //! @ingroup Math_gTests
  //!
  //! Intersection test between a plane and a ray
  //!
  //! @param[in] a_plane Input plane
  //! @param[in] a_ray Input ray
  //! @param[out] a_u Distance along the ray to the closest point on the plane
  //! @param[out] a_point Closest point from plane to ray.
  //!
  //! @return 0: Ray intersects plane
  //! @return 1: Ray lies on the plane. Output set to ray origin.
  //! @return 2: No Intersection. Ray is orthogonal to the plane normal. Output point not set.
  //! @return 3: Ray does not intersect plane; points away from plane. Output point set as the ray origin.
  template<typename Real>
  int TestPlaneRay(Plane<Real> const & a_plane, Ray<Real> const & a_ray,
                   Real & a_u, Vector4<Real> & a_point)
  {
    Vector4<Real> pn(a_plane.Normal());
    Real          po(a_plane.Offset());
    Vector4<Real> ro(a_ray.Origin());
    Vector4<Real> rd(a_ray.Direction());

    Real denom = pn.Dot(rd);

    //check if ray is parallel to plane
    if (Dg::IsZero(denom))
    {
      a_point = ro;
      a_u = 0.0;

      //check if ray is on the plane
      if (Dg::IsZero(a_plane.Distance(ro)))
      {
        return 1;
      }
      return 2;
    }

    a_u = ((ro.Dot(pn) + po) / denom);

    //ray points away from plane
    if (a_u < static_cast<Real>(0.0))
    {
      a_u = static_cast<Real>(0.0);
      a_point = ro;
      return 3;
    }

    a_point = ro + a_u * rd;
    return 0;
  }	//End: Ray::IntersectionPlaneRay()
}

#endif