#ifndef GENLIFE_H
#define GENLIFE_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleGenerator.h"

//! Updates particle color
template<typename Real>
class GenLife : public Dg::ParticleGenerator<Real>
{
public:
  GenLife() : Dg::ParticleGenerator<Real>()
            , m_life(static_cast<Real>(1.0)) {}

  ~GenLife() {}

  GenLife(GenLife<Real> const & a_other)
    : Dg::ParticleGenerator<Real>(a_other)
    , m_life(a_other.m_life){}

  GenLife<Real> & operator=(GenLife<Real> const & a_other)
  {
    Dg::ParticleGenerator<Real>::operator=(a_other);
    m_life = a_other.m_life;
    return *this;
  }

  void SetLife(Real a_life) {m_life = a_life};

  void Generate(Dg::ParticleData<Real> &, int, int);

  GenLife<Real> * Clone() const { return new GenLife<Real>(*this); }

private:
  Real      m_life;
};

template<typename Real>
void GenLife<Real>::Generate(Dg::ParticleData<Real> & a_data, int a_start, int a_end)
{
  Real * pLifes = a_data.GetLife();
  Real * pLifeMaxes = a_data.GetLifeMax();
  Real * pDLifes = a_data.GetDLife();

  if (pLifes)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pLifes[i] = static_cast<Real>(0.0);
    }
  }
  if (pLifeMaxes)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pLifeMaxes[i] = m_life;
    }
  }
  if (pDLifes)
  {
    for (int i = a_start; i <= a_end; ++i)
    {
      pDLifes[i] = static_cast<Real>(1.0);
    }
  }
}

#endif