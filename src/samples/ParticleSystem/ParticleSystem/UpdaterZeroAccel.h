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

  void Update(int start,
              int finish,
              Real dt,
              Dg::ParticleData<Real> & data);

  UpdaterZeroAccel<Real> * Clone() const { return new UpdaterZeroAccel<Real>(*this); }

};

template<typename Real>
void UpdaterZeroAccel<Real>::Update(int a_start,
                                    int a_finish,
                                    Real a_dt,
                                    Dg::ParticleData<Real> & a_data)
{
  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();

  if (pAccels)
  {
    for (int i = a_start; i <= a_finish; ++i)
    {
      pAccels[i].Zero();
    }
  }
}

#endif