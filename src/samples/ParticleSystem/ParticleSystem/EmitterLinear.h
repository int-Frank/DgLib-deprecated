#ifndef EMITTERLINEAR_H
#define EMITTERLINEAR_H

#include <math.h>

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleEmitter.h"

//! Updates position
template<typename Real>
class EmitterLinear : public Dg::ParticleEmitter<Real>
{
public:
  EmitterLinear() : Dg::ParticleEmitter<Real>(), 
                    m_rate(static_cast<Real>(1.0)),
                    m_residual(static_cast<Real>(0.0)){}
  ~EmitterLinear() {}

  EmitterLinear(EmitterLinear<Real> const & a_other) 
    : Dg::ParticleEmitter<Real>(a_other),
      m_rate(a_other.m_rate),
      m_residual(a_other.m_residual) {}

  EmitterLinear<Real> & operator=(EmitterLinear<Real> const & a_other)
  {
    Dg::ParticleEmitter<Real>::operator=(a_other);
    m_rate = a_other.m_rate;
    m_residual = a_other.m_residual;
    return *this;
  }

  void SetRate(Real a_rate) { if (a_rate >= static_cast<Real>(0.0)) { m_rate = a_rate; } }

  int EmitParticles(Real dt, Dg::ParticleData<Real> & data);

  EmitterLinear<Real> * Clone() const { return new EmitterLinear<Real>(*this); }

private:
  Real m_rate;
  Real m_residual;
};

template<typename Real>
int EmitterLinear<Real>::EmitParticles(Real a_dt, Dg::ParticleData<Real> & a_data)
{
  if (Dg::IsZero(m_rate) || !IsOn())
  {
    return 0;
  }

  //Get time since last particle emitted
  Real totalTime = a_dt + m_residual;
  Real _nPar = m_rate * totalTime;
  Real nPar = std::floor(_nPar);
  int nParInt = static_cast<int>(nPar);
  Real tSpacing = static_cast<Real>(1.0) / m_rate;
  m_residual = totalTime - nPar * tSpacing;

  Real * pTimeSinceEmit = a_data.GetTimeSinceEmit();

  if (pTimeSinceEmit)
  {
    while (nPar != static_cast<Real>(0.0) &&
           a_data.GetCountAlive() <= a_data.GetCountMax())
    {
      int parInd = a_data.Wake();
      pTimeSinceEmit[parInd] = totalTime - nPar * tSpacing;
      nPar -= static_cast<Real>(1.0);
    }
  }
  else
  {
    while (nPar != static_cast<Real>(0.0) &&
      a_data.GetCountAlive() <= a_data.GetCountMax())
    {
      int parInd = a_data.Wake();
      nPar -= static_cast<Real>(1.0);
    }
  }

  return nParInt;
}

#endif