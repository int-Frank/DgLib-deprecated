#include "TestHarness.h"
#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleSystem.h"


TEST(Stack_ParticleSystem, DgParticleSystem)
{
  Dg::Attractor<float> att, att2;

  att = att2;

  Dg::ParticleData<float>  pd(1024);
  
  Dg::ParticleSystem<float> ps(1024);
}