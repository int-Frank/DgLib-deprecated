#ifndef UPDATERSIZE_H
#define UPDATERSIZE_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleUpdater.h"

//! Updates particle size
template<typename Real>
class UpdaterSize : public Dg::ParticleUpdater<Real>
{
public:
  UpdaterSize() : Dg::ParticleUpdater<Real>() {}
  ~UpdaterSize() {}

  UpdaterSize(UpdaterSize<Real> const & a_other) :Dg::ParticleUpdater<Real>(a_other) {}
  UpdaterSize<Real> & operator=(UpdaterSize<Real> const & a_other)
  {
    Dg::ParticleUpdater<Real>::operator=(a_other);
    return *this;
  }

  //TODO Properly implement UpdateNew
  void UpdateNew(Dg::ParticleData<Real> & data, int start, Real dt) { implUpdate(data, start, dt); }
  void Update(Dg::ParticleData<Real> & data, int start, Real dt) { implUpdate(data, start, dt); }

  UpdaterSize<Real> * Clone() const { return new UpdaterSize<Real>(*this); }

private:
  void implUpdate(Dg::ParticleData<Real> &, int, Real);
};

template<typename Real>
void UpdaterSize<Real>::implUpdate(Dg::ParticleData<Real> & a_data
                                 , int a_start
                                 , Real a_dt)
{
  Real * pSize = a_data.GetSize();
  Real * pStartSize = a_data.GetStartSize();
  Real * pDSize = a_data.GetDSize();
  Real * pDLifes = a_data.GetDLife();

  int maxParCount = a_data.GetCountAlive();

  if (pSize && pStartSize && pDSize)
  {
    if (pDLifes)
    {
      for (int i = a_start; i < maxParCount; ++i)
      {
        pSize[i] = pStartSize[i] + pDLifes[i] * pDSize[i];
      }
    }
    else
    {
      Real * pLifes = a_data.GetLife();
      Real * pLifeMaxes = a_data.GetLifeMax();
      {
        if (pLifes && pLifeMaxes)
        {
          for (int i = a_start; i < maxParCount; ++i)
          {
            pSize[i] = pStartSize[i] + (pLifes[i] / pLifeMaxes[i]) * pDSize[i];
          }
        }
      }
    }
  }
}

#endif