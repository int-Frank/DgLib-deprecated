#include "TestHarness.h"
#include "DgR3Sphere.h"
#include "query/DgR3QueryLineSphere.h"

typedef double                  Real;
typedef Dg::R3::Vector<Real>       vec;
typedef Dg::R3::Line<Real>          line;
typedef Dg::R3::Sphere<Real>        sphere;

TEST(Stack_DgSphere, DgSphere)
{
  vec center(2.0, 6.0, 9.0, 1.0);
  Real radius = 4.0;

  sphere s = sphere(center, radius);
  CHECK(s.Center() == center);
  CHECK(s.Radius() == radius);

  vec c;
  Real r(0.0);
  s.Get(c, r);
  CHECK(c == center);
  CHECK(r == radius);

  sphere s0 = s;
  sphere s1(s0);
  CHECK(s1 == s);
  CHECK(!(s1 != s));

  s.SetCenter(center);
  s.SetRadius(radius);
  CHECK(s.Center() == center);
  CHECK(s.Radius() == radius);

  s.Set(center, radius);
  CHECK(s.Center() == center);
  CHECK(s.Radius() == radius);
}