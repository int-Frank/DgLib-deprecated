#ifndef EMITTERLINEAR_H
#define EMITTERLINEAR_H

#include <cmath>

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleEmitter.h"


template<typename Real>
class EmitterLinear : public Dg::ParticleEmitter<Real>
{
public:
  EmitterLinear() 
    : Dg::ParticleEmitter<Real>()
    , m_rate(static_cast<Real>(1.0))
    , m_residual(static_cast<Real>(0.0))
  {}
  ~EmitterLinear() {}

  EmitterLinear(EmitterLinear<Real> const & a_other) 
    : Dg::ParticleEmitter<Real>(a_other)
    , m_rate(a_other.m_rate)
    , m_residual(a_other.m_residual)
  {}

  EmitterLinear(Dg::ParticleEmitter<Real> const & a_other) 
    : Dg::ParticleEmitter<Real>(a_other)
    , m_rate(static_cast<Real>(1.0))
    , m_residual(static_cast<Real>(0.0)) 
  {}

  EmitterLinear<Real> & operator=(EmitterLinear<Real> const & a_other)
  {
    Dg::ParticleEmitter<Real>::operator=(a_other);
    m_rate = a_other.m_rate;
    m_residual = a_other.m_residual;
    return *this;
  }

  void SetRate(Real a_rate) { if (a_rate >= static_cast<Real>(0.0)) { m_rate = a_rate; } }
  int EmitParticles(Dg::ParticleData<Real> &, Real);

  EmitterLinear<Real> * Clone() const { return new EmitterLinear<Real>(*this); }

private:
  Real m_rate;
  Real m_residual;
};

template<typename Real>
int EmitterLinear<Real>::EmitParticles(Dg::ParticleData<Real> & a_data, Real a_dt)
{
  if (Dg::IsZero(m_rate) || !IsOn())
  {
    return 0;
  }

  //Get time since last particle emitted
  Real totalTime = a_dt + m_residual;
  Real _nPar = m_rate * totalTime;
  Real nPar = std::floor(_nPar);
  Real tSpacing = static_cast<Real>(1.0) / m_rate;
  m_residual = totalTime - nPar * tSpacing;

  Real * pTimeSinceBirth = a_data.GetTimeSinceBirth();
  int parInd = 0;
  int nNewParticles = 0;
  int startIndex = a_data.GetCountAlive();

  if (pTimeSinceBirth)
  {
    while (nPar != static_cast<Real>(0.0) &&
           a_data.Wake(parInd))
    {
      nNewParticles++;
      pTimeSinceBirth[parInd] = totalTime - nPar * tSpacing;
      nPar -= static_cast<Real>(1.0);
    }
  }
  else
  {
    while (nPar != static_cast<Real>(0.0) &&
           a_data.Wake(parInd))
    {
      nNewParticles++;
      nPar -= static_cast<Real>(1.0);
    }
  }

  //Generate new particles
  for (int g = 0; g < m_generators.size(); ++g)
  {
    m_generators[g]->Generate(a_data, startIndex, startIndex + nNewParticles - 1);
  }

  return nNewParticles;
}

#endif