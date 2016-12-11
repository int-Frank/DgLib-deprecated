#ifndef EMITTERRANDOM_H
#define EMITTERRANDOM_H

#include <cmath>

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleEmitter.h"
#include "DgRNG.h"

//! Updates position
template<typename Real>
class EmitterRandom : public Dg::ParticleEmitter<Real>
{
public:
  EmitterRandom() 
    : Dg::ParticleEmitter<Real>()
    , m_mean(static_cast<Real>(1.0))
    , m_nextEmitTime(static_cast<Real>(0.0)){}
  
  ~EmitterRandom() {}

  EmitterRandom(Dg::ParticleEmitter<Real> const & a_other)
    : Dg::ParticleEmitter<Real>(a_other)
    , m_mean(static_cast<Real>(1.0))
    , m_nextEmitTime(static_cast<Real>(0.0)) {}

  EmitterRandom(EmitterRandom<Real> const & a_other)
    : Dg::ParticleEmitter<Real>(a_other)
    , m_mean(a_other.m_mean)
    , m_nextEmitTime(a_other.m_nextEmitTime){}

  EmitterRandom<Real> & operator=(EmitterRandom<Real> const & a_other)
  {
    Dg::ParticleEmitter<Real>::operator=(a_other);
    m_mean = a_other.mean;
    m_nextEmitTime = a_other.m_nextEmitTime;
    return *this;
  }

  void SetRate(Real a_rate);

  int EmitParticles(Dg::ParticleData<Real> &, Real);

  EmitterRandom<Real> * Clone() const { return new EmitterRandom<Real>(*this); }

private:
  Real                  m_mean;
  Real                  m_nextEmitTime;
};


template<typename Real>
void EmitterRandom<Real>::SetRate(Real a_val)
{
  Real maxMean = static_cast<Real>(1.0) / Dg::Constants<Real>::EPSILON;
  m_mean = (Dg::IsZero(a_val)) ? maxMean : static_cast<Real>(1.0) / a_val;
  m_nextEmitTime = static_cast<Real>(0.0);
}

template<typename Real>
int EmitterRandom<Real>::EmitParticles(Dg::ParticleData<Real> & a_data, Real a_dt)
{
  int nNewParticles = 0;
  int startIndex = a_data.GetCountAlive();
  Dg::RNG rng;

  Real * pTimeSinceBirth = a_data.GetTimeSinceBirth();
  while (a_dt >= m_nextEmitTime)
  {
    int parInd(0);
    if (!a_data.Wake(parInd))
    {
      break;
    }
    
    nNewParticles++;

    if (pTimeSinceBirth)
    {
      pTimeSinceBirth[parInd] = a_dt - m_nextEmitTime;
    }
    
    Real rnd = rng.GetUniform(static_cast<Real>(0.0), static_cast<Real>(2.0));
    
    //This function gives us a Gaussian-like distribution of samples, ranging from
    //0 to 2*m_mean.
    Real timeToNext = m_mean * acos(rnd - static_cast<Real>(1.0)) / static_cast<Real>(Dg::Constants<double>::PI * 0.5);
    m_nextEmitTime += timeToNext;
  }
  m_nextEmitTime -= a_dt;

  //Generate new particles
  for (int g = 0; g < m_generators.size(); ++g)
  {
    m_generators[g]->Generate(a_data, startIndex, startIndex + nNewParticles - 1);
  }

  return nNewParticles;
}

#endif