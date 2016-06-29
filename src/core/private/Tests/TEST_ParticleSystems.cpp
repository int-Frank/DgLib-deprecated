#include "TestHarness.h"
#include "particle_system/DgParticleData.h"
#include "particle_system/DgParticleSystem.h"


TEST(Stack_ParticleSystem, DgParticleSystem)
{
  Dg::ParticleData<float,   (1 << Dg::ParticleDataAttributes::ID)
                          | (1 << Dg::ParticleDataAttributes::StartSize)
                          | (1 << Dg::ParticleDataAttributes::EndSize)
                          | (1 << Dg::ParticleDataAttributes::StartColor)
                          | (1 << Dg::ParticleDataAttributes::EndColor)> 
    pd(1024);
  
  Dg::ParticleSystem<float> ps(1024);
}