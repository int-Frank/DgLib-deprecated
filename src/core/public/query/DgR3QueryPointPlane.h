//! @file DgR3QueryPointPlane.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYPOINTPLANE_H
#define DGR3QUERYPOINTPLANE_H

#include "DgR3CPQuery.h"
#include "..\DgR3Plane.h"

namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Point, Plane.
    template <typename Real>
    class CPQuery<Real, Vector<Real>, Plane<Real>>
    {
    public:

      //! Query result data
      struct Result
      {
        //! Distance between point and line
        Real distance;

        //! Signed distance between point and line
        Real sDistance;

        //! Closest point on the plane to the point.
        Vector<Real> cp;
      };

      //! Perform query
      Result operator()(Vector<Real> const &, Plane<Real> const &);
    };

    //! Template alias for convenience
    template<typename Real>
    using CPPointPlane = CPQuery<Real, Vector<Real>, Plane<Real>>;


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, Vector<Real>, Plane<Real>>::Result
      CPQuery<Real, Vector<Real>, Plane<Real>>::operator()
      (Vector<Real> const & a_point, Plane<Real> const & a_plane)
    {
      Result result;

      result.sDistance = a_plane.SignedDistance(a_point);
      result.distance = abs(result.sDistance);
      result.cp = a_point - result.sDistance * a_plane.Normal();

      return result;
    } //End: CPQuery::operator()
  }
}

#endif