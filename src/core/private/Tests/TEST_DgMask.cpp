#include "TestHarness.h"
#include "DgMask.h"


TEST(Stack_DgMask, creation_DgMask)
{
  Dg::Mask<7> mask7;
  Dg::Mask<8> mask8;
  Dg::Mask<9> mask9;
  Dg::Mask<26> mask26;

  CHECK(sizeof(mask7) == 1);
  CHECK(sizeof(mask8) == 1);
  CHECK(sizeof(mask9) == 2);
  CHECK(sizeof(mask26) == 4);

  for (int i = 0; i < 26; i++)
  {
    CHECK(!mask26.IsOn(i));
  }

  mask26.SetOn(0);
  mask26.SetOn(3);
  mask26.SetOn(9);
  mask26.SetOn(20);
  mask26.SetOn(25);

  Dg::Mask<26> mask26_a(mask26);
  Dg::Mask<26> mask26_b = mask26_a;
  CHECK(mask26_b == mask26);
  CHECK(!(mask26_b != mask26_a));

  CHECK(mask26.IsOn(0));
  CHECK(mask26.IsOn(3));
  CHECK(mask26.IsOn(9));
  CHECK(mask26.IsOn(20));
  CHECK(mask26.IsOn(25));

  mask26.SetOff(0);
  mask26.SetOff(3);
  mask26.SetOff(9);
  mask26.SetOff(20);
  mask26.SetOff(25);

  CHECK(!mask26.IsOn(0));
  CHECK(!mask26.IsOn(3));
  CHECK(!mask26.IsOn(9));
  CHECK(!mask26.IsOn(20));
  CHECK(!mask26.IsOn(25));
}