#ifndef ATTRACTORGLOBAL_H
#define ATTRACTORGLOBAL_H

#include "particle_system/DgAttractor.h"
#include "particle_system/DgParticleData.h"
#include "DgVQS.h"

template<typename Real>
class AttractorGlobal : public Dg::Attractor<Real>
{
public:

  AttractorGlobal() : Dg::Attractor<Real>(), m_globalAccel(Dg::Vector4<Real>::ZeroVector()) {}
  ~AttractorGlobal() {}

  AttractorGlobal(AttractorGlobal<Real> const & a_other) 
    : Dg::Attractor<Real>(a_other), m_globalAccel(a_other.m_globalAccel) {}
  

  AttractorGlobal(Dg::Attractor<Real> const & a_attr)
    : Dg::Attractor<Real>(a_attr)
    , m_globalAccel(Dg::Vector4<Real>::ZeroVector())
  {}

  AttractorGlobal<Real> & operator=(AttractorGlobal<Real> const & a_other)
  {
    Dg::Attractor<Real>::operator=(a_other);
    m_globalAccel = a_other.m_globalAccel;
    return *this;
  }

  void UpdateNew(Dg::ParticleData<Real> &, int, Real) {}
  void Update(Dg::ParticleData<Real> &, int, Real);

  void SetTransformation(Dg::VQS<Real> const &);

  AttractorGlobal<Real> * Clone() const { return new AttractorGlobal<Real>(*this); }

private:
  Dg::Vector4<Real> m_globalAccel;
};

template<typename Real>
void AttractorGlobal<Real>::SetTransformation(Dg::VQS<Real> const & a_vqs)
{
  m_globalAccel = a_vqs.Rotate(Dg::Vector4<Real>::xAxis());
}

template<typename Real>
void AttractorGlobal<Real>::Update(Dg::ParticleData<Real> & a_data
									                      , int a_start
                                        , Real a_dt)
{
  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();
  Real mag(m_strength);
  if (mag * mag > m_maxAppliedAccel * m_maxAppliedAccel)
  {
    mag = (mag < static_cast<Real>(0.0)) ? -m_maxAppliedAccel : m_maxAppliedAccel;
  }

  if (pAccels)
  {
    Dg::Vector4<Real> accel = m_globalAccel * mag;
    for (int i = a_start; i < a_data.GetCountAlive(); ++i)
    {
      pAccels[i] += accel;
    }
  }
}


//template<typename Real>
//void AttractorGlobal<Real>::Update(Dg::ParticleData<Real> & a_data
//                                        , int a_start)
//{
//  Dg::Vector4<Real> * pPos = a_data.GetPosition();
//  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();
//  Real *              pTimeSiceBirth = a_data.GetTimeSinceBirth();
//
//  if (pPos && pAccels && pTimeSiceBirth)
//  {
//    Dg::Vector4<Real> accel = m_globalAccel * m_strength;
//    for (int i = a_start; i <= a_data.GetCountAlive(); ++i)
//    {
//      pAccels[i] += (accel * pTimeSiceBirth[i]);
//    }
//  }
//}
#endif