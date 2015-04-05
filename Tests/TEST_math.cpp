#include "TestHarness.h"

#include "dgmath.h"



TEST(Stack_math, creation_math)
{
  
  uint32_t r_Log2 = Dg::Log2(0x1FFFF);
  CHECK((r_Log2 == 16));

  CHECK(Dg::AreEqual(Dg::IntegrateND(-5.9), 3.595204891775254144926e-17));
  CHECK(Dg::AreEqual(Dg::IntegrateND(-4.32), 5.00023857028464687129e-10));
  CHECK(Dg::AreEqual(Dg::IntegrateND(-3.001), 0.000010975830458868));
  CHECK(Dg::AreEqual(Dg::IntegrateND(-2.8999), 0.000020561503237575));
  CHECK(Dg::AreEqual(Dg::IntegrateND(-1.09999), 0.05989914762945457));
  CHECK(Dg::AreEqual(Dg::IntegrateND(-0.132), 0.42595726267958));
  CHECK(Dg::AreEqual(Dg::IntegrateND(0.0), 0.5));
  CHECK(Dg::AreEqual(Dg::IntegrateND(1.355), 0.97233395557520));
  CHECK(Dg::AreEqual(Dg::IntegrateND(2.79), 0.99996020907374733));
  CHECK(Dg::AreEqual(Dg::IntegrateND(3.101), 0.99999421338590));
  CHECK(Dg::AreEqual(Dg::IntegrateND(4.38), 0.999999999707193));
  CHECK(Dg::AreEqual(Dg::IntegrateND(5.99), 0.9999999999999999878));
}