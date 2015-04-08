/*!
 * @file FastPoisson.h
 *
 * @author Frank Hart
 * @date 14/01/2014
 *
 * class : FastPoisson.h
 */

#ifndef FASTPOISSON_H
#define FASTPOISSON_H

#include <cmath>
#include <map>
#include <stdint.h>

#include "dgmath.h"

#include "DgLib_defines.h"
#include "dg_pod_map.h"

BEGIN_NAMESPACE_DGLIB

  BEGIN_NAMESPACE_IMPL

    template<typename Real>
    struct PoissonData
    {
      Real * m_values;
      uint32_t m_nValues;
      uint32_t m_mean;
      int m_refCounter;
    };
    
  END_NAMESPACE

  template<typename Real>
  class PoissonDist
  {
  public:

    PoissonDist(Real a_mean);
    ~PoissonDist();

    PoissonDist(const PoissonDist&);
    PoissonDist& operator= (const PoissonDist&);

  private:

    impl::PoissonData<Real> * m_data;

  private:

    Real P_i_u(Real m_u, uint32_t a_i);

  };

  template<typename Real>
  PoissonDist<Real>::PoissonDist(Real a_mean) : m_data(nullptr)
  {
    POD::map<uint32_t, Real> results;
    results.resize((static_cast<Real>(a_mean) + 1) * 3);

    //Begin at the mean and move out until accuracy is reached.
    int32_t i = static_cast<int32_t>(a_mean);
    int32_t delta = 0;

    while (true)
    {
      int32_t cur_i = i + delta;

      if (cur_i >= 0)
      {
        Real p = P_i_u(a_mean, cur_i);

        if (IsZero(p))
        {
          break;
        }

        results.insert(p, cur_i);
      }

      delta *= -1;

      if (delta >= 0)
      {
        delta++;
      }
    }

    m_data = new impl::PoissonData<Real>();

    m_data->m_nValues = results.size();
    m_data->m_refCounter = 1;
    m_data->m_mean = a_mean;
    m_data->m_values = new Real[m_data->m_nValues];

    m_data->m_values[0] = results[0];

    for (int i = 1; i < results.size(); ++i)
    {
      m_data->m_values[i] = m_data->m_values[i - 1] + results[i];
    }

  }


  template<typename Real>
  PoissonDist<Real>::~PoissonDist()
  {
    if (m_data != nullptr && m_data->m_refCounter-- == 0)
    {
      delete m_data->m_values;
      delete m_data;
    }
  }

  template<typename Real>
  PoissonDist<Real>::PoissonDist(const PoissonDist& a_other)
    :m_data(nullptr)
  {
    m_data = a_other.m_data;
    m_data->m_refCounter++;
  }

  template<typename Real>
  PoissonDist<Real>& PoissonDist<Real>::operator=(const PoissonDist& a_other)
  {
    if (this == &other)
    {
      return *this;
    }

    if (m_data != nullptr && m_data->m_refCounter-- == 0)
    {
      delete m_data->m_values;
      delete m_data;
    }

    m_data = a_other.m_data;
    m_data->m_refCounter++;

    return *this;
  }

  template<typename Real>
  Real PoissonDist<Real>::P_i_u(Real m_u, uint32_t a_i)
  {
    Real result = static_cast<Real>(1.0);

    for (uint32_t i_n = a_i; i_n != 0; i_n--)
    {
      result *= (m_u / static_cast<Real>(i_n));
    }

    return result * exp(-m_u);
  }

  /*!
   * @ingroup math
   *
   * @class FastPoisson
   *
   * @brief Returns number of events in a given time interval
   *
   * Given a rate of occurance and a time interval, this class
   * returns a number of events following the poisson distribution.
   *
   * @author Frank Hart
   * @date 14/01/2014
   */
  //template<typename Real>
  //class FastPoisson
  //{
  //public:

	 // FastPoisson(){}
	 // ~FastPoisson() {}

	 // FastPoisson(const FastPoisson&);
	 // FastPoisson& operator= (const FastPoisson&);

  //  void Init(Real a_mean);

	 // //! Generate number of events.
	 // uint32 Get(Real dt);

	 // //! Get the rate
	 // Real GetRate() const { return rate; }

	 // //! Set the rate and number of stddevs above
	 // //! the rate to limit results.
	 // void Set(Real _rate);

  //private:

	 // NormalDistributionBounded<Real> distribution;

	 // Real rate;

  //};


  ////--------------------------------------------------------------------------------
  ////	@	FastPoisson::FastPoisson()
  ////--------------------------------------------------------------------------------
  ////		Copy constructor
  ////--------------------------------------------------------------------------------
  //template<typename Real>
  //FastPoisson<Real>::FastPoisson(const FastPoisson& other): 
  //distribution(other.distribution), rate(other.rate)
  //{
  //}	//End: FastPoisson::FastPoisson()


  ////--------------------------------------------------------------------------------
  ////	@	FastPoisson::operator=()
  ////--------------------------------------------------------------------------------
  ////		Assignment
  ////--------------------------------------------------------------------------------
  //template<typename Real>
  //FastPoisson<Real>& FastPoisson<Real>::operator=(
	 // const FastPoisson& other)
  //{
	 // if (this == &other)
		//  return *this;

	 // distribution = other.distribution;
	 // rate = other.rate;

	 // return *this;

  //}	//End: FastPoisson::operator=()


  ////--------------------------------------------------------------------------------
  ////	@	FastPoisson::Set()
  ////--------------------------------------------------------------------------------
  ////		Set up the distribution
  ////--------------------------------------------------------------------------------
  //template<typename Real>
  //void FastPoisson<Real>::Set(Real _rate)
  //{
	 // if (_rate < Real(0.0))
		//  rate = Real(1.0);
	 // else
		//  rate = _rate;

  //}	//End: FastPoisson::Set()


  ////--------------------------------------------------------------------------------
  ////	@	FastPoisson::Set()
  ////--------------------------------------------------------------------------------
  ////		Set up the distribution
  ////--------------------------------------------------------------------------------
  //template<typename Real>
  //uint32_t FastPoisson<Real>::Get(Real dt)
  //{
	 // //Set the distribution values
	 // Real mean = rate * dt;
	 // Real stddev = DgSqrt(mean);

	 // Real lower = Real(0.0);
	 // Real upper = mean + Real(3.0) * stddev;

	 // //Set the bounds
	 // distribution.Set(mean, stddev, lower, upper);

	 // //Increase upperbound to max
	 // distribution.upperBound = (NORMAL_DISTRIBUTION_TABLE_SIZE - 1);

	 // //Increment lower bound, else a value less then 0 migh be returned
	 // distribution.lowerBound++;
	
	 // return uint32(distribution.Get());

  //}	//End: FastPoisson::FastPoisson()

END_NAMESPACE
#endif