//! @file DgR3Vector_ancillary.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015

#ifndef DGR3VECTOR_ANCILLARY_H
#define DGR3VECTOR_ANCILLARY_H

#include "DgR3Vector.h"
#include "DgRNG.h"

namespace Dg
{
  namespace R3
  {
    //-------------------------------------------------------------------------------
    //		@	GetRandomVector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> GetRandomVector()
    {
      RNG generator;

      Real theta = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(2.0) * Dg::Constants<Real>::PI);
      Real rho = generator.GetUniform<Real>(static_cast<Real>(-1.0), static_cast<Real>(1.0));

      Real val = sqrt(static_cast<Real>(1.0) - rho * rho);

      Real x = val * cos(theta);
      Real y = val * sin(theta);
      Real z = rho;

      return Vector<Real>({ x, y, z, static_cast<Real>(0.0) });
    }	//End: GetRandomVector()


    //-------------------------------------------------------------------------------
    //		@ GetRandomOrthonormalVector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> GetRandomOrthonormalVector(Vector<Real> const & a_axis)
    {
      RNG gen;
      Vector<Real> perp = a_axis.Perpendicular();
      Vector<Real> crs = a_axis.Cross(perp);
      Real phi = gen.GetUniform(static_cast<Real>(0.0)
        , Dg::Constants<Real>::PI * static_cast<Real>(2.0));

      return (cos(phi) * perp + sin(phi) * crs);
    }	//End: GetRandomOrthonormalVector()


    //-------------------------------------------------------------------------------
    //		@ GetRandomVector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> GetRandomVector(Vector<Real> const & a_axis, Real theta)
    {
      RNG generator;

      Dg::WrapNumber(static_cast<Real>(0.0)
        , Dg::Constants<Real>::PI
        , theta);

      Real bound = cos(Dg::Constants<Real>::PI - theta);
      Real x = generator.GetUniform(static_cast<Real>(-1.0), bound);
      Real phi = Dg::Constants<Real>::PI - acos(x);
      return (cos(phi) * a_axis + sin(phi) * GetRandomOrthonormalVector(a_axis));
    }	//End: GetRandomVector()
  }
}

#endif