#ifndef UPDATERLIFE_H
#define UPDATERLIFE_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleUpdater.h"

//! Updates particle life, kills particles.
template<typename Real>
class UpdaterLife : public Dg::ParticleUpdater<Real>
{
  UpdaterLife() : Dg::ParticleUpdater<Real>() {}
  ~UpdaterLife() {}

  UpdaterLife(UpdaterLife<Real> const & a_other) :Dg::ParticleUpdater<Real>(a_other) {}
  UpdaterLife<Real> & operator=(UpdaterLife<Real> const & a_other)
  {
    Dg::ParticleUpdater<Real>::operator=(a_other);
    return *this;
  }

  void Update(int start,
              int finish,
              Real dt,
              Dg::ParticleData<Real> & data);

  UpdaterLife<Real> * Clone() const { return new UpdaterLife<Real>(*this); }

};

template<typename Real>
void UpdaterLife<Real>::Update(int a_start,
                                int a_finish,
                                Real a_dt,
                                Dg::ParticleData<Real> & a_data)
{
  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();
  Real * pLifes = a_data.GetLife();
  Real * pLifeMaxes = a_data.GetLifeMax();
  Real * pDLifes = a_data.GetDLife();

  if (pLifes && pLifeMaxes)
  {
    for (int i = a_finish; i >= a_start; --i)
    {
      pLifes[i] += a_dt;
      if (pLifes[i] >= pLifeMaxes[i])
      {
        a_data.Kill(i);
        a_finish--;
      }
    }

    if (pDLifes)
    {
      for (int i = a_start; i <= a_finish; ++i)
      {
        pDLifes[i] = pLifes[i] / pLifeMaxes[i];
      }
    }
  }

  if (pAccels)
  {
    for (int i = a_start; i <= a_finish; ++i)
    {
      pAccels[i].Zero();
    }
  }
}

#endif