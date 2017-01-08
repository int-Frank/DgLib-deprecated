#ifndef GENPOSPOINT_H
#define GENPOSPOINT_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleGenerator.h"

//! Updates particle color
template<typename Real>
class GenPosPoint : public Dg::ParticleGenerator<Real>
{
public:
  GenPosPoint() : Dg::ParticleGenerator<Real>() 
                , m_origin(Dg::R3::Vector4<Real>::Origin())
  {}

  ~GenPosPoint() {}

  GenPosPoint(GenPosPoint<Real> const & a_other)
    : Dg::ParticleGenerator<Real>(a_other)
    , m_origin(a_other.m_origin){}

  GenPosPoint<Real> & operator=(GenPosPoint<Real> const & a_other)
  {
    Dg::ParticleGenerator<Real>::operator=(a_other);
    m_origin = a_other.m_origin;
    return *this;
  }

  void SetTransformation(Dg::R3::VQS<Real> const &);
  void SetOrigin(Dg::R3::Vector4<Real> const &);

  void Generate(Dg::ParticleData<Real> &, int, int);

  GenPosPoint<Real> * Clone() const { return new GenPosPoint<Real>(*this); }

private:
  Dg::R3::Vector4<Real> m_origin;
};

template<typename Real>
void GenPosPoint<Real>::SetTransformation(Dg::R3::VQS<Real> const & a_vqs)
{
  m_origin = a_vqs.Translate(Dg::R3::Vector4<Real>::Origin());
}

template<typename Real>
void GenPosPoint<Real>::SetOrigin(Dg::R3::Vector4<Real> const & a_pos)
{
  m_origin = a_pos;
  m_origin.w() = static_cast<Real>(1.0);
}

template<typename Real>
void GenPosPoint<Real>::Generate(Dg::ParticleData<Real> & a_data, int a_start, int a_end)
{
  Dg::R3::Vector4<Real> * pPos = a_data.GetPosition();

  if (pPos)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pPos[i] = m_origin;
    }
  }
}

#endif