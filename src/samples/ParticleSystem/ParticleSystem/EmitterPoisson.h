#ifndef EMITTERPOISSON_H
#define EMITTERPOISSON_H

#include <math.h>

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleEmitter.h"
#include "DgBoundedSND.h"

//! Updates position
template<typename Real>
class EmitterPoisson : public Dg::ParticleEmitter<Real>
{
public:
  EmitterPoisson() : Dg::ParticleEmitter<Real>(),
    m_rate(static_cast<Real>(1.0)),
    m_residual(static_cast<Real>(0.0)) {}
  ~EmitterPoisson() {}

  EmitterPoisson(EmitterPoisson<Real> const & a_other)
    : Dg::ParticleEmitter<Real>(a_other),
    m_rate(a_other.m_rate),
    m_residual(a_other.m_residual) {}

  EmitterPoisson<Real> & operator=(EmitterPoisson<Real> const & a_other)
  {
    Dg::ParticleEmitter<Real>::operator=(a_other);
    m_rate = a_other.m_rate;
    m_residual = a_other.m_residual;
    return *this;
  }

  void SetRate(Real);

  int EmitParticles(Real dt, Dg::ParticleData<Real> & data);

  EmitterPoisson<Real> * Clone() const { return new EmitterPoisson<Real>(*this); }

private:
  Real                  m_residual;
  Real                  m_nextEmitTime;
  Dg::BoundedSND<Real>  m_poissonDist;
};


template<typename Real>
void EmitterPoisson<Real>::SetRate(Real a_val)
{
  if (a_val < static_cast<Real>(0.0))
  {
    a_val = static_cast<Real>(1.0);
  }

  Real mean = satic_cast<Real>(1.0) / a_val;
  Real sd = std::sqrt(mean);
  Real lower = mean - static_cast<Real>(3.0) * sd;
  if (lower < static_cast<Real>(0.0))
  {
    lower = static_cast<Real>(0.0);
  }
  Real upper = mean + static_cast<Real>(3.0) * sd;
  m_poissonDist.Init(mean, sd, lower, upper, 1024);
  
}


template<typename Real>
int EmitterPoisson<Real>::EmitParticles(Real a_dt, Dg::ParticleData<Real> & a_data)
{
  Real totalTime = a_dt + m_residual;
  

  return nParInt;
}

#endif