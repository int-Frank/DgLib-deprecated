#pragma once

#include "TableGenerator.h"
#include "DgMath.h"

template<typename T>
class TG_HashTableBucketCounts : public TableGenerator<T>
{
public:

  TG_HashTableBucketCounts(T a_minCount, T a_maxCount, double a_mul)
    : m_minCount(a_minCount)
    , m_mul(a_mul)
    , m_maxCount(a_maxCount)
  {}

  void Generate(unsigned a_nValues = 0)
  {
    T minCount = m_minCount;
    while (!Dg::IsPrime(minCount))
    {
      ++minCount;
    }

    long double curExp = static_cast<long double>(m_mul);
    long double maxCount = static_cast<long double>(m_maxCount);

    long double exp = 1.0L;
    while (pow(m_mul, exp) < long double(minCount))
    {
      exp += 1.0L;
    }

    long double cur = pow(m_mul, exp);
    while (cur <= maxCount)
    {
      T val = T(cur);
      std::cout << std::dec << "Current target: " << val << ": ";
      while (!Dg::IsPrime(val))
      {
        --val;
      }

      m_values.push_back(val);
      std::cout << std::dec << val << '\n';

      exp += 1.0L;
      cur = pow(m_mul, exp);
    }
  }

private:
  T               m_minCount;
  long double     m_mul;
  T               m_maxCount;
};