#include "TestHarness.h"

#include "dgmath.h"

using namespace Dg;

TEST(Stack_math, creation_math)
{
  /// Log ////////////////////////////////////////////////////////////////

  uint32_t r_Log2 = Log2_32(0x1FFFF);
  CHECK((r_Log2 == 16));

  /// Wrap ////////////////////////////////////////////////////////////////

  double lower = -2.0;
  double upper = 3.0;
  double val = 0.0;

  CHECK(Dg::NextPower2<uint32_t>(0) == 0);
  CHECK(Dg::NextPower2<uint32_t>(5) == 8);
  CHECK(Dg::NextPower2<uint32_t>(1000) == 1024);
  CHECK(Dg::NextPower2<uint32_t>(0xFFFFFFFF) == 0);

  CHECK(Dg::FloorPower2<uint32_t>(0) == 0);
  CHECK(Dg::FloorPower2<uint32_t>(5) == 4);
  CHECK(Dg::FloorPower2<uint32_t>(1000) == 512);
  CHECK(Dg::FloorPower2<uint32_t>(0xFFFFFFFF) == 0x80000000);

  WrapNumber<double>(lower, upper, val);
  CHECK(val == 0.0);

  val = -2.0;
  WrapNumber<double>(lower, upper, val);
  CHECK(val == -2.0);

  val = -4.0;
  WrapNumber<double>(lower, upper, val);
  CHECK(val == 1.0);

  val = 4.0;
  WrapNumber<double>(lower, upper, val);
  CHECK(val == -1.0);

  lower = -5.0;
  upper = -1.0;

  val = -4.0;
  WrapNumber<double>(lower, upper, val);
  CHECK(val == -4.0);

  val = -6.0;
  WrapNumber<double>(lower, upper, val);
  CHECK(val == -2.0);

  val = 2.0;
  WrapNumber<double>(lower, upper, val);
  CHECK(val == -2.0);

  lower = 1.0;
  upper = 5.0;

  val = 4.0;
  WrapNumber<double>(lower, upper, val);
  CHECK(val == 4.0);

  val = 6.0;
  WrapNumber<double>(lower, upper, val);
  CHECK(val == 2.0);

  val = -2.0;
  WrapNumber<double>(lower, upper, val);
  CHECK(val == 2.0);

  int const isPrime[256] =
  {
    0, 0, 1, 1, 0,    1, 0, 1, 0, 0,
    0, 1, 0, 1, 0,    0, 0, 1, 0, 1,
    0, 0, 0, 1, 0,    0, 0, 0, 0, 1,
    0, 1, 0, 0, 0,    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,    0, 0, 1, 0, 0,
    0, 0, 0, 1, 0,    0, 0, 0, 0, 1,
    0, 1, 0, 0, 0,    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,    0, 0, 0, 0, 1,
    0, 0, 0, 1, 0,    0, 0, 0, 0, 1,
    0, 0, 0, 0, 0,    0, 0, 1, 0, 0,

    0, 1, 0, 1, 0,    0, 0, 1, 0, 1,
    0, 0, 0, 1, 0,    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,    0, 0, 1, 0, 0,
    0, 1, 0, 0, 0,    0, 0, 1, 0, 1,
    0, 0, 0, 0, 0,    0, 0, 0, 0, 1,
    0, 1, 0, 0, 0,    0, 0, 1, 0, 0,
    0, 0, 0, 1, 0,    0, 0, 1, 0, 0,
    0, 0, 0, 1, 0,    0, 0, 0, 0, 1,
    0, 1, 0, 0, 0,    0, 0, 0, 0, 0,
    0, 1, 0, 1, 0,    0, 0, 1, 0, 1,

    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,
    0, 1, 0, 0, 0,    0, 0, 0, 0, 0,
    0, 0, 0, 1, 0,    0, 0, 1, 0, 1,
    0, 0, 0, 1, 0,    0, 0, 0, 0, 1,
    0, 1, 0, 0, 0,    0, 0, 0, 0, 0,
    0, 1, 0, 0, 0,    0
  };

  for (int i = 2; i <= 255; ++i)
  {
    CHECK(Dg::IsPrime(uint8_t(i)) == (isPrime[i] == 1));
  }

  CHECK(Dg::IsPrime(uint32_t(0xFFFFFFFB)));
}

