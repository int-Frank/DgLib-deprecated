#ifndef UPDATERZEROACCEL_H
#define UPDATERZEROACCEL_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleUpdater.h"

//! Zeros acceleration
template<typename Real>
class UpdaterZeroAccel : public Dg::ParticleUpdater<Real>
{
public:
  UpdaterZeroAccel() : Dg::ParticleUpdater<Real>() {}
  ~UpdaterZeroAccel() {}

  UpdaterZeroAccel(UpdaterZeroAccel<Real> const & a_other) :Dg::ParticleUpdater<Real>(a_other) {}
  UpdaterZeroAccel<Real> & operator=(UpdaterZeroAccel<Real> const & a_other)
  {
    Dg::ParticleUpdater<Real>::operator=(a_other);
    return *this;
  }

  void UpdateNew(Dg::ParticleData<Real> &, int, Real) {}
  void Update(Dg::ParticleData<Real> &, int, Real);

  UpdaterZeroAccel<Real> * Clone() const { return new UpdaterZeroAccel<Real>(*this); }

};

template<typename Real>
void UpdaterZeroAccel<Real>::Update(Dg::ParticleData<Real> & a_data
                                  , int a_start
                                  , Real a_dt)
{
  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();

  int maxParCount = a_data.GetCountAlive();

  if (pAccels)
  {
    for (int i = a_start; i < maxParCount; ++i)
    {
      pAccels[i].Zero();
    }
  }
}

#endif