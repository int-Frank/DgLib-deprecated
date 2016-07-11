#ifndef ATTRACTORPOINT_H
#define ATTRACTORPOINT_H

#include "particle_system/DgAttractor.h"
#include "particle_system/DgParticleData.h"
#include "DgVQS.h"

template<typename Real, unsigned Force>
class AttractorPoint : public Dg::Attractor<Real, Force>
{
public:

  AttractorPoint() 
    : Dg::Attractor<Real, Force>()
    , m_point(Dg::Vector4<Real>::Origin())
  {}

  virtual ~AttractorPoint() {}

  AttractorPoint(AttractorPoint<Real, Force> const &);
  
  AttractorPoint<Real, Force> & operator=(AttractorPoint<Real, Force> const &);

  virtual void UpdateNew(Dg::ParticleData<Real> &, int, Real) {}
  virtual void Update(Dg::ParticleData<Real> &, int, Real);

  virtual void SetTransformation(Dg::VQS<Real> const &);

  virtual AttractorPoint<Real, Force> * Clone() const { return new AttractorPoint<Real, Force>(*this); }

protected:
  Dg::Vector4<Real> m_point;
};

template<typename Real, unsigned Force>
AttractorPoint<Real, Force>::AttractorPoint(AttractorPoint<Real, Force> const & a_other)
  : Dg::Attractor<Real, Force>(a_other)
  , m_point(a_other.m_point)
{
}

template<typename Real, unsigned Force>
AttractorPoint<Real, Force> & AttractorPoint<Real, Force>::operator=(AttractorPoint<Real, Force> const & a_other)
{
  Dg::Attractor<Real, Force>::operator=(a_other);
  m_point = a_other.m_point;
  return *this;
}

template<typename Real, unsigned Force>
void AttractorPoint<Real, Force>::SetTransformation(Dg::VQS<Real> const & a_vqs)
{
  m_point = a_vqs.Translate(Dg::Vector4<Real>::Origin());
}

template<typename Real, unsigned Force>
void AttractorPoint<Real, Force>::Update(Dg::ParticleData<Real> & a_data
									                     , int a_start
                                       , Real a_dt)
{
  Dg::Vector4<Real> * pPos = a_data.GetPosition();
  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();

  if (pPos && pAccels)
  {
    for (int i = a_start; i < a_data.GetCountAlive(); ++i)
    {
      pAccels[i] += (GetAccelBetweenPoints(m_point, pPos[i]));
    }
  }
}
#endif