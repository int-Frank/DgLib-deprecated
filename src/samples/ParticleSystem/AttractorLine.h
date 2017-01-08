#ifndef ATTRACTORLINE_H
#define ATTRACTORLINE_H

#include "particle_system/DgAttractor.h"
#include "particle_system/DgParticleData.h"
#include "DgR3Line.h"
#include "./query/DgR3QueryPointLine.h"
#include "DgR3VQS.h"

template<typename Real>
class AttractorLine : public Dg::Attractor<Real>
{
public:

  AttractorLine()
    : Dg::Attractor<Real>()
  {}

  virtual ~AttractorLine() {}

  AttractorLine(AttractorLine<Real> const &);

  AttractorLine(Dg::Attractor<Real> const & a_attr)
    : Dg::Attractor<Real>(a_attr)
  {}

  AttractorLine<Real> & operator=(AttractorLine<Real> const &);

  //TODO Properly implement UpdateNew
  virtual void UpdateNew(Dg::ParticleData<Real> &, int, Real) {}
  virtual void Update(Dg::ParticleData<Real> &, int, Real);

  virtual void SetTransformation(Dg::R3::VQS<Real> const &);

  virtual AttractorLine<Real> * Clone() const { return new AttractorLine<Real>(*this); }

protected:
  Dg::R3::Line<Real> m_line;
};

template<typename Real>
AttractorLine<Real>::AttractorLine(AttractorLine<Real> const & a_other)
  : Dg::Attractor<Real>(a_other)
  , m_line(a_other.m_line)
{
}

template<typename Real>
AttractorLine<Real> & AttractorLine<Real>::operator=(AttractorLine<Real> const & a_other)
{
  Dg::Attractor<Real>::operator=(a_other);
  m_line = a_other.m_line;
  return *this;
}

template<typename Real>
void AttractorLine<Real>::SetTransformation(Dg::R3::VQS<Real> const & a_vqs)
{
  m_line.Set(Dg::R3::Vector4<Real>::Origin(), Dg::R3::Vector4<Real>::xAxis());
  m_line.TransformSelf(a_vqs);
}

template<typename Real>
void AttractorLine<Real>::Update(Dg::ParticleData<Real> & a_data
  , int a_start
  , Real a_dt)
{
  Dg::R3::Vector4<Real> * pPos = a_data.GetPosition();
  Dg::R3::Vector4<Real> * pAccels = a_data.GetAcceleration();

  if (pPos && pAccels)
  {
    Dg::R3::CPPointLine<Real> query;
    Dg::R3::CPPointLine<Real>::Result result;

    switch (GetAttenuationMethod())
    {
    case Constant:
    {
      for (int i = a_start; i < a_data.GetCountAlive(); ++i)
      {
        result = query(pPos[i], m_line);
        pAccels[i] += (GetAccel_Constant(result.cp, pPos[i]));
      }
      break;
    }
    case Inverse:
    {
      for (int i = a_start; i < a_data.GetCountAlive(); ++i)
      {
        result = query(pPos[i], m_line);
        pAccels[i] += (GetAccel_Inverse(result.cp, pPos[i]));
      }
      break;
    }
    case InverseSquare:
    {
      for (int i = a_start; i < a_data.GetCountAlive(); ++i)
      {
        result = query(pPos[i], m_line);
        pAccels[i] += (GetAccel_InverseSquare(result.cp, pPos[i]));
      }
      break;
    }
    }
  }
}
#endif