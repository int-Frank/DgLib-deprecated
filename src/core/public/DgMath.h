//! @file DgMath.h
//!
//! @author Frank Hart
//! @date 4/8/2015
//!
//! This files contains a variety of useful math functions and constants.
#ifndef DGMATH_H
#define DGMATH_H

#include <cmath>
#include <stdint.h>

#include "DgTypes.h"
#include "DgMath.inl"

//! @ingroup DgMath_constants
#define USE_PRECISION_32
//#define USE_PRECISION_64

namespace Dg
{
	
  //! @ingroup DgMath_constants
  //! @{

  enum class EulerOrder
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

  float const PI_f            = 3.141592653589793238462643383279f;
  float const INVPI_f         = 0.31830988618379067153776752674503f;
  float const EPSILON_f       = 1.0e-4f;
  float const SQRT2_f         = 1.4142135623730950488016887242097f;
  float const INVSQRT2_f      = 0.70710678118654752440084436210485f;

  double const PI_d           = 3.141592653589793238462643383279;
  double const INVPI_d        = 0.31830988618379067153776752674503;
  double const EPSILON_d      = 1.0e-8;
  double const SQRT2_d        = 1.4142135623730950488016887242097;
  double const INVSQRT2_d     = 0.70710678118654752440084436210485;

  unsigned const N_C_INVERF   = 512;

#ifdef USE_PRECISION_32
  float const PI              = PI_f;
  float const INVPI           = INVPI_f;
  float const EPSILON         = EPSILON_f;
  float const SQRT2           = SQRT2_f;
  float const INVSQRT2        = INVSQRT2_f;
#else
  double const PI             = PI_d;
  double const INVPI          = INVPI_d;
  double const EPSILON        = EPSILON_d;
  double const SQRT2          = SQRT2_d;
  double const INVSQRT2       = INVSQRT2_d;
#endif

  //! @}
  
  //! @ingroup DgMath_functions
  //! @{

  //! Finds log base 2 of a 32-bit integer.
  uint32_t Log2(uint32_t input);

  //! Floors a number to the nearest power of 2.
  uint32_t FloorPower2(uint32_t input);

  //! Returns the next power of 2.
  uint32_t NextPower2(uint32_t input);

  //! Inverse error function. Uses Mclaurin series expansion approximation.
  //! The template param nTerms represents the number of terms in the series expansion to use.
  //!
  //! @return Result
  //!
  //! @param a_x Input: -1 < x < 1
  template<typename Real, unsigned nTerms = 16>
  Real inverf(Real a_x)
  {
	  static_assert(nTerms <= N_C_INVERF, "Max terms for the inverf is 512");

	  if (a_x < static_cast<Real>(-1.0) || a_x > static_cast<Real>(1.0))
	  {
		  return static_cast<Real>(0.0);
	  }

	  Real x0Sq = a_x * a_x;
	  Real x = a_x;
	  Real result = static_cast<Real>(0.0);
	  
	  for (unsigned i = 0; i < nTerms; ++i)
	  {
		  result += x * static_cast<Real>(impl::C_INVERF[i]);
		  x *= x0Sq;
	  }

	  return result;
  }

  //! Set bits within an integer type.
  template<typename IntType, unsigned Position, unsigned Length>
  IntType SetBitSet(IntType a_input, IntType a_value)
  {
    IntType mask = (IntType(1) << (Length + 1)) - 1;
    a_value &= mask;
    mask <<= Position;
    mask = ~mask;
    a_input &= mask;
    a_input |= (a_value << Position);
    return a_input;
  }// End: SetBitSet()


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
    val = val - static_cast<Real>(PI_d * 2.0)*floor((val + PI) / static_cast<Real>(PI_d * 2.0));
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
    return (abs(a) < static_cast<Real>(EPSILON));

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