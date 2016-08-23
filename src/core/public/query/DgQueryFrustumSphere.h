//! @file DgQueryFrustumSphere.h
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
      int value;
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
    result.value = IntersectType::CompletelyInside;
    for (int i = 0; i < 6; ++i)
    {
      Real dist = a_fdata.planes[i].SignedDistance(a_sphere.Center());
      if (dist <= -a_sphere.Radius())
      {
        result.value = IntersectType::CompletelyOutside;
        return result;
      }

      if (abs(dist) < a_sphere.Radius())
      {
        result.value = IntersectType::Intersecting;
      }
    }
    return result;
  } //End: TIQuery::operator()
}

#endif