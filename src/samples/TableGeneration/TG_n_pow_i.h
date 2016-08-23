#pragma once

#include "TableGenerator.h"

template<typename T>
class TG_n_pow_i : public TableGenerator<T>
{
public:

  TG_n_pow_i(T a_min, T a_max, double a_mul)
    : m_min(a_min)
    , m_max(a_max)
    , m_mul(a_mul)
  {}

  void Generate(unsigned dummy = 0)
  {
    long double exp = 0.0L;
    long double cur = pow(long double(m_mul), exp);
    while (cur < long double(m_min))
    {
      exp += 1.0L;
      cur = pow(long double(m_mul), exp);
    }

    while (cur <= long double(m_max))
    {
      m_values.push_back(T(cur));
      exp += 1.0L;
      cur = pow(long double(m_mul), exp);
    }
  }

private:
  double    m_mul;
  T         m_min;
  T         m_max;
};