#ifndef UPDATERRESETACCEL_H
#define UPDATERRESETACCEL_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleUpdater.h"

//! Zeros acceleration
template<typename Real>
class UpdaterResetAccel : public Dg::ParticleUpdater<Real>
{
public:
  UpdaterResetAccel() : Dg::ParticleUpdater<Real>() {}
  ~UpdaterResetAccel() {}

  UpdaterResetAccel(UpdaterResetAccel<Real> const & a_other) :Dg::ParticleUpdater<Real>(a_other) {}
  UpdaterResetAccel<Real> & operator=(UpdaterResetAccel<Real> const & a_other)
  {
    Dg::ParticleUpdater<Real>::operator=(a_other);
    return *this;
  }

  //TODO Properly implement UpdateNew
  void UpdateNew(Dg::ParticleData<Real> & data, int start, Real dt) { implUpdate(data, start, dt); }
  void Update(Dg::ParticleData<Real> & data, int start, Real dt) { implUpdate(data, start, dt); }

  UpdaterResetAccel<Real> * Clone() const { return new UpdaterResetAccel<Real>(*this); }

private:
  void implUpdate(Dg::ParticleData<Real> &, int, Real);
};

template<typename Real>
void UpdaterResetAccel<Real>::implUpdate(Dg::ParticleData<Real> & a_data
                                      , int a_start
                                      , Real a_dt)
{
  Dg::R3::Vector4<Real> * pAccels = a_data.GetAcceleration();

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