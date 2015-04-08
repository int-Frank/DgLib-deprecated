#include "TestHarness.h"

#include "dgmath.h"

using namespace Dg;

TEST(Stack_math, creation_math)
{
  /// Log ////////////////////////////////////////////////////////////////

  uint32_t r_Log2 = Log2(0x1FFFF);
  CHECK((r_Log2 == 16));

  /// Wrap ////////////////////////////////////////////////////////////////

  double lower = -2.0;
  double upper = 3.0;
  double val = 0.0;

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

}