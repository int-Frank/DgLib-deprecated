#include "TestHarness.h"
#include "DgFixedPoint.h"

TEST(Stack_DgFixedPoint, creation_DgFixedPoint)
{
  //Using results from http://www.rfwireless-world.com/calculators/floating-vs-fixed-point-converter.html
  Dg::FixedPoint<uint32_t, 24> fp_u32_24(13.375);
  CHECK(static_cast<uint32_t>(fp_u32_24) == 224395264);
  CHECK(static_cast<double>(fp_u32_24) == 13.375);
  CHECK(static_cast<double>(static_cast<Dg::FixedPoint<uint32_t, 8>>(fp_u32_24)) == 13.375);

  Dg::FixedPoint<int32_t, 24> fp_s32_24(-13.375);
  CHECK(static_cast<double>(fp_s32_24) == -13.375);

  fp_u32_24 = 169.3;
  CHECK(static_cast<uint32_t>(fp_u32_24) == 2840382669);
  CHECK(static_cast<float>(fp_u32_24) == 169.299988f);


}