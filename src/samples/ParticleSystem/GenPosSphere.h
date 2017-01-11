#ifndef GENPOSSPHERE_H
#define GENPOSSPHERE_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleGenerator.h"
#include "DgR3Sphere.h"

//! Updates particle color
template<typename Real>
class GenPosSphere : public Dg::ParticleGenerator<Real>
{
public:
  GenPosSphere() 
    : Dg::ParticleGenerator<Real>() 
  {}

  ~GenPosSphere() {}

  GenPosSphere(GenPosSphere<Real> const & a_other)
    : Dg::ParticleGenerator<Real>(a_other)
    , m_sphere(a_other.m_sphere)
  {}

  GenPosSphere<Real> & operator=(GenPosSphere<Real> const & a_other)
  {
    Dg::ParticleGenerator<Real>::operator=(a_other);
    m_sphere = a_other.m_sphere;
    return *this;
  }

  void SetTransformation(Dg::R3::VQS<Real> const &);
  void SetOrigin(Dg::R3::Vector<Real> const &);
  void SetRadius(Real);

  void Generate(Dg::ParticleData<Real> &, int, int);

  GenPosSphere<Real> * Clone() const { return new GenPosSphere<Real>(*this); }

private:
  Dg::R3::Sphere<Real>    m_sphere;
};

template<typename Real>
void GenPosSphere<Real>::SetTransformation(Dg::R3::VQS<Real> const & a_vqs)
{
  m_sphere.Set(Dg::R3::Vector<Real>::Origin(), static_cast<Real>(1.0));
  m_sphere.TransformSelf(a_vqs);
}

template<typename Real>
void GenPosSphere<Real>::SetOrigin(Dg::R3::Vector<Real> const & a_origin)
{
  m_sphere.SetCenter(a_origin);
}

template<typename Real>
void GenPosSphere<Real>::SetRadius(Real a_radius)
{
  m_sphere.SetRadius(a_radius);
}

template<typename Real>
void GenPosSphere<Real>::Generate(Dg::ParticleData<Real> & a_data, int a_start, int a_end)
{
  Dg::R3::Vector<Real> * pPos = a_data.GetPosition();

  if (pPos)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pPos[i] = m_sphere.GetRandomPointInside();
    }
  }
}

#endif