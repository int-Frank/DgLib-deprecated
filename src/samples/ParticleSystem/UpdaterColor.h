#ifndef UPDATERCOLOR_H
#define UPDATERCOLOR_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleUpdater.h"

//! Updates particle color
template<typename Real>
class UpdaterColor : public Dg::ParticleUpdater<Real>
{
public:
  UpdaterColor() : Dg::ParticleUpdater<Real>() {}
  ~UpdaterColor() {}

  UpdaterColor(UpdaterColor<Real> const & a_other) :Dg::ParticleUpdater<Real>(a_other) {}
  UpdaterColor<Real> & operator=(UpdaterColor<Real> const & a_other) 
  {
    Dg::ParticleUpdater<Real>::operator=(a_other);
    return *this; 
  }

  //TODO Properly implement UpdateNew
  void UpdateNew(Dg::ParticleData<Real> & data, int start, Real dt) { implUpdate(data, start, dt); }
  void Update(Dg::ParticleData<Real> & data, int start, Real dt) { implUpdate(data, start, dt); }

  UpdaterColor<Real> * Clone() const { return new UpdaterColor<Real>(*this); }

private:
  void implUpdate(Dg::ParticleData<Real> &, int, Real);
};


template<typename Real>
void UpdaterColor<Real>::implUpdate(Dg::ParticleData<Real> & a_data
                                  , int a_start
                                  , Real a_dt)
{
  Dg::R3::Vector4<float> * pColors = a_data.GetColor();
  Dg::R3::Vector4<float> * pStartColors = a_data.GetStartColor();
  Dg::R3::Vector4<float> * pDColors = a_data.GetDColor();
  Real * pDLifes = a_data.GetDLife();

  int maxParCount = a_data.GetCountAlive();

  if (pColors && pStartColors && pDColors)
  {
    if (pDLifes)
    {
      for (int i = a_start; i < maxParCount; ++i)
      {
        pColors[i] = pStartColors[i] + pDLifes[i] * pDColors[i];
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
            pColors[i] = pStartColors[i] + (pLifes[i] / pLifeMaxes[i]) * pDColors[i];
          }
        }
      }
    }
  }
}

#endif