#include <stdint.h>

#include "TestHarness.h"
#include "Dg_QuadTree.h"

typedef Dg::R2::Vector<float>                  vec2;
typedef Dg::QuadTree<float, uint32_t, int>     QT15;
typedef Dg::QuadTree<float, uint32_t, int, 3>  QT3;

TEST(Stack_DgQuadTree, DgQuadtree)
{
  QT15::AABB aabb(vec2::Origin(), 1.f, 1.f);
  QT15 qt15(aabb);

  qt15.Subdivide(qt15.Root());
  QT15::Handle h(0);
  Dg::ErrorCode error = qt15.QueryPoint(0.2f, 0.2f, h);

  CHECK(error == Dg::ErrorCode::None);
  CHECK(h == 7);
}