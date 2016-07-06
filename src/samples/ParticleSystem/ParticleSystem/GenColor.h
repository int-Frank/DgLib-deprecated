#ifndef GENCOLOR_H
#define GENCOLOR_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleGenerator.h"

//! Updates particle color
template<typename Real>
class GenColor : public Dg::ParticleGenerator<Real>
{
public:
  GenColor() : Dg::ParticleGenerator<Real>()
             , m_startColor(Dg::Vector4<Real>::Ones())
             , m_dColor(Dg::Vector4<Real>::Ones()) {}

  ~GenColor() {}

  GenColor(GenColor<Real> const & a_other)
    : Dg::ParticleGenerator<Real>(a_other)
    , m_startColor(a_other.m_startColor) 
    , m_dColor(a_other.m_dColor) {}

  GenColor<Real> & operator=(GenColor<Real> const & a_other)
  {
    Dg::ParticleGenerator<Real>::operator=(a_other);
    m_startColor = a_other.m_startColor;
    m_dColor = a_other.m_dColor;
    return *this;
  }

  void SetColors(Dg::Vector4<Real> const & a_startColor
               , Dg::Vector4<Real> const & a_endColor);

  void Generate(Dg::ParticleData<Real> &, int, int);

  GenColor<Real> * Clone() const { return new GenColor<Real>(*this); }

private:
  Dg::Vector4<Real> m_startColor;
  Dg::Vector4<Real> m_dColor;
};

template<typename Real>
void GenColor<Real>::Generate(Dg::ParticleData<Real> & a_data, int a_start, int a_end)
{
  Dg::Vector4<Real> * pColor = a_data.GetColor();
  Dg::Vector4<Real> * pStartColor = a_data.GetStartColor();
  Dg::Vector4<Real> * pDColor = a_data.GetDColor();

  if (pColor)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pColor[i] = m_startColor;
    }
  }
  if (pStartColor)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pStartColor[i] = m_startColor;
    }
  }
  if (pColor)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pDColor[i] = m_dColor;
    }
  }
}


template<typename Real>
void GenColor<Real>::SetColors(Dg::Vector4<Real> const & a_startColor
                             , Dg::Vector4<Real> const & a_endColor)
{
  m_startColor = a_startColor;
  m_dColor = a_endColor - a_startColor;
}

#endif