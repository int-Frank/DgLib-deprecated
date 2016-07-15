#ifndef UPDATEREULER_H
#define UPDATEREULER_H

#include <cmath>

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleUpdater.h"

//! Updates position
template<typename Real>
class UpdaterEuler : public Dg::ParticleUpdater<Real>
{
public:
  UpdaterEuler() : Dg::ParticleUpdater<Real>() {}
  ~UpdaterEuler() {}

  UpdaterEuler(UpdaterEuler<Real> const & a_other) :Dg::ParticleUpdater<Real>(a_other) {}
  UpdaterEuler<Real> & operator=(UpdaterEuler<Real> const & a_other) 
  {
    Dg::ParticleUpdater<Real>::operator=(a_other);
    return *this; 
  }

  void UpdateNew(Dg::ParticleData<Real> &, int, Real);
  void Update(Dg::ParticleData<Real> &, int, Real);

  UpdaterEuler<Real> * Clone() const { return new UpdaterEuler<Real>(*this); }

};


template<typename Real>
void UpdaterEuler<Real>::UpdateNew(Dg::ParticleData<Real> & a_data
                                 , int a_start
                                 , Real a_dt)
{
  Dg::Vector4<Real> * pPos = a_data.GetPosition();
  Dg::Vector4<Real> * pVels = a_data.GetVelocity();
  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();
  Real * ptimeSinceBirth = a_data.GetTimeSinceBirth();

  if (ptimeSinceBirth == nullptr)
  {
    return;
  }

  int maxParCount = a_data.GetCountAlive();

  if (pVels)
  {
    if (pAccels)
    {
      for (int i = a_start; i < maxParCount; ++i)
      {
        pVels[i] += (pAccels[i] * ptimeSinceBirth[i]);
      }
    }

    if (pPos)
    {
      for (int i = a_start; i < maxParCount; ++i)
      {
        pPos[i] += pVels[i] * ptimeSinceBirth[i];
      }
    }
  }
}


template<typename Real>
void UpdaterEuler<Real>::Update(Dg::ParticleData<Real> & a_data
                              , int a_start
                              , Real a_dt)
{
  Dg::Vector4<Real> * pPos = a_data.GetPosition();
  Dg::Vector4<Real> * pVels = a_data.GetVelocity();
  Dg::Vector4<Real> * pAccels = a_data.GetAcceleration();

  int maxParCount = a_data.GetCountAlive();

  if (pVels)
  {
    if (pAccels)
    {
      for (int i = a_start; i < maxParCount; ++i)
      {
        pVels[i] += (pAccels[i] * a_dt);
      }
    }

    if (pPos)
    {
      for (int i = a_start; i < maxParCount; ++i)
      {
        pPos[i] += pVels[i] * a_dt;
      }
    }
  }
}

#endif