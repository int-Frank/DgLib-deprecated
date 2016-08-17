//! @file DgTIFrustumSphere.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: TIQuery

#ifndef DGQUERYFRUSTUMSPHERE_H
#define DGQUERYFRUSTUMSPHERE_H

#include "DgTIQuery.h"
#include "DgQueryLineSphere.h"
#include "../DgFrustum.h"
#include "../DgSphere.h"

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //!
  //! Test for intersection between a frustum and a sphere
  template <typename Real>
  class TIQuery<Real, FrustumData<Real>, Sphere<Real>>
  {
  public:

    //! Query return data
    struct Result
    {
      bool isIntersecting;
    };

    //! Perform the query
    Result operator()(FrustumData<Real> const &, Sphere<Real> const &);
  };

  //! Template alias for convenience.
  template<typename Real>
  using TIFrustumSphere = TIQuery<Real, FrustumData<Real>, Sphere<Real>>;

  //--------------------------------------------------------------------------------
  //	@	TIQuery::operator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  typename TIQuery<Real, FrustumData<Real>, Sphere<Real>>::Result
    TIQuery<Real, FrustumData<Real>, Sphere<Real>>::operator()
    (FrustumData<Real> const & a_fdata, Sphere<Real> const & a_sphere)
  {
    Result result;

    int touchingPlanes(0);
    int nTouchingPlanes(0);
    bool centerInside = true;
    for (int i = 0; i < 6; ++i)
    {
      Real dist = a_fdata.planes[i].SignedDistance(a_sphere.Center());
      Real absDist = abs(dist);
      if (dist < static_cast<Real>(0.0))
      {
        if (absDist >= a_sphere.Radius())
        {
          result.isIntersecting = false;
          return result;
        }
        centerInside = false;
      }

      if (absDist < a_sphere.Radius()) 
      {
        touchingPlanes |= (1 << i);
        ++nTouchingPlanes;
      }
    }

    if (centerInside || nTouchingPlanes == 1)
    {
      result.isIntersecting = true;
      return result;
    }

    int ind = FDataConstants::PVMap[touchingPlanes];
    if (nTouchingPlanes == 2)
    {
      TILineSphere<Real> query;
      TILineSphere<Real>::Result queryRes(query(a_fdata.edges[ind], a_sphere));
      result.isIntersecting = queryRes.isIntersecting;
      return result;
    }

    //Else the sphere must be intersecting all three planes.
    //Check if vertex is inside sphere.
    result.isIntersecting = (SquaredDistance(a_sphere.Center(), a_fdata.vertices[ind]) >= a_sphere.Radius() * a_sphere.Radius());

    return result;
  } //End: TIQuery::operator()

  ////--------------------------------------------------------------------------------
  ////	@	TIQuery::operator()
  ////--------------------------------------------------------------------------------
  //template<typename Real>
  //typename TIQuery<Real, FrustumData<Real>, Sphere<Real>>::Result
  //  TIQuery<Real, FrustumData<Real>, Sphere<Real>>::operator()
  //  (FrustumData<Real> const & a_fdata, Sphere<Real> const & a_sphere)
  //{
  //  Result result;
  //
  //  //Find which quadrant we are in.
  //  Vector4<Real> s(a_sphere.Center() - a_fdata.center);
  //  Real f(s.Dot(a_fdata.basis[0]));
  //  Real l(s.Dot(a_fdata.basis[1]));
  //  Real u(s.Dot(a_fdata.basis[2]));
  //
  //  int vertex(0);
  //
  //  if (u > static_cast<Real>(0.0)) vertex |= FrustumData<Real>::Top;
  //  if (f > static_cast<Real>(0.0)) vertex |= FrustumData<Real>::Far;
  //  if (l > static_cast<Real>(0.0)) vertex |= FrustumData<Real>::Left;
  //
  //  int touchingPlanes(0);
  //  bool centerInside = true;
  //  for (int i = 0; i < 3; ++i)
  //  {
  //    Real dist = a_fdata.planes[FrustumData<Real>::VPlanes[vertex][i]].SignedDistance(a_sphere.Center());
  //    Real absDist = abs(dist);
  //    if (dist < static_cast<Real>(0.0))
  //    {
  //      if (absDist >= a_sphere.Radius())
  //      {
  //        result.isIntersecting = false;
  //        return result;
  //      }
  //      centerInside = false;
  //    }
  //
  //    if (absDist < a_sphere.Radius()) touchingPlanes |= (1 << i);
  //  }
  //
  //  if (centerInside
  //    || touchingPlanes == 1
  //    || touchingPlanes == 2
  //    || touchingPlanes == 4)
  //  {
  //    result.isIntersecting = true;
  //    return result;
  //  }
  //
  //  if (touchingPlanes != 7)
  //  {
  //    //Convert the flag to the relavent index in the edge list.
  //    int ind = (touchingPlanes == 3) ? 0 : (touchingPlanes == 5) ? 2 : 3;
  //    TILineSphere<Real> query;
  //    TILineSphere<Real>::Result queryRes(query(a_fdata.edges[FrustumData<Real>::VEdges[vertex][ind]], a_sphere));
  //    result.isIntersecting = queryRes.isIntersecting;
  //    return result;
  //  }
  //
  //  //Else the sphere must be intersecting all three planes.
  //  //Check if vertex is inside sphere.
  //  result.isIntersecting = (SquaredDistance(a_sphere.Center(), a_fdata.vertices[vertex]) >= a_sphere.Radius() * a_sphere.Radius());
  //  
  //  return result;
  //} //End: TIQuery::operator()

}

#endif