#ifndef UPDATERCOLOR_H
#define UPDATERCOLOR_H

#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleUpdater.h"

//! Updates particle color
template<typename Real>
class UpdaterColor : public Dg::ParticleUpdater<Real>
{
  UpdaterColor() : Dg::ParticleUpdater<Real>() {}
  ~UpdaterColor() {}

  UpdaterColor(UpdaterColor<Real> const & a_other) :Dg::ParticleUpdater<Real>(a_other) {}
  UpdaterColor<Real> & operator=(UpdaterColor<Real> const & a_other) 
  {
    Dg::ParticleUpdater<Real>::operator=(a_other);
    return *this; 
  }

  void Update(int start,
              int finish,
              Real dt,
              Dg::ParticleData<Real> & data);

  UpdaterColor<Real> * Clone() const { return new UpdaterColor<Real>(*this); }

};

template<typename Real>
void UpdaterColor<Real>::Update(int a_start,
                                int a_finish,
                                Real a_dt,
                                Dg::ParticleData<Real> & a_data)
{
  Dg::Vector4<float> * pColors = a_data.GetColor();
  Dg::Vector4<float> * pStartColors = a_data.GetStartColor();
  Dg::Vector4<float> * pDColors = a_data.GetDColor();
  Real * pDLifes = a_data.GetDLife();

  if (pColors && pStartColors && pDColors)
  {
    if (pDLifes)
    {
      for (int i = a_start; i <= a_finish; ++i)
      {
        pColors[i] = StartColor[i] + pDLifes[i] * pDColor[i];
      }
    }
    else
    {
      Real * pLifes = a_data.GetLife();
      Real * pLifeMaxes = a_data.GetLifeMax();
      {
        if (pLifes && pLifeMaxes)
        {
          for (int i = a_start; i <= a_finish; ++i)
          {
            pColors[i] = StartColor[i] + (pLifes[i] / pLifeMaxes[i]) * DColor[i];
          }
        }
      }
    }
  }
}

#endif