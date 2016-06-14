#include "TestHarness.h"
#include "DgRay.h"
#include "query/DgQueryPointRay.h"
#include "query/DgQueryRayLine.h"
#include "query/DgQueryRayRay.h"
#include "query/DgQueryRayPlane.h"

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
  Dg::DCPPointRay<Real> dcpPointRay;
  Dg::DCPPointRay<Real>::Result dcpPointRay_res;
  vec pIn;

  //Point in front of ray
  pIn.Set(7.0, -12.0, 35.0, 1.0);
  dcpPointRay_res = dcpPointRay(pIn, r0);
  CHECK(dcpPointRay_res.u == 7.0);
  CHECK(dcpPointRay_res.distance = 37.0);
  CHECK(dcpPointRay_res.sqDistance = 37.0 * 37.0);
  CHECK(dcpPointRay_res.cp == vec(7.0, 0.0, 0.0, 1.0));

  //Point behind ray
  pIn.Set(-2.0, -3.0, 6.0, 1.0);
  dcpPointRay_res = dcpPointRay(pIn, r0);
  CHECK(dcpPointRay_res.u == 0.0);
  CHECK(dcpPointRay_res.distance = 7.0);
  CHECK(dcpPointRay_res.sqDistance = 49.0);
  CHECK(dcpPointRay_res.cp == r0.Origin());

  //Ray - Ray
  Dg::DCPRayRay<Real> dcpRayRay;
  Dg::DCPRayRay<Real>::Result dcpRayRay_res;

  //Rays parallel, no overlap, facing opposite directions
  r1.Set(vec(-3.0, 8.0, 36.0, 1.0), vec(-1.0, 0.0, 0.0, 0.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == 0);
  CHECK(dcpRayRay_res.u0 == 0.0);
  CHECK(dcpRayRay_res.u1 == 0.0);
  CHECK(dcpRayRay_res.cp0 == r0.Origin());
  CHECK(dcpRayRay_res.cp1 == r1.Origin());
  CHECK(dcpRayRay_res.distance == 37.0);
  CHECK(dcpRayRay_res.sqDistance == 37.0 * 37.0);

  //Rays parallel, overlap, facing opposite directions
  r1.Set(vec(4.0, 13.0, 84.0, 1.0), vec(-1.0, 0.0, 0.0, 0.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == 1);
  CHECK(dcpRayRay_res.u0 == 0.0);
  CHECK(dcpRayRay_res.u1 == 4.0);
  CHECK(dcpRayRay_res.cp0 == r0.Origin());
  CHECK(dcpRayRay_res.cp1 == vec(0.0, 13.0, 84.0, 1.0));
  CHECK(dcpRayRay_res.distance == 85.0);
  CHECK(dcpRayRay_res.sqDistance == 85.0 * 85.0);

  //Rays parallel, overlap, facing same direction
  r1.Set(vec(4.0, 3.0, 4.0, 1.0), vec(1.0, 0.0, 0.0, 0.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == 1);
  CHECK(dcpRayRay_res.u0 == 4.0);
  CHECK(dcpRayRay_res.u1 == 0.0);
  CHECK(dcpRayRay_res.cp0 == vec(4.0, 0.0, 0.0, 1.0));
  CHECK(dcpRayRay_res.cp1 == r1.Origin());
  CHECK(dcpRayRay_res.distance == 5.0);
  CHECK(dcpRayRay_res.sqDistance == 25.0);

  //Rays parallel, overlap, facing same direction, switch rays
  r1.Set(vec(4.0, 3.0, 4.0, 1.0), vec(1.0, 0.0, 0.0, 0.0));
  dcpRayRay_res = dcpRayRay(r1, r0);
  CHECK(dcpRayRay_res.code == 1);
  CHECK(dcpRayRay_res.u1 == 4.0);
  CHECK(dcpRayRay_res.u0 == 0.0);
  CHECK(dcpRayRay_res.cp1 == vec(4.0, 0.0, 0.0, 1.0));
  CHECK(dcpRayRay_res.cp0 == r1.Origin());
  CHECK(dcpRayRay_res.distance == 5.0);
  CHECK(dcpRayRay_res.sqDistance == 25.0);

  //Rays not parallel, cp are along r0, r1.Origin.
  r1.Set(vec(3.0, 4.0, 3.0, 1.0), vec(0.0, 0.0, 1.0, 0.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == 0);
  CHECK(dcpRayRay_res.u0 == 3.0);
  CHECK(dcpRayRay_res.u1 == 0.0);
  CHECK(dcpRayRay_res.cp0 == vec(3.0, 0.0, 0.0, 1.0));
  CHECK(dcpRayRay_res.cp1 == r1.Origin());
  CHECK(dcpRayRay_res.distance == 5.0);
  CHECK(dcpRayRay_res.sqDistance == 25.0);

  //Rays not parallel, cp are along both rays
  r1.Set(vec(3.0, 4.0, 3.0, 1.0), vec(0.0, 0.0, -1.0, 0.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == 0);
  CHECK(dcpRayRay_res.u0 == 3.0);
  CHECK(dcpRayRay_res.u1 == 3.0);
  CHECK(dcpRayRay_res.cp0 == vec(3.0, 0.0, 0.0, 1.0));
  CHECK(dcpRayRay_res.cp1 == vec(3.0, 4.0, 0.0, 1.0));
  CHECK(dcpRayRay_res.distance == 4.0);
  CHECK(dcpRayRay_res.sqDistance == 16.0);

  //Line - Ray
  line ln;
  Dg::DCPRayLine<Real> dcpRayLine;
  Dg::DCPRayLine<Real>::Result dcpRayLine_res;

  //Line-Ray Parallel, same direction
  ln.Set(vec(5.0, 11.0, 60.0, 1.0), vec(1.0, 0.0, 0.0, 0.0));
  dcpRayLine_res = dcpRayLine(r0, ln);
  CHECK(dcpRayLine_res.code == 1);
  CHECK(dcpRayLine_res.ur == 0.0);
  CHECK(dcpRayLine_res.ul == -5.0);
  CHECK(dcpRayLine_res.cpr == r0.Origin());
  CHECK(dcpRayLine_res.cpl == vec(0.0, 11.0, 60.0, 1.0));
  CHECK(dcpRayLine_res.distance == 61.0);
  CHECK(dcpRayLine_res.sqDistance == 61.0 * 61.0);

  //Line-Ray Parallel, opposite directions
  ln.Set(vec(5.0, 11.0, 60.0, 1.0), vec(-1.0, 0.0, 0.0, 0.0));
  dcpRayLine_res = dcpRayLine(r0, ln);
  CHECK(dcpRayLine_res.code == 1);
  CHECK(dcpRayLine_res.ur == 0.0);
  CHECK(dcpRayLine_res.ul == 5.0);
  CHECK(dcpRayLine_res.cpr == r0.Origin());
  CHECK(dcpRayLine_res.cpl == vec(0.0, 11.0, 60.0, 1.0));
  CHECK(dcpRayLine_res.distance == 61.0);
  CHECK(dcpRayLine_res.sqDistance == 61.0 * 61.0);

  //Line-Ray not parallel, closest point is ray origin
  ln.Set(vec(-9.0, 40.0, 61.0, 1.0), vec(0.0, 0.0, -1.0, 0.0));
  dcpRayLine_res = dcpRayLine(r0, ln);
  CHECK(dcpRayLine_res.code == 0);
  CHECK(dcpRayLine_res.ur == 0.0);
  CHECK(dcpRayLine_res.ul == 61.0);
  CHECK(dcpRayLine_res.cpr == r0.Origin());
  CHECK(dcpRayLine_res.cpl == vec(-9.0, 40.0, 0.0, 1.0));
  CHECK(dcpRayLine_res.distance == 41.0);
  CHECK(dcpRayLine_res.sqDistance == 41.0 * 41.0);

  //Line-Ray not parallel, closest point is along the ray
  ln.Set(vec(9.0, 40.0, -61.0, 1.0), vec(0.0, 0.0, 1.0, 0.0));
  dcpRayLine_res = dcpRayLine(r0, ln);
  CHECK(dcpRayLine_res.code == 0);
  CHECK(dcpRayLine_res.ur == 9.0);
  CHECK(dcpRayLine_res.ul == 61.0);
  CHECK(dcpRayLine_res.cpr == vec(9.0, 0.0, 0.0, 1.0));
  CHECK(dcpRayLine_res.cpl == vec(9.0, 40.0, 0.0, 1.0));
  CHECK(dcpRayLine_res.distance == 40.0);
  CHECK(dcpRayLine_res.sqDistance == 1600.0);

  //Ray-Plane
  plane pln;
  vec pr;
  Real ur;
  int result;

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