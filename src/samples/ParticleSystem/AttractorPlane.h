#ifndef ATTRACTORPLANE_H
#define ATTRACTORPLANE_H

#include "particle_system/DgAttractor.h"
#include "particle_system/DgParticleData.h"
#include "DgR3Plane.h"
#include "./query/DgR3QueryPointPlane.h"
#include "DgR3VQS.h"

template<typename Real>
class AttractorPlane : public Dg::Attractor<Real>
{
public:

  AttractorPlane()
    : Dg::Attractor<Real>()
  {}

  virtual ~AttractorPlane() {}

  AttractorPlane(AttractorPlane<Real> const &);

  AttractorPlane(Dg::Attractor<Real> const & a_attr)
    : Dg::Attractor<Real>(a_attr)
  {}

  AttractorPlane<Real> & operator=(AttractorPlane<Real> const &);

  //TODO Properly implement UpdateNew
  virtual void UpdateNew(Dg::ParticleData<Real> &, int, Real) {}
  virtual void Update(Dg::ParticleData<Real> &, int, Real);

  virtual void SetTransformation(Dg::R3::VQS<Real> const &);

  virtual AttractorPlane<Real> * Clone() const { return new AttractorPlane<Real>(*this); }

protected:
  Dg::R3::Plane<Real> m_plane;
};

template<typename Real>
AttractorPlane<Real>::AttractorPlane(AttractorPlane<Real> const & a_other)
  : Dg::Attractor<Real>(a_other)
  , m_plane(a_other.m_plane)
{
}

template<typename Real>
AttractorPlane<Real> & AttractorPlane<Real>::operator=(AttractorPlane<Real> const & a_other)
{
  Dg::Attractor<Real>::operator=(a_other);
  m_plane = a_other.m_plane;
  return *this;
}

template<typename Real>
void AttractorPlane<Real>::SetTransformation(Dg::R3::VQS<Real> const & a_vqs)
{
  m_plane.Set(a_vqs.Rotate(Dg::R3::Vector4<Real>::xAxis())
            , Dg::R3::Vector4<Real>::Origin() + a_vqs.V());
}

template<typename Real>
void AttractorPlane<Real>::Update(Dg::ParticleData<Real> & a_data
                                , int a_start
                                , Real a_dt)
{
  Dg::R3::Vector4<Real> * pPos = a_data.GetPosition();
  Dg::R3::Vector4<Real> * pAccels = a_data.GetAcceleration();

  Dg::R3::CPPointPlane<Real> query;
  Dg::R3::CPPointPlane<Real>::Result result;

  if (pPos && pAccels)
  {
    switch (GetAttenuationMethod())
    {
    case Constant:
    {
      for (int i = a_start; i < a_data.GetCountAlive(); ++i)
      {
        result = query(pPos[i], m_plane);
        pAccels[i] += (GetAccel_Constant(result.cp, pPos[i]));
      }
      break;
    }
    case Inverse:
    {
      for (int i = a_start; i < a_data.GetCountAlive(); ++i)
      {
        result = query(pPos[i], m_plane);
        pAccels[i] += (GetAccel_Inverse(result.cp, pPos[i]));
      }
      break;
    }
    case InverseSquare:
    {
      for (int i = a_start; i < a_data.GetCountAlive(); ++i)
      {
        result = query(pPos[i], m_plane);
        pAccels[i] += (GetAccel_InverseSquare(result.cp, pPos[i]));
      }
      break;
    }
    }
  }
}
#endif