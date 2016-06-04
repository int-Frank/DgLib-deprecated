#include "TestHarness.h"
#include "DgRay.h"

typedef double Real;
typedef Dg::Vector4<Real> vec;
typedef Dg::Plane<Real>   plane;
typedef Dg::Line<Real>    line;
typedef Dg::Ray<Real>     ray;

TEST(Stack_DgRay, DgRay)
{
  vec origin = vec::Origin();
  vec direction = vec::xAxis();

  ray r0 = ray(origin, direction);

  vec o, d;
  r0.Get(o, d);
  CHECK(o == r0.Origin() && d == r0.Direction());

  ray r1 = r0;
  ray r2(r1);
  CHECK(r1 == r0);
  CHECK(r2 == r0);

  //Geometric tests

  //Ray-Point
  Real u = 0.0;
  vec pOut;
  vec pIn;
  int result;

  //Point in front of ray
  pIn.Set(7.0, -34.5, 90.53, 1.0);
  result = r0.ClosestPoint(pIn, pOut, u);
  CHECK(result == 0);
  CHECK(u == 7.0);
  CHECK(pOut == vec(7.0, 0.0, 0.0, 1.0));

  //Point behind ray
  pIn.Set(-7.0, -34.5, 90.53, 1.0);
  result = r0.ClosestPoint(pIn, pOut, u);
  CHECK(result == 0);
  CHECK(u == 0.0);
  CHECK(pOut == r0.Origin());

  //Ray - Ray
  vec p0, p1;
  Real u0 = 0.0, u1 = 0.0;

  //Rays parallel, no overlap, facing opposite directions
  r1.Set(vec(-2.0, 1.0, 0.0, 1.0), vec(-1.0, 0.0, 0.0, 0.0));
  result = ClosestPointsRayRay(r0, r1, u0, u1, p0, p1);
  CHECK(result == 1);
  CHECK(u0 == 0.0);
  CHECK(u1 == 0.0);
  CHECK(p0 == r0.Origin());
  CHECK(p1 == r1.Origin());

  //Rays parallel, overlap, facing opposite directions
  r1.Set(vec(2.0, 1.0, 0.0, 1.0), vec(-1.0, 0.0, 0.0, 0.0));
  result = ClosestPointsRayRay(r0, r1, u0, u1, p0, p1);
  CHECK(result == 1);
  CHECK(u0 == 0.0);
  CHECK(u1 == 2.0);
  CHECK(p0 == r0.Origin());
  CHECK(p1 == vec(0.0, 1.0, 0.0, 1.0));

  //Rays parallel, overlap, facing same direction
  r1.Set(vec(2.0, 1.0, 0.0, 1.0), vec(1.0, 0.0, 0.0, 0.0));
  result = ClosestPointsRayRay(r0, r1, u0, u1, p0, p1);
  CHECK(result == 1);
  CHECK(u0 == 2.0);
  CHECK(u1 == 0.0);
  CHECK(p0 == vec(2.0, 0.0, 0.0, 1.0));
  CHECK(p1 == r1.Origin());

  //Rays parallel, overlap, facing same direction, switch rays
  result = ClosestPointsRayRay(r1, r0, u1, u0, p1, p0);
  CHECK(result == 1);
  CHECK(u0 == 2.0);
  CHECK(u1 == 0.0);
  CHECK(p0 == vec(2.0, 0.0, 0.0, 1.0));
  CHECK(p1 == r1.Origin());

  //Rays not parallel
  r1.Set(vec(2.0, 2.0, 2.0, 1.0), vec(0.0, 0.0, -1.0, 0.0));
  result = ClosestPointsRayRay(r0, r1, u0, u1, p0, p1);
  CHECK(result == 0);
  CHECK(u0 == 2.0);
  CHECK(u1 == 2.0);
  CHECK(p0 == vec(2.0, 0.0, 0.0, 1.0));
  CHECK(p1 == vec(2.0, 2.0, 0.0, 1.0));

  //Line - Ray
  line ln;
  Real ul = 0.0, ur = 0.0;
  vec pr, pl;

  //Line-Ray Parallel, same direction
  ln.Set(vec(5.0, 5.0, 5.0, 1.0), vec(1.0, 0.0, 0.0, 0.0));
  result = ClosestPointsRayLine(r0, ln, ur, ul, pr, pl);
  CHECK(result == 1);
  CHECK(ur == 0.0);
  CHECK(ul == -5.0);
  CHECK(pr == r0.Origin());
  CHECK(pl == vec(0.0, 5.0, 5.0, 1.0));

  //Line-Ray Parallel, opposite directions
  ln.Set(vec(5.0, 5.0, 5.0, 1.0), vec(-1.0, 0.0, 0.0, 0.0));
  result = ClosestPointsRayLine(r0, ln, ur, ul, pr, pl);
  CHECK(result == 1);
  CHECK(ur == 0.0);
  CHECK(ul == 5.0);
  CHECK(pr == r0.Origin());
  CHECK(pl == vec(0.0, 5.0, 5.0, 1.0));

  //Line-Ray not parallel, closest point is ray origin
  ln.Set(vec(-5.0, 1.0, -2.0, 1.0), vec(0.0, 0.0, -1.0, 0.0));
  result = ClosestPointsRayLine(r0, ln, ur, ul, pr, pl);
  CHECK(result == 0);
  CHECK(ur == 0.0);
  CHECK(ul == -2.0);
  CHECK(pr == r0.Origin());
  CHECK(pl == vec(-5.0, 1.0, 0.0, 1.0));

  //Line-Ray not parallel, closest point is along the ray
  ln.Set(vec(5.0, 5.0, 5.0, 1.0), vec(0.0, 0.0, -1.0, 0.0));
  result = ClosestPointsRayLine(r0, ln, ur, ul, pr, pl);
  CHECK(result == 0);
  CHECK(ur == 5.0);
  CHECK(ul == 5.0);
  CHECK(pr == vec(5.0, 0.0, 0.0, 1.0));
  CHECK(pl == vec(5.0, 5.0, 0.0, 1.0));

  //Ray-Plane
  plane pln;

  //Ray not intersecting plane, pointing away from plane
  pln.Set(vec(-1.0, 0.0, 0.0, 0.0), vec(-1.0, 0.0, 0.0, 1.0));
  result = TestPlaneRay(pln, r0, ur, pr);
  CHECK(result == 3);
  CHECK(ur == 0.0);
  CHECK(pr == r0.Origin());

  //Ray not intersecting plane, parallel to plane
  pln.Set(vec(0.0, 1.0, 0.0, 0.0), vec(0.0, 1.0, 0.0, 1.0));
  result = TestPlaneRay(pln, r0, ur, pr);
  CHECK(result == 2);
  CHECK(ur == 0.0);
  CHECK(pr == r0.Origin());

  //Ray lies on the plane
  pln.Set(vec(0.0, 1.0, 0.0, 0.0), vec(0.0, 0.0, 0.0, 1.0));
  result = TestPlaneRay(pln, r0, ur, pr);
  CHECK(result == 1);
  CHECK(ur == 0.0);
  CHECK(pr == vec(0.0, 0.0, 0.0, 1.0));

  //Ray intersects plane
  pln.Set(vec(1.0, 0.0, 0.0, 0.0), vec(3.0, 0.0, 0.0, 1.0));
  result = TestPlaneRay(pln, r0, ur, pr);
  CHECK(result == 0);
  CHECK(ur == 3.0);
  CHECK(pr == vec(3.0, 0.0, 0.0, 1.0));

  //Ray intersects plane
  r0.Set(vec(6.0, 2.3, -5.6, 1.0), vec::xAxis());
  pln.Set(vec::xAxis(), vec(10.0, 0.0, 0.0, 1.0));
  result = TestPlaneRay(pln, r0, ur, pr);
  CHECK(result == 0);
  CHECK(ur == 4.0);
  CHECK(pr == vec(10.0, 2.3, -5.6, 1.0));

}