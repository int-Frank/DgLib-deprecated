#ifndef ATTRACTORGLOBAL_H
#define ATTRACTORGLOBAL_H

#include "particle_system/DgAttractor.h"
#include "particle_system/DgParticleData.h"
#include "DgR3VQS.h"

template<typename Real>
class AttractorGlobal : public Dg::Attractor<Real>
{
public:

  AttractorGlobal() : Dg::Attractor<Real>(), m_globalAccel(Dg::R3::Vector4<Real>::ZeroVector()) {}
  ~AttractorGlobal() {}

  AttractorGlobal(AttractorGlobal<Real> const & a_other) 
    : Dg::Attractor<Real>(a_other), m_globalAccel(a_other.m_globalAccel) {}
  

  AttractorGlobal(Dg::Attractor<Real> const & a_attr)
    : Dg::Attractor<Real>(a_attr)
    , m_globalAccel(Dg::R3::Vector4<Real>::ZeroVector())
  {}

  AttractorGlobal<Real> & operator=(AttractorGlobal<Real> const & a_other)
  {
    Dg::Attractor<Real>::operator=(a_other);
    m_globalAccel = a_other.m_globalAccel;
    return *this;
  }

  //TODO Properly implement UpdateNew
  void UpdateNew(Dg::ParticleData<Real> &, int, Real) {}
  void Update(Dg::ParticleData<Real> &, int, Real);

  void SetTransformation(Dg::R3::VQS<Real> const &);

  AttractorGlobal<Real> * Clone() const { return new AttractorGlobal<Real>(*this); }

private:
  Dg::R3::Vector4<Real> m_globalAccel;
};

template<typename Real>
void AttractorGlobal<Real>::SetTransformation(Dg::R3::VQS<Real> const & a_vqs)
{
  m_globalAccel = a_vqs.Rotate(Dg::R3::Vector4<Real>::xAxis());
}

template<typename Real>
void AttractorGlobal<Real>::Update(Dg::ParticleData<Real> & a_data
									                      , int a_start
                                        , Real a_dt)
{
  Dg::R3::Vector4<Real> * pAccels = a_data.GetAcceleration();
  Real mag(m_strength);
  if (mag * mag > m_maxAppliedAccel * m_maxAppliedAccel)
  {
    mag = (mag < static_cast<Real>(0.0)) ? -m_maxAppliedAccel : m_maxAppliedAccel;
  }

  if (pAccels)
  {
    Dg::R3::Vector4<Real> accel = m_globalAccel * mag;
    for (int i = a_start; i < a_data.GetCountAlive(); ++i)
    {
      pAccels[i] += accel;
    }
  }
}

#endif