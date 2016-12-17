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
#include <limits.h>

#include "DgTypes.h"
#include "impl/DgMath_impl.h"

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

  template<typename Real>
  struct Constants
  {};

  template<>
  struct Constants<float>
  {
    static float const PI;
    static float const INVPI;
    static float const EPSILON;
    static float const SQRT2;
    static float const INVSQRT2;
  };

  template<>
  struct Constants<double>
  {
    static double const PI;
    static double const INVPI;
    static double const EPSILON;
    static double const SQRT2;
    static double const INVSQRT2;
  };

  unsigned const N_C_INVERF   = 512;

  //! @}
  
  //! @ingroup DgMath_functions
  //! @{

  //! Reverse bits of the integer
  template<typename T>
  T ReverseBits(T a_val)
  {
    static_assert(impl::IsIntType<T>::value, "Can only reverse bits of int types.");
    T result(0);
    for (int i = 0; i < sizeof(T); ++i)
    {
      result |= (BitReverseTable256[(a_val >> (i * CHAR_BIT)) & 0xff] << (sizeof(T) - i) * CHAR_BIT);
    }
    return result;
  }

  //! Test to see if a number is prime
  template<typename T>
  bool IsPrime(T a_val)
  {
    if (a_val <= 1) return false;
    if (a_val <= 3) return true;
    if (a_val % 2 == 0 || a_val % 3 == 0) return false;

    T maxi = (T(1) << ((CHAR_BIT * sizeof(T)) / 2)) - 1; //Root of the highest square T can hold before overflow.
    for (T i = 5; i*i <= a_val && i <= maxi; i += 6)
    {
      if (a_val % i == 0 || a_val % (i + 2) == 0)
      {
        return false;
      }
    }
    return true;
  }

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
    val = val - static_cast<Real>(Constants<Real>::PI * 2.0)*floor((val + Constants<Real>::PI) / (Constants<Real>::PI * 2.0));
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
    return (abs(a) < Dg::Constants<Real>::EPSILON);

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