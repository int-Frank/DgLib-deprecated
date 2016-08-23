#pragma once

#include "TableGenerator.h"

template<typename T>
class TG_erf : public TableGenerator<T>
{
public:

  void Generate(unsigned a_nValues)
  {
    m_values.clear();

    double sign = 1.0;
    double val = 1.0;
    double factorial = 1.0;
    double twoPowN = 1.0;
    double invSqrt2Pi = 0.39894228040143267793994605993438;

    for (unsigned i = 0; i < a_nValues; ++i)
    {
      m_values.push_back((sign * val * factorial * twoPowN * invSqrt2Pi));

      //Prepare values for next iteration.
      sign *= -1.0;
      val += 2.0;
      factorial *= (static_cast<double>(i) + 1.0);
      twoPowN *= 2;
    }
  }
};