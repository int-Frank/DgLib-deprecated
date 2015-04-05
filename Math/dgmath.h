//================================================================================
// @ CommonMath.h
// 
// Description: 
//
// This files contains a variety of useful math functions and constants.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef COMMONMATH_H
#define COMMONMATH_H

#include <math.h>
#include <stdint.h>

#include "config.h"
#include "utility.h"

namespace Dg
{
  extern const float PI_f;
  extern const float TWOPI_f;
  extern const float HALFPI_f;
  extern const float INVPI_f;
  extern const float EPSILON_f;
  extern const float SQRT2_f;

  extern const double PI_d;
  extern const double TWOPI_d;
  extern const double HALFPI_d;
  extern const double INVPI_d;
  extern const double EPSILON_d;
  extern const double INV_SQRT_2PI_d;
  extern const double SQRT2_d;

#if defined PRECISION_FLOAT

  const float PI            = 3.141592653589793238462643383279f;
  const float TWOPI         = 6.283185307179586476925286766559f;
  const float HALFPI        = 1.570796326794896619231321691639f;
  const float INVPI         = 0.31830988618379067153776752674503f;
  const float EPSILON       = 1.0e-4f;
  const float SQRT2         = 1.4142135623730950488016887242097f;

#elif defined PRECISION_DOUBLE

  const double PI           = 3.141592653589793238462643383279;
  const double TWOPI        = 6.283185307179586476925286766559;
  const double HALFPI       = 1.570796326794896619231321691639;
  const double INVPI        = 0.31830988618379067153776752674503;
  const double EPSILON      = 1.0e-4;
  const double SQRT2        = 1.4142135623730950488016887242097;

#endif

  /**
   * Finds log base 2 of an integer.
   */
  uint32_t Log2(uint32_t input);


  /**
  * Floors a number to the nearest power of 2
  */
  uint32_t FloorPower2(uint32_t input);

  /**
  * Integrate the standard normal distribution from -inf to a_x
  */
  double IntegrateND(double a_x);

  /**
  * Finds x, such that the integral of the SND function from -inf -> x = a_intgX.
  */
  Dg_Result SolveIntegralND(double a_intgX, double& a_out);

  /**
  * Fast integer power function
  */
  template<typename Real>
  Dg_Result FastPower(Real& a_out, Real a_num, int a_exp)
  {
    if (a_exp > 0)
    {
      a_out = a_num;
    }
    else if (a_exp < 0)
    {
      if (IsZero(a_num))
      {
        return DgR_Undefined;
      }
      a_out = Real(1.0) / a_num;
      a_exp *= -1;
    }
    else
    {
      a_out = Real(1.0);
      return DgR_Success;
    }

    for (int i = 1; i < a_exp; ++i)
      a_out *= a_out;

    return DgR_Success;
  }


  /**		
   * Wrap a number to a range
   */
  template<typename Real>
  inline void WrapNumber(Real lower, Real upper, Real& val)
  {
    if (val < lower || val > upper)
    {
      Real range = upper - lower;
      val = val - range * Real(floor((val - lower) / range));
    }

  }	//End: WrapNum()


  /**		
   * Wraps val to [-PI, PI]
   */
  template<typename Real>
  inline void WrapAngle(Real& val)
  {
    val = val - static_cast<Real>(TWOPI_d)*floor(val / static_cast<Real>(TWOPI_d)) + static_cast<Real>(PI_d);
  }	//End: WrapAngle()


  /**		
   * Clamp a number to a range
   */
  template<typename Real>
  inline void ClampNumber(Real lower, Real upper, Real& val)
  {
    if (val < lower)
      val = lower;
    else if (val > upper)
      val = upper;

  }	//End: ClampNumber()


  /**		
   * Is the argument close to zero?
   */
  template<typename Real>
  inline bool IsZero(Real a)
  {
    return (abs(a) < static_cast<Real>(EPSILON_d));

  }	//End: IsZero()


  /**		
   * Are a and b close to being equal?
   */
  template<typename Real>
  inline bool AreEqual(Real a, Real b)
  {
    return (Dg::IsZero(a - b));

  }	//End: AreEqual()

}

#endif