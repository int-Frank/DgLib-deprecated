#include "TestHarness.h"
#include "DgLineSegment.h"

typedef double Real;
typedef Dg::Vector4<Real> vec;
typedef Dg::Plane<Real>   plane;
typedef Dg::Line<Real>    line;
typedef Dg::Ray<Real>     ray;
typedef Dg::LineSegment<Real> lineSeg;

TEST(Stack_DgLineSegment, DgLineSegment)
{
  vec origin = vec::Origin();
  vec direction = vec::xAxis() * 5.0;

  lineSeg ls0 = lineSeg(origin, direction);

  CHECK(Dg::AreEqual(ls0.Length(), 5.0));
  CHECK(Dg::AreEqual(ls0.LengthSquared(), 25.0));

  vec p0, p1;
  ls0.Get(p0, p1);
  CHECK(p0 == vec::Origin() && p1 == vec(5.0, 0.0, 0.0, 1.0));
  CHECK(ls0.GetP0() == vec::Origin());
  CHECK(ls0.GetP1() == vec(5.0, 0.0, 0.0, 1.0));
  CHECK(ls0.GetCenter() == vec(2.5, 0.0, 0.0, 1.0));

  lineSeg ls1 = ls0;
  lineSeg ls2(ls1);
  CHECK(ls1 == ls0);
  CHECK(ls2 == ls0);

  //Geometric tests

  //lineSeg-point
  ls0.Set(vec(2.0, 0.0, 0.0, 1.0), vec(6.0, 0.0, 0.0, 1.0));
  Real uls = 0.0;
  vec pIn, pOut;
  int result;

  //Point behind p0
  pIn.Set(-1.0, -1.8, -2.9, 1.0);
  result = ls0.ClosestPoint(pIn, pOut, uls);
  CHECK(result == 0);
  CHECK(uls == 0.0);
  CHECK(pOut == ls0.GetP0());

  //Point behind p1
  pIn.Set(10.0, 11.8, -12.9, 1.0);
  result = ls0.ClosestPoint(pIn, pOut, uls);
  CHECK(result == 0);
  CHECK(uls == 1.0);
  CHECK(pOut == ls0.GetP1());

  //Closest point along lineSeg
  pIn.Set(3.0, 11.8, -12.9, 1.0);
  result = ls0.ClosestPoint(pIn, pOut, uls);
  CHECK(result == 0);
  CHECK(uls == 0.25);
  CHECK(pOut == vec(3.0, 0.0, 0.0, 1.0));

  //lineSeg-Line
  line l;
  vec pls, pl;
  Real ul;

  //LineSeg parallel to line
  l.Set(vec(1.0, 1.0, 1.0, 1.0), vec::xAxis());
  result = ClosestPointsLineSegmentLine(ls0, l, uls, ul, pls, pl);
  CHECK(result == 1);
  CHECK(uls == 0.0);
  CHECK(ul == 1.0);
  CHECK(pls == ls0.GetP0());
  CHECK(pl == vec(2.0, 0.0, 0.0, 1.0));

  //LineSeg parallel to line, opposite direction
  //lineSeg-p0 closest point
  //lineSeg-p1 closest point
  //Closest point along lineSeg

  //lineSeg-Ray

  //LineSeg parallel to ray, but behind ray origin
  //LineSeg parallel to ray, but behind ray origin, switch LineSeg direction
  //LineSeg parallel to ray, p0 behind ray origin, p1 along ray
  //LineSeg parallel to ray, p1 behind ray origin, p0 along ray
  //LineSeg parallel to ray, completely in front of ray, p0 closer to ray origin
  //LineSeg parallel to ray, completely in front of ray, p1 closer to ray origin
  //LineSeg not parallel, behind ray, Closest points are ls-p0, ray-origin
  //LineSeg not parallel, behind ray, Closest points are ls-p1, ray-origin
  //LineSeg not parallel, behind ray, Closest points are along the ls, ray-origin
  //LineSeg in front of ray, closest point p0
  //LineSeg in front of ray, closest point p1
  //LineSeg in front of ray, closest point along ls

  //lineSeg-LineSeg

  //LineSegs parallel, no overlap, closest points ls0-p0, ls1-p0
  //LineSegs parallel, no overlap, closest points ls0-p0, ls1-p1
  //LineSegs parallel, no overlap, closest points ls0-p1, ls1-p0
  //LineSegs parallel, no overlap, closest points ls0-p1, ls1-p1
  //LineSegs parallel, overlap, ls0p0---ls1p0---ls0p1---ls1p1
  //LineSegs parallel, overlap, ls0p1---ls1p0---ls0p0---ls1p1
  //LineSegs parallel, overlap, ls0p0---ls1p0---ls1p1---ls0p1
  //LineSegs parallel, overlap, ls0p1---ls1p0---ls1p1---ls0p0
  //LineSegs not parallel, closest points: ls0p0, ls1p0
  //LineSegs not parallel, closest points: ls0p0, ls1p1
  //LineSegs not parallel, closest points: ls0p1, ls1p0
  //LineSegs not parallel, closest points: ls0p1, ls1p1
  //LineSegs not parallel, closest points: ls0p0, ls1-along ls
  //LineSegs not parallel, closest points: ls0p1, ls1-along ls
  //LineSegs not parallel, closest points: ls0-along ls, ls1-along ls

  //LineSeg-plane

  //LineSeg parallel to plane
  //LineSeg on plane
  //LineSeg not parallel to plane, not intersecting
  //LineSeg not parallel to plane, intersecting
  //LineSeg not parallel to plane, intersecting, switch points
}