#ifndef GENRELATIVEFORCE_H
#define GENRELATIVEFORCE_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleGenerator.h"

//! Updates particle color
template<typename Real>
class GenRelativeForce : public Dg::ParticleGenerator<Real>
{
public:
  GenRelativeForce() 
    : m_val(static_cast<Real>(1.0))
    , Dg::ParticleGenerator<Real>() {}

  ~GenRelativeForce() {}

  GenRelativeForce(GenRelativeForce<Real> const & a_other)
    : Dg::ParticleGenerator<Real>(a_other)
    , m_val(a_other.m_val) {}

  GenRelativeForce<Real> & operator=(GenRelativeForce<Real> const & a_other)
  {
    Dg::ParticleGenerator<Real>::operator=(a_other);
    m_val = a_other.m_val;
    return *this;
  }

  void SetTransformation(Dg::VQS<Real> const &);

  //! Must be >= 0
  void SetValue(Real);

  void Generate(Dg::ParticleData<Real> &, int, int);

  GenRelativeForce<Real> * Clone() const { return new GenRelativeForce<Real>(*this); }

private:
  Real m_val;
};

template<typename Real>
void GenRelativeForce<Real>::SetTransformation(Dg::VQS<Real> const & a_vqs)
{
}

template<typename Real>
void GenRelativeForce<Real>::SetValue(Real a_val)
{
  m_val = (a_val > static_cast<Real>(0.0)) ? a_val : static_cast<Real>(0.0);
}

template<typename Real>
void GenRelativeForce<Real>::Generate(Dg::ParticleData<Real> & a_data, int a_start, int a_end)
{
  Real * pForce = a_data.GetForce();

  if (pForce)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pForce[i] = m_val;
    }
  }
}

#endif