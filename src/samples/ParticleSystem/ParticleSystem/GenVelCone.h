#ifndef GENVELCONE_H
#define GENVELCONE_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleGenerator.h"
#include "DgMath.h"

//! Updates particle color
template<typename Real>
class GenVelCone : public Dg::ParticleGenerator<Real>
{
  GenVelCone() : m_axis(Dg::Vector4<Real>::xAxis()), 
                 m_angle(static_cast<Real>(0.785)),
                 Dg::ParticleGenerator<Real>() {}
  ~GenVelCone() {}

  GenVelCone(GenVelCone<Real> const & a_other) 
    : Dg::ParticleGenerator<Real>(a_other),
      m_axis(a_other.m_axis),
      m_angle(a_other.m_angle) {}

  GenVelCone<Real> & operator=(GenVelCone<Real> const & a_other)
  {
    Dg::ParticleGenerator<Real>::operator=(a_other);
    m_axis = a_other.m_axis;
    m_angle = a_other.m_angle;
    return *this;
  }

  void SetTransformation(Dg::VQS<Real> const &);
  void SetAngle(Real);

  void Generate(int, int, Dg::ParticleData<Real> &);

  GenVelCone<Real> * Clone() const { return new GenVelCone<Real>(*this); }

private:
  Dg::Vector4<Real> m_axis;
  Real              m_angle;
};

template<typename Real>
void GenVelCone<Real>::SetTransformation(Dg::VQS<Real> const & a_vqs)
{
  m_axis = a_vqs.Rotate(Dg::Vector4<Real>::xAxis());
}


template<typename Real>
void GenVelCone<Real>::SetAngle(Real a_ang)
{
  m_angle = (a_ang > static_cast<Real>(Dg::PI)) ? static_cast<Real>(Dg::PI) : a_ang;
}

template<typename Real>
void GenVelCone<Real>::Generate(int a_start, int a_end, Dg::ParticleData<Real> & a_data)
{
  Dg::Vector4<Real> * pVels = a_data.GetVelocity();

  if (pVels)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pVels[i] = Dg::GetRandomVector(m_axis, m_angle);
    }
  }
}

#endif