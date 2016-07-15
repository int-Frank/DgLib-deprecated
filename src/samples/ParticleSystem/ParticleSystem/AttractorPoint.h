#ifndef ATTRACTORPOINT_H
#define ATTRACTORPOINT_H

#include "particle_system/DgAttractor.h"
#include "particle_system/DgParticleData.h"
#include "DgVQS.h"

template<typename Real>
class AttractorPoint : public Dg::Attractor<Real>
{
public:

  AttractorPoint() 
    : Dg::Attractor<Real>()
    , m_point(Dg::Vector4<Real>::Origin())
  {}

  virtual ~AttractorPoint() {}

  AttractorPoint(AttractorPoint<Real> const &);

  AttractorPoint(Dg::Attractor<Real> const & a_attr)
    : Dg::Attractor<Real>(a_attr)
    , m_point(Dg::Vector4<Real>::Origin())
  {}
  
  AttractorPoint<Real> & operator=(AttractorPoint<Real> const &);

  virtual void UpdateNew(Dg::ParticleData<Real> &, int, Real) {}
  virtual void Update(Dg::ParticleData<Real> &, int, Real);

  virtual void SetTransformation(Dg::VQS<Real> const &);

  virtual AttractorPoint<Real> * Clone() const { return new AttractorPoint<Real>(*this); }

protected:
  Dg::Vector4<Real> m_point;
};

template<typename Real>
AttractorPoint<Real>::AttractorPoint(AttractorPoint<Real> const & a_other)
  : Dg::Attractor<Real>(a_other)
  , m_point(a_other.m_point)
{
}

template<typename Real>
AttractorPoint<Real> & AttractorPoint<Real>::operator=(AttractorPoint<Real> const & a_other)
{
  Dg::Attractor<Real>::operator=(a_other);
  m_point = a_other.m_point;
  return *this;
}

template<typename Real>
void AttractorPoint<Real>::SetTransformation(Dg::VQS<Real> const & a_vqs)
{
  m_point = a_vqs.Translate(Dg::Vector4<Real>::Origin());
}

template<typename Real>
void AttractorPoint<Real>::Update(Dg::ParticleData<Real> & a_data
									                     , int a_start
                                       , Real a_dt)
{
  Dg::Vector4<Real> * pPos = a_data.GetPosition();
  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();

  if (pPos && pAccels)
  {
    switch (GetAccelType())
    {
    case Constant:
    {
      for (int i = a_start; i < a_data.GetCountAlive(); ++i)
      {
        pAccels[i] += (GetAccel_Constant(m_point, pPos[i]));
      }
      break;
    }
    case Linear:
    {
      for (int i = a_start; i < a_data.GetCountAlive(); ++i)
      {
        pAccels[i] += (GetAccel_Linear(m_point, pPos[i]));
      }
      break;
    }
    case InvSq:
    {
      for (int i = a_start; i < a_data.GetCountAlive(); ++i)
      {
        pAccels[i] += (GetAccel_InvSq(m_point, pPos[i]));
      }
      break;
    }
    }
  }
}
#endif