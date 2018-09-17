#include "TestHarness.h"
#include "DgMesh.h"


TEST(Stack_DgMesh, creation_DgMesh)
{
  Dg::eHandle eh;
  Dg::MeshTools::Source(eh);
  Dg::MeshTools::Target(eh);
}