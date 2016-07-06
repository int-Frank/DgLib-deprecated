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

  void Update(Dg::ParticleData<Real> &, int) {}
  void Update(Dg::ParticleData<Real> &, int, Real);

  UpdaterColor<Real> * Clone() const { return new UpdaterColor<Real>(*this); }

};

template<typename Real>
void UpdaterColor<Real>::Update(Dg::ParticleData<Real> & a_data
                              , int a_start
                              , Real a_dt)
{
  Dg::Vector4<float> * pColors = a_data.GetColor();
  Dg::Vector4<float> * pStartColors = a_data.GetStartColor();
  Dg::Vector4<float> * pDColors = a_data.GetDColor();
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