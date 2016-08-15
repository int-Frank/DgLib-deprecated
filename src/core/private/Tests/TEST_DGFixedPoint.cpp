#include "TestHarness.h"
#include "DgFixedPoint.h"

typedef Dg::FixedPoint<int32_t, 24> FP_s32_24;

TEST(Stack_DgFixedPoint, creation_DgFixedPoint)
{
  //Construction form mixed FP types
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

  //Floating point conversion
  CHECK(static_cast<float>(fp_s32_24) == -13.375f);
  CHECK(static_cast<double>(fp_s32_24) == -13.375);
  CHECK(static_cast<long double>(fp_s32_24) == -13.375L);

  //Floating point construction
  CHECK(FP_s32_24(-13.375f) == FP_s32_24(-13.375));
  CHECK(FP_s32_24(-13.375f) == FP_s32_24(-13.375L));

  CHECK(static_cast<double>(-fp_s32_24) == 13.375);

  fp_u32_24 = 169.3;
  //CHECK(fp_u32_24.GetBase() == 2840382669);
  CHECK(static_cast<float>(fp_u32_24) == 169.300003f);

  //Integer construction and conversion
  CHECK(true == bool(FP_s32_24(bool(123))));
  CHECK(int8_t(123) == int8_t(FP_s32_24(int8_t(123))));
  CHECK(uint8_t(123) == uint8_t(FP_s32_24(uint8_t(123))));
  CHECK(int16_t(123) == int16_t(FP_s32_24(int16_t(123))));
  CHECK(uint16_t(123) == uint16_t(FP_s32_24(uint16_t(123))));
  CHECK(int32_t(123) == int32_t(FP_s32_24(int32_t(123))));
  CHECK(uint32_t(123) == uint32_t(FP_s32_24(uint32_t(123))));
  CHECK(int64_t(123) == int64_t(FP_s32_24(int64_t(123))));
  CHECK(uint64_t(123) == uint64_t(FP_s32_24(uint64_t(123))));

  //Assigment
  FP_s32_24 a(4.3);
  FP_s32_24 b(a);
  FP_s32_24 c = a;
  CHECK(a == b);
  CHECK(a == c);

  //Comparison
  b = 4.0;
  c = 5.25;
  FP_s32_24 d = a;
  FP_s32_24 e = -a;
  FP_s32_24 f = 0.0;
  CHECK(b < a);
  CHECK(b <= a);
  CHECK(c > a);
  CHECK(c >= a);
  CHECK(d <= a);
  CHECK(d >= a);
  CHECK(b != a);
  CHECK(-e == a);
  CHECK(!f);

  //De/Increment
  a = 6.0;
  b = a++;
  c = ++a;
  CHECK(static_cast<double>(a) == 8.0);
  CHECK(static_cast<double>(b) == 6.0);
  CHECK(static_cast<double>(c) == 8.0);


  a = 6.0;
  b = a--;
  c = --a;
  CHECK(static_cast<double>(a) == 4.0);
  CHECK(static_cast<double>(b) == 6.0);
  CHECK(static_cast<double>(c) == 4.0);

  //Shifting
  Dg::FixedPoint<uint32_t, 8> forShifts = 40.0;
  double res = static_cast<double>(forShifts << 2);
  CHECK(static_cast<double>(forShifts << 2) == 40.0 * 4.0);
  CHECK(static_cast<double>(forShifts >> 2) == 40.0 / 4.0);
  CHECK(static_cast<double>(forShifts <<= 2) == 40.0 * 4.0);
  CHECK(static_cast<double>(forShifts >>= 2) == 40.0);
}