#ifndef ATTRACTORGLOBAL_H
#define ATTRACTORGLOBAL_H

#include "particle_system/DgAttractor.h"
#include "particle_system/DgParticleData.h"
#include "DgVQS.h"

template<typename Real, unsigned Force>
class AttractorGlobal : public Dg::Attractor<Real, Force>
{
public:

  AttractorGlobal() : Dg::Attractor<Real, Force>(), m_globalAccel(Dg::Vector4<Real>::Origin()) {}
  ~AttractorGlobal() {}

  AttractorGlobal(AttractorGlobal<Real, Force> const & a_other) 
    : Dg::Attractor<Real, Force>(a_other), m_globalAccel(a_other.m_globalAccel) {}
  AttractorGlobal<Real, Force> & operator=(AttractorGlobal<Real, Force> const & a_other)
  {
    Dg::Attractor<Real, Force>::operator=(a_other);
    m_globalAccel = a_other.m_globalAccel;
    return *this;
  }

  void Update(Dg::ParticleData<Real> &, int);
  void Update(Dg::ParticleData<Real> &, int, Real);

  void SetStrength(Real a_str) { if (a_str <= m_maxAccelMag) {m_strength = a_str; }}

  void SetTransformation(Dg::VQS<Real> const &);

  AttractorGlobal<Real, Force> * Clone() const { return new AttractorGlobal<Real, Force>(*this); }

private:
  Dg::Vector4<Real> m_globalAccel;
};

template<typename Real, unsigned Force>
void AttractorGlobal<Real, Force>::SetTransformation(Dg::VQS<Real> const & a_vqs)
{
  m_globalAccel = a_vqs.Translate(Dg::Vector4<Real>::xAxis());
}

template<typename Real, unsigned Force>
void AttractorGlobal<Real, Force>::Update(Dg::ParticleData<Real> & a_data
									                      , int a_start
                                        , Real a_dt)
{
  Dg::Vector4<Real> * pPos = a_data.GetPosition();
  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();

  if (pPos && pAccels)
  {
    Dg::Vector4<Real> accel = m_globalAccel * m_strength;
    for (int i = a_start; i < a_data.GetCountAlive(); ++i)
    {
      pAccels[i] += (accel * a_dt);
    }
  }
}


template<typename Real, unsigned Force>
void AttractorGlobal<Real, Force>::Update(Dg::ParticleData<Real> & a_data
                                        , int a_start)
{
  Dg::Vector4<Real> * pPos = a_data.GetPosition();
  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();
  Real *              pTimeSiceBirth = a_data.GetTimeSinceBirth();

  if (pPos && pAccels && pTimeSiceBirth)
  {
    Dg::Vector4<Real> accel = m_globalAccel * m_strength;
    for (int i = a_start; i <= a_data.GetCountAlive(); ++i)
    {
      pAccels[i] += (accel * pTimeSiceBirth[i]);
    }
  }
}
#endif