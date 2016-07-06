#ifndef UPDATERLIFE_H
#define UPDATERLIFE_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleUpdater.h"

//! Updates particle life, kills particles.
template<typename Real>
class UpdaterLife : public Dg::ParticleUpdater<Real>
{
public:
  UpdaterLife() : Dg::ParticleUpdater<Real>() {}
  ~UpdaterLife() {}

  UpdaterLife(UpdaterLife<Real> const & a_other) :Dg::ParticleUpdater<Real>(a_other) {}
  UpdaterLife<Real> & operator=(UpdaterLife<Real> const & a_other)
  {
    Dg::ParticleUpdater<Real>::operator=(a_other);
    return *this;
  }

  void UpdateNew(Dg::ParticleData<Real> &, int, Real) {}
  void Update(Dg::ParticleData<Real> &, int, Real);

  UpdaterLife<Real> * Clone() const { return new UpdaterLife<Real>(*this); }

};

template<typename Real>
void UpdaterLife<Real>::Update(Dg::ParticleData<Real> & a_data
                             , int a_start
                             , Real a_dt)
{
  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();
  Real * pLifes = a_data.GetLife();
  Real * pLifeMaxes = a_data.GetLifeMax();
  Real * pDLifes = a_data.GetDLife();

  int maxParCount = a_data.GetCountAlive();

  if (pLifes && pLifeMaxes)
  {
    for (int i = maxParCount - 1; i >= a_start; --i)
    {
      pLifes[i] += a_dt;
      if (pLifes[i] >= pLifeMaxes[i])
      {
        maxParCount = a_data.Kill(i);
      }
    }

    if (pDLifes)
    {
      for (int i = a_start; i < maxParCount; ++i)
      {
        pDLifes[i] = pLifes[i] / pLifeMaxes[i];
      }
    }
  }
}

#endif