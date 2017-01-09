//! @file DgR2QuerySegmentSegment.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016

#ifndef DGR2QUERYSEGMENTSEGMENT_H
#define DGR2QUERYSEGMENTSEGMENT_H

#include "DgQueryCommon.h"
#include "DgR2CPQuery.h"
#include "DgR2TIQuery.h"
#include "..\DgR2Segment.h"

namespace Dg
{
  namespace R2
  {
    //! @ingroup DgMath_geoQueries
    //! Intersection test: Segment, Segment
    template <typename Real>
    class TIQuery<Real, Segment<Real>, Segment<Real>>
    {
    public:

      //! Query return data
      struct Result
      {
        bool isIntersecting;
      };

      //! Perform query.
      Result operator()(Segment<Real> const &, Segment<Real> const &);
    };

    //! @ingroup DgMath_geoQueries
    //! Distance and closest-point query: Line Segment, Line Segment
    template <typename Real>
    class CPQuery<Real, Segment<Real>, Segment<Real>>
    {
    public:

      //! Query return data
      struct Result
      {
        //! Fraction along segment 0 to closest point to the segment 1
        Real u0;

        //! Fraction along segment 1 to closest point to the segment 0
        Real u1;

        //! Closest point on segment 0 to segment 1
        Vector3<Real> cp0;

        //! Closest point on segment 1 to segment 0
        Vector3<Real> cp1;

        //! Return code. Codes include:
        //!   Intersecting, Parallel, Overlapping
        QueryCode code;
      };

      //! Perform query.
      Result operator()(Segment<Real> const &, Segment<Real> const &);
    };

    //! Template alias for convenience.
    template<typename Real>
    using CPSegmentSegment = CPQuery<Real, Segment<Real>, Segment<Real>>;

    template<typename Real>
    using TISegmentSegment = TIQuery<Real, Segment<Real>, Segment<Real>>;


    //--------------------------------------------------------------------------------
    //	@	CPQuery::operator()
    //--------------------------------------------------------------------------------
    template<typename Real>
    typename CPQuery<Real, Segment<Real>, Segment<Real>>::Result
      CPQuery<Real, Segment<Real>, Segment<Real>>::operator()
      (Segment<Real> const & a_seg0, Segment<Real> const & a_seg1)
    {
      Result result;

      Vector3<Real> const & dir0 = a_seg0.Direction();
      Vector3<Real> const & dir1 = a_seg1.Direction();

      Vector3<Real> w = a_seg0.GetP0() - a_seg1.GetP0();
      Real denom = PerpDot(a_seg0.Direction(), a_seg1.Direction());
      Real u0_numerator = PerpDot(a_seg1.Direction(), w);
      Real u1_numerator = PerpDot(a_seg0.Direction(), w);

      //Parallel, or one or both segments length is zero.
      if (Dg::IsZero(denom))
      {
        Real ls0 = dir0.LengthSquared();
        Real ls1 = dir1.LengthSquared();

        //If length of seg_0 is zero
        if (Dg::IsZero(ls0))
        {
          result.u0 = static_cast<Real>(0);

          //If both segments have zero length
          if (Dg::IsZero(ls1))
          {
            result.u1 = static_cast<Real>(0);
            if (Dg::IsZero(w))
            {
              result.code = QueryCode::Overlapping;
            }
            else
            {
              result.code = QueryCode::NotIntersecting
            }
          }
          //Only seg_0 has zero length
          else
          {
            result.u1 = Dot(w, a_seg1.Direction()) / a_seg1.Direction().LengthSquared();
            result.code = QueryCode::NotIntersecting;
            if (result.u1 < static_cast<Real>(0))
            {
              result.u1 = static_cast<Real>(0);
            }
            else if (result.u1 > static_cast<Real>(1))
            {
              result.u1 = static_cast<Real>(1);
            }
            else if (Dg::IsZero(PerpDot(w, dir1)))
            {
              result.code = QueryCode::Overlapping;
            }
          }
        }
        //Only seg_1 has zero length
        else if (Dg::IsZero(ls1))
        {
          result.u1 = static_cast<Real>(0);
          result.u0 = Dot(w, dir0) / dir0.LengthSquared();
          result.code = QueryCode::NotIntersecting;
          if (result.u0 < static_cast<Real>(0))
          {
            result.u0 = static_cast<Real>(0);
          }
          else if (result.u0 > static_cast<Real>(1))
          {
            result.u0 = static_cast<Real>(1);
          }
          else if (Dg::IsZero(PerpDot(w, dir0)))
          {
            result.code = QueryCode::Overlapping;
          }
        }

        //Both segments have non-zero lengths
        else
        {
          Vector3<Real> v0 = a_seg0.Direction() / a_seg0.LengthSquared();
          Vector3<Real> v1 = a_seg1.Direction() / a_seg1.LengthSquared();
          Vector3<Real> v = a_seg0.GetP1() - a_seg1.GetP0();
          Real a = Dot(w, v1);                               //s1.p0 to s0.p0 dot s1.direction
          Real b = Dot(a_seg0.GetP1() - a_seg1.GetP0(), v1); //s1.p0 to s0.P1 dor s1.direction

          if (a > static_cast<Real>(1))
          {
            if (b > static_cast<Real>(1))
            {
              if (b > a)
              {
                result.u0 = static_cast<Real>(0);
                result.u1 = static_cast<Real>(1);
              }
              else
              {
                result.u0 = static_cast<Real>(1);
                result.u1 = static_cast<Real>(1);
              }
              result.code = QueryCode::NotIntersecting;
            }
            else
            {
              result.u0 = Dot(-w, v0);
              result.u1 = static_cast<Real>(1);

              if (Dg::IsZero(u0_numerator))
              {
                result.code = QueryCode::Overlapping;
              }
              else
              {
                result.code = QueryCode::NotIntersecting;
              }
            }
          }
          else if (a < static_cast<Real>(0))
          {
            if (b < static_cast<Real>(0))
            {
              if (b > a)
              {
                result.u0 = static_cast<Real>(0);
                result.u1 = static_cast<Real>(0);
              }
              else
              {
                result.u0 = static_cast<Real>(1);
                result.u1 = static_cast<Real>(0);
              }
              result.code = QueryCode::NotIntersecting;
            }
            else
            {
              result.u0 = Dot(-w, v0);
              result.u1 = static_cast<Real>(0);

              if (Dg::IsZero(u0_numerator))
              {
                result.code = QueryCode::Overlapping;
              }
              else
              {
                result.code = QueryCode::NotIntersecting;
              }
            }
          }
          else
          {
            result.u0 = static_cast<Real>(0);
            result.u1 = Dot(w, v0);

            if (Dg::IsZero(u0_numerator))
            {
              result.code = QueryCode::Overlapping;
            }
            else
            {
              result.code = QueryCode::NotIntersecting;
            }
          }
        }
      }
      else
      {

      }

    } //End: CPQuery::operator()
  }
}

#endif