TEST(Stack_math_HighestBit, creation_math_HighestBit)
{
  CHECK(Dg::HighestBit<uint32_t>(0) == 0);
  CHECK(Dg::HighestBit<uint32_t>(1) == 1);
  CHECK(Dg::HighestBit<uint32_t>(2) == 2);
  CHECK(Dg::HighestBit<uint32_t>(3) == 2);
  CHECK(Dg::HighestBit<uint32_t>(4) == 3);
  CHECK(Dg::HighestBit<uint32_t>(5) == 3);
  CHECK(Dg::HighestBit<uint32_t>(6) == 3);
  CHECK(Dg::HighestBit<uint32_t>(7) == 3);
  CHECK(Dg::HighestBit<uint32_t>(8) == 4);
  CHECK(Dg::HighestBit<uint32_t>(9) == 4);
  CHECK(Dg::HighestBit<uint32_t>(234) == 8);
  CHECK(Dg::HighestBit<uint32_t>(65535) == 16);
}

TEST(Stack_math_ClosestSquare, creation_math_ClosestSquare)
{
  typedef uint32_t UINT;
  UINT lower, upper;

  Dg::ClosestSquare<UINT>(0, lower, upper);
  CHECK(lower == 0, upper == 0);

  Dg::ClosestSquare<UINT>(1, lower, upper);
  CHECK(lower == 1, upper == 1);

  Dg::ClosestSquare<UINT>(2, lower, upper);
  CHECK(lower == 1, upper == 2);

  Dg::ClosestSquare<UINT>(3, lower, upper);
  CHECK(lower == 2, upper == 2);

  Dg::ClosestSquare<UINT>(4, lower, upper);
  CHECK(lower == 2, upper == 2);

  Dg::ClosestSquare<UINT>(5, lower, upper);
  CHECK(lower == 2, upper == 3);

  Dg::ClosestSquare<UINT>(6, lower, upper);
  CHECK(lower == 2, upper == 3);

  Dg::ClosestSquare<UINT>(7, lower, upper);
  CHECK(lower == 3, upper == 3);

  Dg::ClosestSquare<UINT>(8, lower, upper);
  CHECK(lower == 3, upper == 3);

  Dg::ClosestSquare<UINT>(9, lower, upper);
  CHECK(lower == 3, upper == 3);

  Dg::ClosestSquare<UINT>(10, lower, upper);
  CHECK(lower == 3, upper == 4);

  Dg::ClosestSquare<UINT>(11, lower, upper);
  CHECK(lower == 3, upper == 4);

  Dg::ClosestSquare<UINT>(12, lower, upper);
  CHECK(lower == 3, upper == 4);

  Dg::ClosestSquare<UINT>(13, lower, upper);
  CHECK(lower == 4, upper == 4);

  Dg::ClosestSquare<UINT>(14, lower, upper);
  CHECK(lower == 4, upper == 4);

  Dg::ClosestSquare<UINT>(15, lower, upper);
  CHECK(lower == 4, upper == 4);

  Dg::ClosestSquare<UINT>(16, lower, upper);
  CHECK(lower == 4, upper == 4);

  ///
  Dg::ClosestSquare<UINT>(24, lower, upper);
  CHECK(lower == 5, upper == 5);

  Dg::ClosestSquare<UINT>(34, lower, upper);
  CHECK(lower == 6, upper == 6);

  Dg::ClosestSquare<UINT>(73, lower, upper);
  CHECK(lower == 9, upper == 9);

  Dg::ClosestSquare<UINT>(91, lower, upper);
  CHECK(lower == 10, upper == 10);

  Dg::ClosestSquare<UINT>(65537, lower, upper);
  CHECK(lower == 256, upper == 257);
}