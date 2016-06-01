#include "TestHarness.h"
#include "DgLine.h"

typedef double Real;
typedef Dg::Vector4<Real> vec;
typedef Dg::Plane<Real>   plane;
typedef Dg::Line<Real>    line;

TEST(Stack_DgLine, DgLine)
{
  vec origin = vec::Origin();
  vec direction = vec::xAxis();

  line l0 = line(origin, direction);

  vec o, d;
  l0.Get(o, d);
  CHECK(o == l0.Origin() && d == l0.Direction());

  line l1 = l0;
  line l2(l1);
  CHECK(l1 == l0);
  CHECK(l2 == l0);

  CHECK(l0.ClosestPoint(vec(7.0, -34.5, 90.53, 1.0)) == vec(7.0, 0.0, 0.0, 1.0));

  //Geometric tests

  //Line-Line
  vec p0, p1;
  int result = 0;

  //Lines parallel
  l1.Set(vec(1.0, 1.0, 0.0, 1.0), vec(1.0, 0.0, 0.0, 0.0));
  result = ClosestPointsLineLine(l0, l1, p0, p1);
  CHECK(result == 1);
  CHECK(p0 == vec(0.0, 0.0, 0.0, 1.0));
  CHECK(p1 == vec(0.0, 1.0, 0.0, 1.0));

  //Lines not parallel
  l1.Set(vec(1.0, 1.0, 0.0, 1.0), vec(0.0, 0.0, 1.0, 0.0));
  result = ClosestPointsLineLine(l0, l1, p0, p1);
  CHECK(result == 0);
  CHECK(p0 == vec(1.0, 0.0, 0.0, 1.0));
  CHECK(p1 == vec(1.0, 1.0, 0.0, 1.0));

  //Line-Plane
  plane pl;

  //Line parallel to plane
  pl.Set(vec(0.0, 1.0, 0.0, 0.0), -1.0);
  result = TestPlaneLine(pl, l0, p0);
  CHECK(result == 2);

  //Line on plane
  pl.Set(vec(0.0, 1.0, 0.0, 0.0), 0.0);
  result = TestPlaneLine(pl, l0, p0);
  CHECK(result == 1);

  //Line intersecting plane
  pl.Set(vec(1.0, 0.0, 0.0, 0.0), -1.0);
  result = TestPlaneLine(pl, l0, p0);
  CHECK(result == 0);
  CHECK(p0 == vec(1.0, 0.0, 0.0, 1.0));

}