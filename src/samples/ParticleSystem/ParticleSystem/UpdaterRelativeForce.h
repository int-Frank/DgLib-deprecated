#ifndef UPDATERRELATIVEFORCE_H
#define UPDATERRELATIVEFORCE_H

#include <cmath>

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleUpdater.h"

//! Updates position
template<typename Real>
class UpdaterRelativeForce : public Dg::ParticleUpdater<Real>
{
public:
  UpdaterRelativeForce() : Dg::ParticleUpdater<Real>() {}
  ~UpdaterRelativeForce() {}

  UpdaterRelativeForce(UpdaterRelativeForce<Real> const & a_other) :Dg::ParticleUpdater<Real>(a_other) {}
  UpdaterRelativeForce<Real> & operator=(UpdaterRelativeForce<Real> const & a_other)
  {
    Dg::ParticleUpdater<Real>::operator=(a_other);
    return *this;
  }

  void UpdateNew(Dg::ParticleData<Real> &, int, Real) {}
  void Update(Dg::ParticleData<Real> &, int, Real);

  UpdaterRelativeForce<Real> * Clone() const { return new UpdaterRelativeForce<Real>(*this); }

};


template<typename Real>
void UpdaterRelativeForce<Real>::Update(Dg::ParticleData<Real> & a_data
                                      , int a_start
                                      , Real a_dt)
{
  Dg::Vector4<Real> * pVels = a_data.GetVelocity();
  Real * pForces = a_data.GetForce();

  int maxParCount = a_data.GetCountAlive();

  if (pVels && pForces)
  {
    for (int i = a_start; i < maxParCount; ++i)
    {
      pVels[i] *= std::pow(pForces[i], a_dt);
    }
  }
}

#endif