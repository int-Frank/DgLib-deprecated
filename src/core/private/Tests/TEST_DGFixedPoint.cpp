#include "TestHarness.h"
#include "DgFixedPoint.h"

TEST(Stack_DgFixedPoint, creation_DgFixedPoint)
{
  Dg::impl::Promote<int> p;
  Dg::FixedPoint<uint32_t, 24> fp(1.5);
}