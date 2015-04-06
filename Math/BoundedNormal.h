#ifndef BOUNDEDNORMAL_H
#define BOUNDEDNORMAL_H

#include "utility.h"

namespace Dg
{
  template<typename Real>
  class BoundedSND
  {
  public:

    BoundedSND() : m_values(nullptr), m_nValues(0) {}
    ~BoundedSND();

    BoundedSND(const BoundedSND&);
    BoundedSND& operator=(const BoundedSND&);

    Dg_Result Init(Real a_mean,
                   Real a_sd, 
                   Real a_lower,
                   Real a_upper,
                   unsigned int a_nValues = 1024);

    bool IsGood() const { return (m_nValues != 0); }
    void Clean();
    Real Get() const;

  private:

    void _init(const BoundedSND&);

  private:

    Real * m_values;
    unsigned int m_nValues;

  };
}

#endif