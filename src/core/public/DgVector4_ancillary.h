//! @file DgVector4_ancillary.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector4

#ifndef DGVECTOR4_ANCILLARY_H
#define DGVECTOR4_ANCILLARY_H

#include "DgVector4.h"
#include "DgRNG.h"

namespace Dg
{
  //-------------------------------------------------------------------------------
  //		@	GetRandomVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> GetRandomVector()
  {
    RNG generator;

    Real theta = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(2.0) * static_cast<Real>(Dg::PI_d));
    Real rho = generator.GetUniform<Real>(static_cast<Real>(-1.0), static_cast<Real>(1.0));

    Real val = sqrt(static_cast<Real>(1.0) - rho * rho);

    Real x = val * cos(theta);
    Real y = val * sin(theta);
    Real z = rho;

    return Vector4<Real>({ x, y, z, static_cast<Real>(0.0) });
  }	//End: GetRandomVector()


  //-------------------------------------------------------------------------------
  //		@ GetRandomOrthonormalVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> GetRandomOrthonormalVector(Vector4<Real> const & a_axis)
  {
    RNG gen;
    Vector4<Real> perp = Perpendicular(a_axis);
    Vector4<Real> crs = Cross(a_axis, perp);
    Real phi = gen.GetUniform(static_cast<Real>(0.0)
                            , static_cast<Real>(Dg::PI_d * 2.0));

    return (cos(phi) * perp + sin(phi) * crs);
  }	//End: GetRandomOrthonormalVector()


  //-------------------------------------------------------------------------------
  //		@ GetRandomVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> GetRandomVector(Vector4<Real> const & a_axis, Real theta)
  {
    RNG generator;

    Dg::WrapNumber(static_cast<Real>(0.0)
                 , static_cast<Real>(Dg::PI_d)
                 , theta);

    Real bound = cos(static_cast<Real>(Dg::PI_d) - theta);
    Real x = generator.GetUniform(static_cast<Real>(-1.0), bound);
    Real phi = static_cast<Real>(Dg::PI_d) - acos(x);
    return (cos(phi) * a_axis + sin(phi) * GetRandomOrthonormalVector(a_axis));
  }	//End: GetRandomVector()
}

#endif