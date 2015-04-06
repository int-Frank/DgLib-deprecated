#include "BoundedNormal.h"
#include "dgmath.h"
#include "SimpleRNG.h"

namespace Dg
{
  template<typename Real>
  BoundedSND<Real>::~BoundedSND()
  {
    delete[] m_values;
  }

  template<typename Real>
  void BoundedSND<Real>::_init(const BoundedSND<Real>& a_other)
  {
    Clean();

    if (a_other.m_nValues)
    {
      m_nValues = a_other.m_nValues;
      m_values = (Real*)malloc(m_nValues * sizeof(Real));
      memcpy(m_values, a_other.m_values, m_nValues * sizeof(Real));
    }
  }

  template<typename Real>
  BoundedSND<Real>::BoundedSND(const BoundedSND& a_other) : m_values(nullptr), m_nValues(0)
  {
    _init(a_other);
  }

  template<typename Real>
  BoundedSND<Real>& BoundedSND<Real>::operator=(const BoundedSND<Real>& a_other)
  {
    if (this == &a_other)
    {
      return *this;
    }

    _init(a_other);

    return *this;
  }

  template<typename Real>
  void BoundedSND<Real>::Clean()
  {
    free(m_values);
    m_values = nullptr;
    m_nValues = 0;
  }

  template<typename Real>
  Dg_Result BoundedSND<Real>::Init(Real a_mean,
                                   Real a_sd,
                                   Real a_lower,
                                   Real a_upper,
                                   unsigned int a_nValues)
  {
    Clean();

    //Check input
    if (a_lower >= a_upper
     || Dg::IsZero(a_sd)
     || a_nValues == 0)
    {
      return DgR_OutOfBounds;
    }

    Real normLower = (a_lower - a_mean) / a_sd;
    Real normUpper = (a_upper - a_mean) / a_sd;

    m_nValues = a_nValues;
    m_values = new Real[m_nValues];

    for (unsigned i = 0; i < a_nValues; i++)
    {
      double c = static_cast<double>(normLower + (normUpper - normLower) * static_cast<Real>(i) / static_cast<Real>(m_nValues));
      c = 2.0 * c - 1;
      Real inverfResult = static_cast<Real>(inverf(c));
      m_values[i] = inverfResult * a_sd + a_mean;
    }

    return DgR_Success;
  }


  template<typename Real>
  Real BoundedSND<Real>::Get() const
  {
    if (m_nValues == 0)
    {
      return 0.0;
    }

    SimpleRNG rng;
    unsigned int index = rng.GetUint(0, m_nValues - 1);
    return m_values[index];
  }

}