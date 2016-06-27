#include "TestHarness.h"
#include "particle_system/ParticleData.h"


TEST(Stack_ParticleSystem, DgParticleSystem)
{
  Dg::ParticleData<float,   (1 << Dg::ParticleDataAttributes::ID)
                          | (1 << Dg::ParticleDataAttributes::StartSize)
                          | (1 << Dg::ParticleDataAttributes::EndSize)
                          | (1 << Dg::ParticleDataAttributes::StartColor)
                          | (1 << Dg::ParticleDataAttributes::EndColor)> pd(1024);
  char u = 0;
}