#ifndef GENVELCONE_H
#define GENVELCONE_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleGenerator.h"
#include "DgMath.h"

template<typename Real>
class GenVelCone : public Dg::ParticleGenerator<Real>
{
public:
  GenVelCone() : Dg::ParticleGenerator<Real>()
               , m_axis(Dg::R3::Vector<Real>::yAxis())
               , m_angle(static_cast<Real>(0.785))
               , m_velocity(static_cast<Real>(1.0))
                  {}
  ~GenVelCone() {}

  GenVelCone(GenVelCone<Real> const & a_other) 
    : Dg::ParticleGenerator<Real>(a_other)
    , m_axis(a_other.m_axis)
    , m_angle(a_other.m_angle)
    , m_velocity(a_other.m_velocity) {}

  GenVelCone<Real> & operator=(GenVelCone<Real> const & a_other)
  {
    Dg::ParticleGenerator<Real>::operator=(a_other);
    m_axis = a_other.m_axis;
    m_angle = a_other.m_angle;
    m_velocity = a_other.m_velocity;
    return *this;
  }

  void SetTransformation(Dg::R3::VQS<Real> const &);
  void SetAngle(Real);
  void SetVelocity(Real a_vel) { m_velocity = a_vel; }

  void Generate(Dg::ParticleData<Real> &, int, int);

  GenVelCone<Real> * Clone() const { return new GenVelCone<Real>(*this); }

private:
  Dg::R3::Vector<Real> m_axis;
  Real              m_angle;
  Real              m_velocity;
};

template<typename Real>
void GenVelCone<Real>::SetTransformation(Dg::R3::VQS<Real> const & a_vqs)
{
  m_axis = a_vqs.Rotate(Dg::R3::Vector<Real>::xAxis());
}


template<typename Real>
void GenVelCone<Real>::SetAngle(Real a_ang)
{
  m_angle = (a_ang > Dg::Constants<Real>::PI) ? Dg::Constants<Real>::PI : a_ang;
}

template<typename Real>
void GenVelCone<Real>::Generate(Dg::ParticleData<Real> & a_data, int a_start, int a_end)
{
  Dg::R3::Vector<Real> * pVels = a_data.GetVelocity();
  Dg::R3::Vector<Real> * pAccel = a_data.GetAcceleration();

  if (pVels)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      Dg::R3::Vector<float> vec = Dg::R3::GetRandomVector(m_axis, m_angle);
      pVels[i] = m_velocity * vec;
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