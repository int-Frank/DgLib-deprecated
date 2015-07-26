#include "TestHarness.h"

#include "ResourceKey.h"

using namespace Dg;

TEST(Stack_ResourceKey, creation_ResourceKey)
{
  Dg::RKey key(0x9AEB5C39);
  uint32_t result = key.GetBitSet<4, 4>();
  CHECK(result == 0x3);

  result = key.GetBitSet<10, 7>();
  CHECK(result == 0x57);

  result = key.GetBitSet<0, 32>();
  CHECK(result == 0x9AEB5C39);

  result = key.GetBitSet<31, 1>();
  CHECK(result == 1);

  Dg::RKey key2(key);
  key2.SetBitSet<4, 5>(0x1A);
  result = key2.GetBitSet<4, 5>();
  CHECK(result = 0x1A);

  key2 = key;
  uint32_t saved = key2.GetBitSet<10, 4>();
  key2.SetBitSet<10, 4>(0x496);
  result = key2.GetBitSet<10, 4>();
  key2.SetBitSet<10, 4>(saved);
  CHECK(result = 0x69);
  CHECK(key2 == key);

}