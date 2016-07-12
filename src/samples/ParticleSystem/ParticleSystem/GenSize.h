#ifndef GENSIZE_H
#define GENSIZE_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleGenerator.h"

//! Updates particle color
template<typename Real>
class GenSize : public Dg::ParticleGenerator<Real>
{
public:
  GenSize() : Dg::ParticleGenerator<Real>()
            , m_startSize(static_cast<Real>(0.1))
            , m_dSize(static_cast<Real>(0.0)) {}

  ~GenSize() {}

  GenSize(GenSize<Real> const & a_other)
    : Dg::ParticleGenerator<Real>(a_other)
    , m_startSize(a_other.m_startSize)
    , m_dSize(a_other.m_dSize) {}

  GenSize<Real> & operator=(GenSize<Real> const & a_other)
  {
    Dg::ParticleGenerator<Real>::operator=(a_other);
    m_startSize = a_other.m_startSize;
    m_dSize = a_other.m_dSize;
    return *this;
  }

  void SetSizes(Real startSize, Real endSize);

  void Generate(Dg::ParticleData<Real> &, int, int);

  GenSize<Real> * Clone() const { return new GenSize<Real>(*this); }

private:
  Real m_startSize;
  Real m_dSize;
};

template<typename Real>
void GenSize<Real>::Generate(Dg::ParticleData<Real> & a_data, int a_start, int a_end)
{
  Real * pSize = a_data.GetSize();
  Real * pStartSize = a_data.GetStartSize();
  Real * pDSize = a_data.GetDSize();

  if (pSize)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pSize[i] = m_startSize;
    }
  }
  if (pStartSize)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pStartSize[i] = m_startSize;
    }
  }
  if (pDSize)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pDSize[i] = m_dSize;
    }
  }
}


template<typename Real>
void GenSize<Real>::SetSizes(Real a_startSize, Real a_endSize)
{
  m_startSize = a_startSize;
  m_dSize = a_endSize - a_startSize;
}

#endif