#pragma once

#include "TableGenerator.h"

template<typename T>
class TG_inverf : public TableGenerator<T>
{
public:

  void Generate(unsigned a_nValues)
  {
    m_values.clear();
    T * c = new T[a_nValues]();
    c[0] = (T)1.0;
    for (unsigned k = 1; k < a_nValues; ++k)
    {
      for (unsigned m = 0; m <= k - 1; m++)
      {
        c[k] += (c[m] * c[k - 1 - m]) / ((m + 1) * (2 * m + 1));
      }
    }

    T a = (T)0.88622692545275801364908374167057; //sqrt(pi) / 2
    T a_sq = a * a;
    T b = (T)1.0; //2k + 1

    for (unsigned k = 0; k < a_nValues; ++k)
    {
      m_values.push_back((c[k] * a / b));

      //Prepare values for next iteration
      a *= a_sq;
      b += (T)2.0;
    }
    delete[] c;
  }
};