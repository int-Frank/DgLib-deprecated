#ifndef GENVELOUTWARDS_H
#define GENVELOUTWARDS_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleGenerator.h"
#include "DgMath.h"

//! Updates particle color
template<typename Real>
class GenVelOutwards : public Dg::ParticleGenerator<Real>
{
public:
  GenVelOutwards() 
    : Dg::ParticleGenerator<Real>()
    , m_origin(Dg::Vector4<Real>::yAxis())
    , m_velocity(static_cast<Real>(1.0))
  {}

  ~GenVelOutwards() {}

  GenVelOutwards(GenVelOutwards<Real> const & a_other)
    : Dg::ParticleGenerator<Real>(a_other)
    , m_origin(a_other.m_origin)
    , m_velocity(a_other.m_velocity) 
  {}

  GenVelOutwards<Real> & operator=(GenVelOutwards<Real> const & a_other)
  {
    Dg::ParticleGenerator<Real>::operator=(a_other);
    m_origin = a_other.m_origin;
    m_velocity = a_other.m_velocity;
    return *this;
  }

  void SetTransformation(Dg::VQS<Real> const &);
  void SetAngle(Real);
  void SetVelocity(Real a_vel) { m_velocity = a_vel; }

  void Generate(Dg::ParticleData<Real> &, int, int);

  GenVelOutwards<Real> * Clone() const { return new GenVelOutwards<Real>(*this); }

private:
  Dg::Vector4<Real> m_origin;
  Real              m_velocity;
};

template<typename Real>
void GenVelOutwards<Real>::SetTransformation(Dg::VQS<Real> const & a_vqs)
{
  m_origin = a_vqs.Rotate(Dg::Vector4<Real>::xAxis());
}


template<typename Real>
void GenVelOutwards<Real>::SetAngle(Real a_ang)
{
  m_angle = (a_ang > static_cast<Real>(Dg::PI)) ? static_cast<Real>(Dg::PI) : a_ang;
}

template<typename Real>
void GenVelOutwards<Real>::Generate(Dg::ParticleData<Real> & a_data, int a_start, int a_end)
{
  Dg::Vector4<Real> * pVels = a_data.GetVelocity();
  Dg::Vector4<Real> * pAccel = a_data.GetAcceleration();

  if (pVels)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pVels[i] = m_velocity * Dg::GetRandomVector(m_origin, m_angle);
    }
  }
  if (pAccel)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pAccel[i].Zero();
    }
  }
}

#endif