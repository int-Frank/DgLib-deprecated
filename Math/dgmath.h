//! @file dgmath.h
//!
//! @author Frank Hart
//! @date 4/8/2015
//!
//! This files contains a variety of useful math functions and constants.
#ifndef COMMONMATH_H
#define COMMONMATH_H

#include <math.h>
#include <stdint.h>

#include "utility.h"

namespace Dg
{
  //! @ingroup Math_constants
  //! @{

  enum EulerOrder
  {
    XYZ,
    XZY,
    YXZ,
    YZX,
    ZYX,
    ZXY,
    XYX,
    XZX,
    YXY,
    YZY,
    ZYZ,
    ZXZ
  };

  const float PI_f            = 3.141592653589793238462643383279f;
  const float INVPI_f         = 0.31830988618379067153776752674503f;
  const float EPSILON_f       = 1.0e-4f;
  const float SQRT2_f         = 1.4142135623730950488016887242097f;

  const double PI_d           = 3.141592653589793238462643383279;
  const double INVPI_d        = 0.31830988618379067153776752674503;
  const double EPSILON_d      = 1.0e-4;
  const double SQRT2_d        = 1.4142135623730950488016887242097;

  //! @}
  
  //! @ingroup Math_functions
  //! @{

  //! Finds log base 2 of a 32-bit integer.
  uint32_t Log2(uint32_t input);

  //! Floors a number to the nearest power of 2.
  uint32_t FloorPower2(uint32_t input);

  //! Inverse error function. Uses Mclaurin series expansion approximation.
  //! @param a_nTerms Number of terms in the series expansion to use. 0 denotes maximum number.
  double inverf(double a_x, unsigned a_nTerms = 16);

  //! Wrap a number to a range.
  template<typename Real>
  void WrapNumber(Real lower, Real upper, Real& val)
  {
    if (val < lower || val > upper)
    {
      Real range = upper - lower;
      val = val - range * Real(floor((val - lower) / range));
    }

  }	//End: WrapNum()


  //! Wraps val to [-PI, PI].
  template<typename Real>
  void WrapAngle(Real& val)
  {
    val = val - static_cast<Real>(PI_d * 2.0)*floor(val / static_cast<Real>(PI_d * 2)) + static_cast<Real>(PI_d);
  }	//End: WrapAngle()


  //! Clamp a number to a range.
  template<typename Real>
  void ClampNumber(Real lower, Real upper, Real& val)
  {
    if (val < lower)
      val = lower;
    else if (val > upper)
      val = upper;

  }	//End: ClampNumber()


  //! Is the argument close to zero?
  template<typename Real>
  bool IsZero(Real a)
  {
    return (abs(a) < static_cast<Real>(EPSILON_d));

  }	//End: IsZero()


  //! Are a and b close to being equal?
  template<typename Real>
  bool AreEqual(Real a, Real b)
  {
    return (Dg::IsZero(a - b));

  }	//End: AreEqual()

  //! @}

}

#endif