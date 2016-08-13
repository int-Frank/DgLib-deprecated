#include "TestHarness.h"
#include "DgFixedPoint.h"

typedef Dg::FixedPoint<int32_t, 24> FP_s32_24;

TEST(Stack_DgFixedPoint, creation_DgFixedPoint)
{
  //Using results from http://www.rfwireless-world.com/calculators/floating-vs-fixed-point-converter.html
  Dg::FixedPoint<uint32_t, 24> fp_u32_24(13.375);
  //CHECK(fp_u32_24.GetBase() == 224395264);
  CHECK(static_cast<double>(fp_u32_24) == 13.375);
  double val = static_cast<double>(static_cast<Dg::FixedPoint<uint32_t, 8>>(fp_u32_24));
  CHECK(static_cast<double>(static_cast<Dg::FixedPoint<uint32_t, 8>>(fp_u32_24)) == 13.375);
  CHECK(static_cast<double>(static_cast<Dg::FixedPoint<uint32_t, 26>>(fp_u32_24)) == 13.375);
  CHECK(static_cast<double>(static_cast<Dg::FixedPoint<uint32_t, 30>>(fp_u32_24)) == 1.375);
  
  CHECK(static_cast<double>(static_cast<Dg::FixedPoint<int32_t, 8>>(fp_u32_24)) == 13.375);
  CHECK(static_cast<double>(static_cast<Dg::FixedPoint<int32_t, 26>>(fp_u32_24)) == 13.375);
  CHECK(static_cast<double>(static_cast<Dg::FixedPoint<int32_t, 30>>(fp_u32_24)) == 1.375);
  
  Dg::FixedPoint<int32_t, 24> fp_s32_24(-13.375);
  CHECK(static_cast<double>(fp_s32_24) == -13.375);
  
  CHECK(static_cast<double>(static_cast<Dg::FixedPoint<int32_t, 8>>(fp_s32_24)) == -13.375);
  CHECK(static_cast<double>(static_cast<Dg::FixedPoint<int32_t, 26>>(fp_s32_24)) == -13.375);
  CHECK(static_cast<double>(static_cast<Dg::FixedPoint<int32_t, 30>>(fp_s32_24)) == -1.375);

  //! Floating point conversion
  CHECK(static_cast<float>(fp_s32_24) == -13.375f);
  CHECK(static_cast<double>(fp_s32_24) == -13.375);
  CHECK(static_cast<long double>(fp_s32_24) == -13.375L);

  CHECK(FP_s32_24(-13.375f) == FP_s32_24(-13.375));
  CHECK(FP_s32_24(-13.375f) == FP_s32_24(-13.375L));

  CHECK(static_cast<double>(-fp_s32_24) == 13.375);

  fp_u32_24 = 169.3;
  //CHECK(fp_u32_24.GetBase() == 2840382669);
  CHECK(static_cast<float>(fp_u32_24) == 169.300003f);

  //Comparison

}