#ifndef EMITTERFACTORY_H
#define EMITTERFACTORY_H

class EmitterData;

namespace Dg
{
  template<typename Real>
  class ParticleEmitter;

  template<typename Real>
  class ParticleGenerator;
}

class EmitterFactory
{
public:

  Dg::ParticleEmitter<float> * operator()(EmitterData const &) const;

  Dg::ParticleGenerator<float> * CreateGenPosition(EmitterData const &) const;
  Dg::ParticleGenerator<float> * CreateGenVelocity(EmitterData const &) const;

  Dg::ParticleGenerator<float> * CreateGenColor(EmitterData const &) const;
  Dg::ParticleGenerator<float> * CreateGenLife(EmitterData const &) const;
  Dg::ParticleGenerator<float> * CreateGenPosPoint(EmitterData const &) const;
  Dg::ParticleGenerator<float> * CreateGenPosSphere(EmitterData const &) const;
  Dg::ParticleGenerator<float> * CreateGenSize(EmitterData const &) const;
  Dg::ParticleGenerator<float> * CreateGenVelCone(EmitterData const &) const;
  Dg::ParticleGenerator<float> * CreateGenVelOutwards(EmitterData const &) const;
  Dg::ParticleGenerator<float> * CreateGenRelativeForce(EmitterData const &) const;

};

#endif