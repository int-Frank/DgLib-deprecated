#include "TestHarness.h"
#include "DgBoundedSND.h"


TEST(Stack_BoundedSND, creation_BoundedSND)
{
  Dg::BoundedSND<double> b;
  b.Init(0.0, 1.0, -3.0, 3.0, 100);
}