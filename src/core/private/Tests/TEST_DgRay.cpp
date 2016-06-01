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

  //Closest point
  Real u = 0.0;
  vec pOut;
  vec pIn(7.0, -34.5, 90.53, 1.0);
  int result = r0.ClosestPoint(pIn, pOut, u);

  CHECK(result == 0);
  CHECK(u == 7.0);
  CHECK(pOut == vec(7.0, 0.0, 0.0, 1.0));

  pIn.Set(-7.0, -34.5, 90.53, 1.0);
  result = r0.ClosestPoint(pIn, pOut, u);

  CHECK(result == 1);
  CHECK(u == 0.0);
  CHECK(pOut == vec(0.0, 0.0, 0.0, 1.0));

  //Geometric tests

  //Ray - Ray

  //Rays parallel, no overlap, facing opposite directions

  //Rays parallel, overlap, facing opposite directions

  //Rays parallel, overlap, facing same direction
  //Rays parallel, overlap, facing same direction, switch rays

  //Rays not parallel

  //Line - Ray

  //Line-Ray Parallel, same direction
  //Line-Ray Parallel, opposite directions

  //Line-Ray not parallel, closest point is ray origin
  //Line-Ray not parallel, closest point is along the ray

  //Ray-Plane

  //Ray not intersecting plane, pointing away from plane

  //Ray not intersecting plane, parallel to plane

  //Ray intersects plane

}