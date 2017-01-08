//! @file DgR2Vector3_ancillary.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector3

#ifndef DGR2VECTOR3_ANCILLARY_H
#define DGR2VECTOR3_ANCILLARY_H

#include "DgR2Vector3.h"
#include "DgRNG.h"

namespace Dg
{
  namespace R2
  {
    //-------------------------------------------------------------------------------
    //		@	GetRandomVector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector3<Real> GetRandomVector()
    {
      RNG generator;

      Real theta = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(2.0) * Dg::Constants<Real>::PI);

      Real x = cos(theta);
      Real y = sin(theta);

      return Vector3<Real>({ x, y, static_cast<Real>(0.0) });
    }	//End: GetRandomVector()
  }
}

#endif