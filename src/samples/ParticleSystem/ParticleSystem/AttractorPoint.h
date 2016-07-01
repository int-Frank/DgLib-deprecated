#ifndef ATTRACTORPOINT_H
#define ATTRACTORPOINT_H

#include "particle_system/DgAttractor.h"
#include "particle_system/DgParticleData.h"
#include "DgVQS.h"

template<typename Real, unsigned Force>
class AttractorPoint : public Dg::Attractor<Real, Force>
{
public:

  AttractorPoint() : Dg::Attractor<Real, Force>(), m_point(Dg::Vector4<Real>::Origin()) {}
  ~AttractorPoint() {}

  AttractorPoint(AttractorPoint<Real, Force> const & a_other) 
    : Dg::Attractor<Real, Force>(a_other), m_point(a_other.m_point) {}
  
  AttractorPoint<Real, Force> & operator=(AttractorPoint<Real, Force> const & a_other)
  {
    Dg::Attractor<Real, Force>::operator=(a_other);
    m_point = a_other.m_point;
    return *this;
  }

  void Update(int start,
              int finish,
              Real dt,
              Dg::ParticleData<Real> & data);

  void SetTransformation(Dg::VQS<Real> const &);

  AttractorPoint<Real, Force> * Clone() const { return new AttractorPoint<Real, Force>(*this); }

private:
  Dg::Vector4<Real> m_point;
};

template<typename Real, unsigned Force>
void AttractorPoint<Real, Force>::SetTransformation(Dg::VQS<Real> const & a_vqs)
{
  m_point = a_vqs.Translate(Dg::Vector4<Real>::Origin());
}

template<typename Real, unsigned Force>
void AttractorPoint<Real, Force>::Update(int a_start,
                                         int a_finish,
                                         Real a_dt,
                                         Dg::ParticleData<Real> & a_data)
{
  Dg::Vector4<Real> * pPos = a_data.GetPosition();
  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();
  if (pPos && pAccels)
  {
    for (int i = a_start; i <= a_finish; ++i)
    {
      pAccels[i] += (_GetAccelBetweenPoints(pPos[i], m_point) * a_dt);
    }
  }
}

#endif