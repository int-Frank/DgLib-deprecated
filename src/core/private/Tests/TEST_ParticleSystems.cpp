#include "TestHarness.h"
#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleSystem.h"

int test()
{
  int x = 1;
  return x++;
}

TEST(Stack_ParticleSystem, DgParticleSystem)
{
  int g = test();
  Dg::ParticleData<float>  pd(1024);
  
  Dg::ParticleSystem<float> ps(1024);
}