#include "TestHarness.h"

#include "dgmath.h"



TEST(Stack_math, creation_math)
{
  
  uint32_t r_Log2 = Dg::Log2(0x1FFFF);
  CHECK((r_Log2 == 16));

}