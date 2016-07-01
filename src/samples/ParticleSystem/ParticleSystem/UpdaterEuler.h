#ifndef UPDATEREULER_H
#define UPDATEREULER_H

#include <math.h>

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleUpdater.h"

//! Updates position
template<typename Real>
class UpdaterEuler : public Dg::ParticleUpdater<Real>
{
  UpdaterEuler() : Dg::ParticleUpdater<Real>() {}
  ~UpdaterEuler() {}

  UpdaterEuler(UpdaterEuler<Real> const & a_other) :Dg::ParticleUpdater<Real>(a_other) {}
  UpdaterEuler<Real> & operator=(UpdaterEuler<Real> const & a_other) 
  {
    Dg::ParticleUpdater<Real>::operator=(a_other);
    return *this; 
  }

  void Update(int start,
              int finish,
              Real dt,
              Dg::ParticleData<Real> & data);

  UpdaterEuler<Real> * Clone() const { return new UpdaterEuler<Real>(*this); }

};

template<typename Real>
void UpdaterEuler<Real>::Update(int a_start,
                                int a_finish,
                                Real a_dt,
                                Dg::ParticleData<Real> & a_data)
{
  Dg::Vector4<Real> * pPos = a_data.GetPosition();
  Dg::Vector4<Real> * pVels = a_data.GetVelocity();
  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();
  Dg::Vector4<Real> * pForces = a_data.GetForce();

  if (pVels)
  {
    if (pAccels && pForces)
    {
      for (int i = a_start; i <= a_finish; ++i)
      {
        pVels[i] = (pVels[i] + pAccels[i] * a_dt) * std::pow(pForces[i], a_dt);
      }
    }
    else if (pAccels)
    {
      for (int i = a_start; i <= a_finish; ++i)
      {
        pVels[i] += (pAccels[i] * a_dt);
      }
    }
    else if (pForces)
    {
      for (int i = a_start; i <= a_finish; ++i)
      {
        pVels[i] *= std::pow(pForces[i], a_dt);
      }
    }

    for (int i = a_start; i <= a_finish; ++i)
    {
      pPos[i] += pVels[i];
    }

  }
}

#endif