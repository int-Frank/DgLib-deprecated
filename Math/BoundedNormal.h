#ifndef BOUNDEDNORMAL_H
#define BOUNDEDNORMAL_H

#include "utility.h"

namespace Dg
{
  class BoundedSND
  {
  public:

    BoundedSND() : m_values(nullptr), m_nValues(0) {}
    ~BoundedSND();

    BoundedSND(const BoundedSND&);
    BoundedSND& operator=(const BoundedSND&);

    Dg_Result Init(double a_mean, 
                   double a_sd, 
                   double a_lower, 
                   double a_upper, 
                   unsigned int a_count = 1024);

    bool IsGood() const { return (m_nValues != 0); }
    void Clean();
    double Get() const;

  private:

    void _init(const BoundedSND&);

  private:

    double * m_values;
    unsigned int m_nValues;

  };
}

#endif