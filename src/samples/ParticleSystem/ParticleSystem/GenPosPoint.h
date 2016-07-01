#ifndef GENPOSPOINT_H
#define GENPOSPOINT_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleGenerator.h"

//! Updates particle color
template<typename Real>
class GenPosPoint : public Dg::ParticleGenerator<Real>
{
  GenPosPoint() : m_origin(Dg::Vector4<Real>::Origin()),
                  Dg::ParticleGenerator<Real>() {}

  ~GenPosPoint() {}

  GenPosPoint(GenPosPoint<Real> const & a_other)
    : Dg::ParticleGenerator<Real>(a_other),
      m_origin(a_other.m_origin){}

  GenPosPoint<Real> & operator=(GenPosPoint<Real> const & a_other)
  {
    Dg::ParticleGenerator<Real>::operator=(a_other);
    origin = a_other.origin;
    return *this;
  }

  void SetTransformation(Dg::VQS<Real> const &);

  void Generate(int, int, Dg::ParticleData<Real> &);

  GenPosPoint<Real> * Clone() const { return new GenPosPoint<Real>(*this); }

private:
  Dg::Vector4<Real> m_origin;
};

template<typename Real>
void GenPosPoint<Real>::SetTransformation(Dg::VQS<Real> const & a_vqs)
{
  origin = a_vqs.Translate(Dg::Vector4<Real>::Origin());
}

template<typename Real>
void GenPosPoint<Real>::Generate(int a_start, int a_end, Dg::ParticleData<Real> & a_data)
{
  Dg::Vector4<Real> * pPos = a_data.GetPosition();

  if (pPos)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pPos[i] = m_origin;
    }
  }
}

#endif