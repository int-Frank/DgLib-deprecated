#include "TestHarness.h"
#include "DgR2AABB.h"
#include "DgR2Vector.h"

typedef Dg::R2::Vector<float>     vec;
typedef Dg::R2::AABB<float>       AABB;

TEST(Stack_DgR2AABB, DgR2AABB)
{
  AABB aabb;
  float hl[2];
  aabb.GetHalfLengths(hl);
  CHECK(aabb.GetCenter() == vec::Origin());
  CHECK(hl[0] == 0.5f);
  CHECK(hl[1] == 0.5f);

  vec center(1.f, -23.f, 1.f);
  float hl2[2] = { 4.3f, 6.7f };

  AABB aabbX(center, hl2);
  aabbX.GetHalfLengths(hl);
  CHECK(aabbX.GetCenter() == center);
  CHECK(hl[0] == hl2[0]);
  CHECK(hl[1] == hl2[1]);

  aabb.SetCenter(center);
  aabb.SetHalfLengths(hl2);

  AABB aabb1(aabb);
  aabb1.GetHalfLengths(hl);
  CHECK(aabb1.GetCenter() == center);
  CHECK(hl[0] == hl2[0]);
  CHECK(hl[1] == hl2[1]);

  aabb1 = aabb;
  aabb1.GetHalfLengths(hl);
  CHECK(aabb1.GetCenter() == center);
  CHECK(hl[0] == hl2[0]);
  CHECK(hl[1] == hl2[1]);
}