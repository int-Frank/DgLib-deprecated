#include "BoundedNormal.h"
#include "dgmath.h"
#include "SimpleRNG.h"

namespace Dg
{
  BoundedSND::~BoundedSND()
  {
    delete[] m_values;
  }

  void BoundedSND::_init(const BoundedSND& a_other)
  {
    Clean();

    if (a_other.m_nValues)
    {
      m_nValues = a_other.m_nValues;
      m_values = (double*)malloc(m_nValues * sizeof(double));
      memcpy(m_values, a_other.m_values, m_nValues * sizeof(double));
    }
  }

  BoundedSND::BoundedSND(const BoundedSND& a_other) : m_values(nullptr), m_nValues(0)
  {
    _init(a_other);
  }

  BoundedSND& BoundedSND::operator=(const BoundedSND& a_other)
  {
    if (this == &a_other)
    {
      return *this;
    }

    _init(a_other);

    return *this;
  }

  void BoundedSND::Clean()
  {
    free(m_values);
    m_values = nullptr;
    m_nValues = 0;
  }

  Dg_Result BoundedSND::Init(double a_mean,
                             double a_sd,
                             double a_lower,
                             double a_upper,
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

    double normLower = (a_lower - a_mean) / a_sd;
    double normUpper = (a_upper - a_mean) / a_sd;

    //Range cannot be outside of 6 standard deviations.
    if (normLower < -6.0 || normUpper > 6.0)
    {
      return DgR_OutOfBounds;
    }

    double dCount = static_cast<double>(a_nValues);

    double intgL = IntegrateND(normLower);
    double intgU = IntegrateND(normUpper);

    m_nValues = a_nValues;
    m_values = (double*)malloc(m_nValues * sizeof(double));

    for (unsigned int i = 0; i < m_nValues; ++i)
    {
      double intgX = (intgL + (intgU - intgL) * static_cast<double>(i) / m_nValues);
      double x(0.0);
      if (!SolveIntegralND(intgX, x))
      {
        Clean();
        return DgR_Failure;
      }
      m_values[i] = (x * a_sd) + a_mean;
    }

    return DgR_Success;
  }


  double BoundedSND::Get() const
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