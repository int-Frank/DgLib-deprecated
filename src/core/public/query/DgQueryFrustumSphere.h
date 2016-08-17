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

#define ROBUST_FRUSTUM_SPHERE

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
#ifdef ROBUST_FRUSTUM_SPHERE
    Result result;

    bool centerInside = true;
    int nTouchingPlanes(0);
    int flaggedEdges(0);
    int flaggedVerts(0);
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

        flaggedEdges |= FDataConstants::PEMap[i];
        flaggedVerts |= FDataConstants::PVMap[i];
        ++nTouchingPlanes;
        centerInside = false;
        continue;
      }

      if (absDist < a_sphere.Radius())
      {
        flaggedEdges |= FDataConstants::PEMap[i];
        flaggedVerts |= FDataConstants::PVMap[i];
        ++nTouchingPlanes;
      }
    }

    if (centerInside || nTouchingPlanes == 1)
    {
      result.isIntersecting = true;
      return result;
    }

    //Check edges
    for (int i = 1; i < (1 << 12); i << 1)
    {
      if (flaggedEdges & i)
      {
        Vector4<Real> cp();
      }
    }

    return result;
#else
    Result result;
    result.isIntersecting = true;
    for (int i = 0; i < 6; ++i)
    {
      Real dist = a_fdata.planes[i].SignedDistance(a_sphere.Center());
      if (dist <= -a_sphere.Radius())
      {
        result.isIntersecting = false;
        return result;
      }
    }
    return result;
#endif

  } //End: TIQuery::operator()
}

#endif