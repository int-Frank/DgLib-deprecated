#include "TestHarness.h"
#include "DgR2AABB.h"

typedef Dg::R2::Vector<float>     vec;
typedef Dg::R2::AABB<float>       AABB;

TEST(Stack_DgR2AABB, DgR2AABB)
{
  AABB aabb;
  float hx(0.f), hy(0.f);
  aabb.GetHalfLengths(hx, hy);
  CHECK(aabb.GetCenter() == vec::Origin());
  CHECK(hx == 0.5f);
  CHECK(hy == 0.5f);

  vec center(1.f, -23.f, 1.f);
  float x = 4.3f;
  float y = 6.7f;

  AABB aabbX(center, x, y);
  aabbX.GetHalfLengths(hx, hy);
  CHECK(aabbX.GetCenter() == center);
  CHECK(hx == x);
  CHECK(hy == y);

  aabb.SetCenter(center);
  aabb.SetHalfLengths(x, y);

  AABB aabb1(aabb);
  aabb1.GetHalfLengths(hx, hy);
  CHECK(aabb1.GetCenter() == center);
  CHECK(hx == x);
  CHECK(hy == y);

  aabb1 = aabb;
  aabb1.GetHalfLengths(hx, hy);
  CHECK(aabb1.GetCenter() == center);
  CHECK(hx == x);
  CHECK(hy == y);
}