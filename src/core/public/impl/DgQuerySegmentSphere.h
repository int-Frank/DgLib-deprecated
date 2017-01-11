//! @file DgR3QuerySegmentSphere.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR3QUERYSEGMENTSPHERE_H
#define DGR3QUERYSEGMENTSPHERE_H

#include "../query/DgQueryCommon.h"
#include "DgTIQuery.h"
#include "DgFIQuery.h"
#include "..\DgR3Segment.h"
#include "..\DgR3Sphere.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //! Test for intersection between a line and a sphere.
    template <typename Real>
    class TIQuery<Real, 3, 
                  R3::Segment<Real>, 
                  R3::Sphere<Real>>
    {
    public:

      //! Query result data.
      struct Result
      {
        //! True if line intersects sphere.
        bool isIntersecting;
      };

      //! Perform query.
      Result operator()(R3::Segment<Real> const &, R3::Sphere<Real> const &);
    };


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename TIQuery<Real, 3, R3::Segment<Real>, R3::Sphere<Real>>::Result
      TIQuery<Real, 3, R3::Segment<Real>, R3::Sphere<Real>>::operator()
      (R3::Segment<Real> const & a_seg, R3::Sphere<Real> const & a_sphere)
    {
      Result result;
      Vector_generic<Real, 3> w0(a_seg.Origin() - a_sphere.Center());
      Real a = a_seg.Direction().LengthSquared();
      Real inv_a = static_cast<Real>(1.0) / a;
      Real b = inv_a * w0.Dot(a_seg.Direction());
      Real c = inv_a * (w0.LengthSquared() - a_sphere.Radius() *a_sphere.Radius());
      Real discriminant = b * b + c;

      if (discriminant < static_cast<Real>(0.0))
      {
        result.isIntersecting = false;
        return result;
      }

      //TODO Not complete
      Real irr = sqrt(b * b - static_cast<Real>(4.0) * a * c);
      Real u0 = (-b - irr) / (static_cast<Real>(2.0) * a);
      Real u1 = (-b + irr) / (static_cast<Real>(2.0) * a);
      result.isIntersecting = (u0 > static_cast<Real>(0.0) && u0 < static_cast<Real>(1.0))
        || (u1 > static_cast<Real>(0.0) && u1 < static_cast<Real>(1.0))
        || (u0 * u1 < static_cast<Real>(0.0))
        return result;
    } //End: CPQuery::operator()
  }
}

#endif