//! @file DgRNG.h
//!
//! @author Frank Hart
//! @date 4/8/2015
//!
//! Class declaration: RNG

#ifndef DGRNG_H
#define DGRNG_H

namespace Dg
{

	//! @ingroup DgMath_types
	//!
	//!
	//! @brief A simple random number generator.
	//! Original code by John D. Cook
	//!
	//! @author Frank Hart
	//! @date 4/8/2015
  // TODO Make this a class again as then we can have multiple RNGs across
  //      multiple threads, i.e. it will be thread safe.
	namespace RNG
	{
		//! Seed the internal generator from one value.
		void SetSeed(unsigned int seed);

		//! Seed the internal generator from two values.
		void SetSeed(unsigned int seed1, unsigned int seed2);

		//! Produce a uniform random sample from the open interval (0, 1).
    //--------------------------------------------------------------------------------
    //		The method will not return either end point.
    //--------------------------------------------------------------------------------
    template<class Real>
    Real GetUniform()
    {
      // 0 <= u < 2^32
      unsigned int u = GetUint();
      // The magic number below is 1/(2^32 + 2).
      // The result is strictly between 0 and 1.
      return Real(u + 1) * Real(2.328306435454494e-10);

    }	//End: RNG::GetUniform()

		//! Produce a uniform random sample from the open interval (a, b).
    template<class Real>
    Real GetUniform(Real a, Real b)
    {
      if (b < a)
        return a;

      Real range = b - a;
      return GetUniform<Real>() * range + a;

    }	//End: RNG::GetUniform()

		//! Get random unsigned integer within the interval [a, b].
		unsigned int GetUint(unsigned int a, unsigned int b);

    //! Flip a coin
    bool FlipCoin();

		//! Get random unsigned integer.
		unsigned int GetUint();

		//! Get a Gaussian random sample with mean 0 and std 1.
    template<class Real>
    Real GetNormal()
    {
      // Use Box-Muller algorithm
      Real u1 = GetUniform<Real>();
      Real u2 = GetUniform<Real>();
      Real r = sqrt(Real(-2.0)*log(u1));
      Real theta = Real(6.283185307179586476925286766559)*u2;
      return r*sin(theta);
    }	//End: RNG::GetNormal()

		//! Get a Gaussian random sample with specified mean and standard deviation.
    template<class Real>
    Real GetNormal(Real mean, Real std)
    {
      if (std <= Real(0.0))
        return mean;
      return mean + std*GetNormal<Real>();

    }	//End: RNG::GetNormal()

		//! Get a Gamma random sample with specified shape and scale.
    template<class Real>
    Real GetGamma(Real shape, Real scale)
    {
      // Implementation based on "A Simple Method for Generating Gamma Variables"
      // by George Marsaglia and Wai Wan Tsang.  ACM Transactions on Mathematical Software
      // Vol 26, No 3, September 2000, pages 363-372.

      Real d, c, x, xsquared, v, u;

      if (shape >= Real(1.0))
      {
        d = shape - Real(0.33333333333333333333333333333333);
        c = Real(1.0) / DgSqrt(Real(9.0)*d);
        for (;;)
        {
          do
          {
            x = GetNormal<Real>();
            v = Real(1.0) + c*x;
          } while (v <= Real(0.0));
          v = v*v*v;
          u = GetUniform<Real>();
          xsquared = x*x;
          if (u < Real(1.0) - Real(0.0331)*xsquared*xsquared || DgLog(u) < Real(0.5)*xsquared + d*(Real(1.0) - v + DgLog(v)))
            return scale*d*v;
        }
      }
      else if (shape <= Real(0.0))
      {
        /*string msg = string.Format("Shape must be positive. Received {0}.", shape);
        throw new ArgumentOutOfRangeException(msg)*/
        return Real(-1.0);
      }
      else
      {
        Real g = GetGamma(shape + Real(1.0), Real(1.0));
        Real w = GetUniform<Real>();
        return scale*g*std::pow(w, Real(1.0) / shape);
      }
    }	//End: RNG::GetGamma()
	}
}


#endif