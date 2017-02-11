#include "TestHarness.h"
#include "DgR2Polygon.h"

typedef float Real;
typedef Dg::R2::Vector<Real>  vec;
typedef Dg::R2::Polygon<Real> polygon;

TEST(Stack_DgR2Polygon, creation_DgR2Polygon)
{
  polygon poly;

  CHECK(poly.Orientation() == Dg::Orientation::Colinear);

  poly.push_back(vec(1.f, 1.f, 1.f));
  CHECK(poly.Orientation() == Dg::Orientation::Colinear);

  poly.push_back(vec(1.f, -1.f, 1.f));
  CHECK(poly.Orientation() == Dg::Orientation::Colinear);

  poly.push_back(vec(-1.f, -1.f, 1.f));
  CHECK(poly.Orientation() == Dg::Orientation::CW);

  poly.push_back(vec(-1.f, 1.f, 1.f));
  CHECK(poly.Orientation() == Dg::Orientation::CW);

  CHECK(poly.Area() == 4.f);
  CHECK(poly.Centroid() == vec::Origin());

  poly.clear();
  CHECK(poly.Orientation() == Dg::Orientation::Colinear);

  poly.push_back(vec(1.f, 1.f, 1.f));
  CHECK(poly.Orientation() == Dg::Orientation::Colinear);

  poly.push_back(vec(-1.f, 1.f, 1.f));
  CHECK(poly.Orientation() == Dg::Orientation::Colinear);

  poly.push_back(vec(-1.f, -1.f, 1.f));
  CHECK(poly.Orientation() == Dg::Orientation::CCW);

  poly.push_back(vec(1.f, -1.f, 1.f));
  CHECK(poly.Orientation() == Dg::Orientation::CCW);

  CHECK(poly.Area() == 4.f);
  CHECK(poly.Centroid() == vec::Origin());
}
