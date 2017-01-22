#include "TestHarness.h"
#include "DgR3Triangle.h"
#include "query/DgR3QueryRayTriangle.h"

typedef double                  Real;
typedef Dg::R3::Vector<Real>    vec;
typedef Dg::R3::Ray<Real>       ray;
typedef Dg::R3::Triangle<Real>  triangle;

TEST(Stack_DgTriangle, DgTriangle)
{
  vec p0(1.0, -1.0, 0.0, 1.0);
  vec p1(1.0, 1.0,  0.0, 1.0);
  vec p2(1.0, 0.0, 1.0, 1.0);
  triangle tri(p0, p1, p2);

  Dg::R3::TIRayTriangle<Real> tiRayTriangle;
  Dg::R3::TIRayTriangle<Real>::Result tiRayTriangle_res;

  vec ro(0.0, 0.0, -1.0, 1.0);
  vec rd;

  ray r(ro, vec::xAxis());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);

  r = ray(ro, -vec::xAxis());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);

  r = ray(ro, vec::yAxis());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);

  r = ray(ro, -vec::yAxis());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);

  r = ray(ro, vec::zAxis());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);

  r = ray(ro, -vec::zAxis());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);

  r = ray(vec::Origin(), vec::xAxis());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == true);
  CHECK(tiRayTriangle_res.u == 1.0);

  r = ray(vec(0.0, 0.0, 0.2, 1.0), vec::xAxis());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == true);
  CHECK(tiRayTriangle_res.u == 1.0);

  r = ray(vec(0.0, 0.0, 0.2, 1.0), -vec::xAxis());
  tiRayTriangle_res = tiRayTriangle(r, tri);
  CHECK(tiRayTriangle_res.isIntersecting == false);
}