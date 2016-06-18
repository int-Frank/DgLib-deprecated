#include "TestHarness.h"
#include "DgLineSegment.h"
#include "query/DgQueryPointLineSegment.h"
#include "query/DgQueryLineSegmentLine.h"
#include "query/DgQueryLineSegmentRay.h"

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
  Dg::DCPPointLineSegment<Real>           dcpPointLS;
  Dg::DCPPointLineSegment<Real>::Result   dcpPointLS_res;
  ls0.Set(vec(2.0, 0.0, 0.0, 1.0), vec(6.0, 0.0, 0.0, 1.0));
  vec pIn;

  //Point behind p0
  pIn.Set(0.0, 3.0, -6.0, 1.0);
  dcpPointLS_res = dcpPointLS(pIn, ls0);
  CHECK(dcpPointLS_res.u == 0.0);
  CHECK(dcpPointLS_res.cp == ls0.GetP0());
  CHECK(dcpPointLS_res.distance == 7.0);
  CHECK(dcpPointLS_res.sqDistance == 49.0);

  //Point behind p1
  pIn.Set(8.0, 3.0, -6.0, 1.0);
  dcpPointLS_res = dcpPointLS(pIn, ls0);
  CHECK(dcpPointLS_res.u == 1.0);
  CHECK(dcpPointLS_res.cp == ls0.GetP1());
  CHECK(dcpPointLS_res.distance == 7.0);
  CHECK(dcpPointLS_res.sqDistance == 49.0);

  //Closest point along lineSeg
  pIn.Set(3.0, 3.0, -4.0, 1.0);
  dcpPointLS_res = dcpPointLS(pIn, ls0);
  CHECK(dcpPointLS_res.u == 0.25);
  CHECK(dcpPointLS_res.cp == vec(3.0, 0.0, 0.0, 1.0));
  CHECK(dcpPointLS_res.distance == 5.0);
  CHECK(dcpPointLS_res.sqDistance == 25.0);

  //lineSeg-Line
  Dg::DCPLineSegmentLine<Real>           dcpLSLine;
  Dg::DCPLineSegmentLine<Real>::Result   dcpLSLine_res;
  line l;

  //LineSeg parallel to line
  l.Set(vec(3.0, 3.0, 4.0, 1.0), vec::xAxis());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == 1);
  CHECK(dcpLSLine_res.ul == -1.0);
  CHECK(dcpLSLine_res.uls == 0.0);
  CHECK(dcpLSLine_res.cpls == ls0.GetP0());
  CHECK(dcpLSLine_res.cpl == vec(2.0, 3.0, 4.0, 1.0));
  CHECK(dcpLSLine_res.distance == 5.0);
  CHECK(dcpLSLine_res.sqDistance == 25.0);

  //LineSeg parallel to line, opposite direction
  l.Set(vec(3.0, 3.0, 4.0, 1.0), -vec::xAxis());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == 1);
  CHECK(dcpLSLine_res.ul == 1.0);
  CHECK(dcpLSLine_res.uls == 0.0);
  CHECK(dcpLSLine_res.cpls == ls0.GetP0());
  CHECK(dcpLSLine_res.cpl == vec(2.0, 3.0, 4.0, 1.0));
  CHECK(dcpLSLine_res.distance == 5.0);
  CHECK(dcpLSLine_res.sqDistance == 25.0);

  //lineSeg-p0 closest point
  l.Set(vec(-1.0, 4.0, 3.0, 1.0), -vec::zAxis());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == 0);
  CHECK(dcpLSLine_res.ul == 3.0);
  CHECK(dcpLSLine_res.uls == 0.0);
  CHECK(dcpLSLine_res.cpls == ls0.GetP0());
  CHECK(dcpLSLine_res.cpl == vec(-1.0, 4.0, 0.0, 1.0));
  CHECK(dcpLSLine_res.distance == 5.0);
  CHECK(dcpLSLine_res.sqDistance == 25.0);

  //lineSeg-p1 closest point
  l.Set(vec(9.0, 4.0, 3.0, 1.0), -vec::zAxis());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == 0);
  CHECK(dcpLSLine_res.ul == 3.0);
  CHECK(dcpLSLine_res.uls == 1.0);
  CHECK(dcpLSLine_res.cpls == ls0.GetP1());
  CHECK(dcpLSLine_res.cpl == vec(9.0, 4.0, 0.0, 1.0));
  CHECK(dcpLSLine_res.distance == 5.0);
  CHECK(dcpLSLine_res.sqDistance == 25.0);

  //Closest point along lineSeg
  l.Set(vec(3.0, 4.0, 3.0, 1.0), vec::zAxis());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == 0);
  CHECK(dcpLSLine_res.ul == -3.0);
  CHECK(dcpLSLine_res.uls == 0.25);
  CHECK(dcpLSLine_res.cpls == vec(3.0, 0.0, 0.0, 1.0));
  CHECK(dcpLSLine_res.cpl == vec(3.0, 4.0, 0.0, 1.0));
  CHECK(dcpLSLine_res.distance == 4.0);
  CHECK(dcpLSLine_res.sqDistance == 16.0);

  //lineSeg-Ray
  ray r;
  Dg::DCPLineSegmentRay<Real>           dcpLSRay;
  Dg::DCPLineSegmentRay<Real>::Result   dcpLSRay_res;

  //LineSeg parallel to ray, but behind ray origin
  r.Set(vec(-1.0, 4.0, 12.0, 1.0), -vec::xAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.uls == 0.0);
  CHECK(dcpLSRay_res.cpls == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == r.Origin());
  CHECK(dcpLSRay_res.distance == 13.0);
  CHECK(dcpLSRay_res.sqDistance == 13.0 * 13.0);

  //LineSeg parallel to ray, but behind ray origin, switch LineSeg direction
  r.Set(vec(9.0, 4.0, -12.0, 1.0), vec::xAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.uls == 1.0);
  CHECK(dcpLSRay_res.cpls == ls0.GetP1());
  CHECK(dcpLSRay_res.cpr == r.Origin());
  CHECK(dcpLSRay_res.distance == 13.0);
  CHECK(dcpLSRay_res.sqDistance == 13.0 * 13.0);

  //LineSeg parallel to ray, p0 behind ray origin, p1 along ray
  r.Set(vec(4.0, 3.0, 4.0, 1.0), vec::xAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 1);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.uls == 0.5);
  CHECK(dcpLSRay_res.cpls == vec(4.0, 0.0, 0.0, 1.0));
  CHECK(dcpLSRay_res.cpr == r.Origin());
  CHECK(dcpLSRay_res.distance == 5.0);
  CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg parallel to ray, p1 behind ray origin, p0 along ray
  r.Set(vec(4.0, 3.0, 4.0, 1.0), -vec::xAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 1);
  CHECK(dcpLSRay_res.ur == 2.0);
  CHECK(dcpLSRay_res.uls == 0.0);
  CHECK(dcpLSRay_res.cpls == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == vec(2.0, 3.0, 4.0, 1.0));
  CHECK(dcpLSRay_res.distance == 5.0);
  CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg parallel to ray, completely in front of ray, p0 closer to ray origin
  r.Set(vec(-2.0, 3.0, 4.0, 1.0), vec::xAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 1);
  CHECK(dcpLSRay_res.ur == 4.0);
  CHECK(dcpLSRay_res.uls == 0.0);
  CHECK(dcpLSRay_res.cpls == ls0.Origin());
  CHECK(dcpLSRay_res.cpr == vec(2.0, 3.0, 4.0, 1.0));
  CHECK(dcpLSRay_res.distance == 5.0);
  CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg parallel to ray, completely in front of ray, p1 closer to ray origin
  r.Set(vec(10.0, 3.0, 4.0, 1.0), -vec::xAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 1);
  CHECK(dcpLSRay_res.ur == 8.0);
  CHECK(dcpLSRay_res.uls == 0.0);
  CHECK(dcpLSRay_res.cpls == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == vec(2.0, 3.0, 4.0, 1.0));
  CHECK(dcpLSRay_res.distance == 5.0);
  CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg not parallel, behind ray, Closest points are ls-p0, ray-origin
  r.Set(vec(1.0, 6.0, -18.0, 1.0), -vec::zAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.uls == 0.0);
  CHECK(dcpLSRay_res.cpls == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == r.Origin());
  CHECK(dcpLSRay_res.distance == 19.0);
  CHECK(dcpLSRay_res.sqDistance == 19.0 * 19.0);
  
  //LineSeg not parallel, behind ray, Closest points are ls-p1, ray-origin
  r.Set(vec(7.0, -6.0, 18.0, 1.0), -vec::yAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.uls == 1.0);
  CHECK(dcpLSRay_res.cpls == ls0.GetP1());
  CHECK(dcpLSRay_res.cpr == r.Origin());
  CHECK(dcpLSRay_res.distance == 19.0);
  CHECK(dcpLSRay_res.sqDistance == 19.0 * 19.0);
  
  //LineSeg not parallel, behind ray, Closest points are along the ls, ray-origin
  r.Set(vec(5.0, -3.0, 4.0, 1.0), -vec::yAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.uls == 0.75);
  CHECK(dcpLSRay_res.cpls == vec(5.0, 0.0, 0.0, 1.0));
  CHECK(dcpLSRay_res.cpr == r.Origin());
  CHECK(dcpLSRay_res.distance == 5.0);
  CHECK(dcpLSRay_res.sqDistance == 25.0);
  
  //LineSeg in front of ray, closest point p0
  r.Set(vec(-1.0, -4.0, -3.0, 1.0), vec::zAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 3.0);
  CHECK(dcpLSRay_res.uls == 0.0);
  CHECK(dcpLSRay_res.cpls == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == vec(-1.0, -4.0, 0.0, 1.0));
  CHECK(dcpLSRay_res.distance == 5.0);
  CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg in front of ray, closest point p1
  r.Set(vec(9.0, -4.0, -3.0, 1.0), vec::zAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 3.0);
  CHECK(dcpLSRay_res.uls == 1.0);
  CHECK(dcpLSRay_res.cpls == ls0.GetP1());
  CHECK(dcpLSRay_res.cpr == vec(9.0, -4.0, 0.0, 1.0));
  CHECK(dcpLSRay_res.distance == 5.0);
  CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg in front of ray, closest point along ls
  r.Set(vec(5.0, -4.0, -3.0, 1.0), vec::zAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 3.0);
  CHECK(dcpLSRay_res.uls == 0.75);
  CHECK(dcpLSRay_res.cpls == vec(5.0, 0.0, 0.0, 1.0));
  CHECK(dcpLSRay_res.cpr == vec(5.0, -4.0, 0.0, 1.0));
  CHECK(dcpLSRay_res.distance == 4.0);
  CHECK(dcpLSRay_res.sqDistance == 16.0);

  //lineSeg-LineSeg
  Real uls0 = 0.0;
  Real uls1 = 0.0;
  vec pls0, pls1;
  int result;

  //LineSegs parallel, no overlap, closest points ls0-p0, ls1-p0
  ls1.Set(vec(-3.0, -2.0, 3.0, 1.0), vec(-5.0, -2.0, 3.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 1);
  CHECK(uls0 == 0.0);
  CHECK(uls1 == 0.0);
  CHECK(pls0 == ls0.GetP0());
  CHECK(pls1 == ls1.GetP0());

  //LineSegs parallel, no overlap, closest points ls0-p0, ls1-p1
  ls1.Set(vec(-5.0, -2.0, 3.0, 1.0), vec(-3.0, -2.0, 3.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 1);
  CHECK(uls0 == 0.0);
  CHECK(uls1 == 1.0);
  CHECK(pls0 == ls0.GetP0());
  CHECK(pls1 == ls1.GetP1());
  
  //LineSegs parallel, no overlap, closest points ls0-p1, ls1-p0
  ls1.Set(vec(10.0, -2.0, 3.0, 1.0), vec(12.0, -2.0, 3.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 1);
  CHECK(uls0 == 1.0);
  CHECK(uls1 == 0.0);
  CHECK(pls0 == ls0.GetP1());
  CHECK(pls1 == ls1.GetP0());
  
  //LineSegs parallel, no overlap, closest points ls0-p1, ls1-p1
  ls1.Set(vec(12.0, -2.0, 3.0, 1.0), vec(10.0, -2.0, 3.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 1);
  CHECK(uls0 == 1.0);
  CHECK(uls1 == 1.0);
  CHECK(pls0 == ls0.GetP1());
  CHECK(pls1 == ls1.GetP1());
  
  //LineSegs parallel, overlap, ls0p0---ls1p0---ls0p1---ls1p1
  ls1.Set(vec(3.0, -2.0, 3.0, 1.0), vec(9.0, -2.0, 3.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 1);
  CHECK(uls0 == 0.25);
  CHECK(uls1 == 0.0);
  CHECK(pls0 == vec(3.0, 0.0, 0.0, 1.0));
  CHECK(pls1 == ls1.GetP0());

  //LineSegs parallel, overlap, ls0p1---ls1p0---ls0p0---ls1p1
  ls1.Set(vec(-3.0, -2.0, 3.0, 1.0), vec(4.0, -2.0, 3.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 1);
  CHECK(uls0 == 0.0);
  CHECK(Dg::AreEqual(uls1, 5.0 / 7.0));
  CHECK(pls0 == ls0.GetP0());
  CHECK(pls1 == vec(2.0, -2.0, 3.0, 1.0));

  //LineSegs parallel, overlap, ls0p0---ls1p0---ls1p1---ls0p1
  ls1.Set(vec(3.0, -2.0, 3.0, 1.0), vec(5.0, -2.0, 3.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 1);
  CHECK(uls0 == 0.25);
  CHECK(uls1 == 0.0);
  CHECK(pls0 == vec(3.0, 0.0, 0.0, 1.0));
  CHECK(pls1 == ls1.GetP0());

  //LineSegs parallel, overlap, ls0p1---ls1p0---ls1p1---ls0p0
  ls1.Set(vec(5.0, -2.0, 3.0, 1.0), vec(3.0, -2.0, 3.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 1);
  CHECK(uls0 == 0.25);
  CHECK(uls1 == 1.0);
  CHECK(pls0 == vec(3.0, 0.0, 0.0, 1.0));
  CHECK(pls1 == ls1.GetP1());

  //LineSegs not parallel, closest points: ls0p0, ls1p0
  ls1.Set(vec(-3.0, -2.0, 3.0, 1.0), vec(-4.0, -2.0, 5.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 0);
  CHECK(uls0 == 0.0);
  CHECK(uls1 == 0.0);
  CHECK(pls0 == ls0.GetP0());
  CHECK(pls1 == ls1.GetP0());

  //LineSegs not parallel, closest points: ls0p0, ls1p1
  ls1.Set(vec(-3.0, -2.0, 5.0, 1.0), vec(-4.0, -2.0, 3.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 0);
  CHECK(uls0 == 0.0);
  CHECK(uls1 == 1.0);
  CHECK(pls0 == ls0.GetP0());
  CHECK(pls1 == ls1.GetP1());

  //LineSegs not parallel, closest points: ls0p1, ls1p0
  ls1.Set(vec(10.0, -2.0, 5.0, 1.0), vec(12.0, -2.0, 30.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 0);
  CHECK(uls0 == 1.0);
  CHECK(uls1 == 0.0);
  CHECK(pls0 == ls0.GetP1());
  CHECK(pls1 == ls1.GetP0());

  //LineSegs not parallel, closest points: ls0p1, ls1p1
  ls1.Set(vec(10.0, -2.0, 50.0, 1.0), vec(12.0, -2.0, 30.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 0);
  CHECK(uls0 == 1.0);
  CHECK(uls1 == 1.0);
  CHECK(pls0 == ls0.GetP1());
  CHECK(pls1 == ls1.GetP1());

  //LineSegs not parallel, closest points: ls0p0, ls1-along ls
  ls1.Set(vec(-3.0, -2.0, 2.0, 1.0), vec(-3.0, -2.0, -2.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 0);
  CHECK(uls0 == 0.0);
  CHECK(uls1 == 0.5);
  CHECK(pls0 == ls0.GetP0());
  CHECK(pls1 == vec(-3.0, -2.0, 0.0, 1.0));

  //LineSegs not parallel, closest points: ls0p1, ls1-along ls
  ls1.Set(vec(9.0, -2.0, 2.0, 1.0), vec(9.0, -2.0, -2.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 0);
  CHECK(uls0 == 1.0);
  CHECK(uls1 == 0.5);
  CHECK(pls0 == ls0.GetP1());
  CHECK(pls1 == vec(9.0, -2.0, 0.0, 1.0));

  //LineSegs not parallel, closest points: ls0-along ls, ls1-along ls
  ls1.Set(vec(4.0, -2.0, 2.0, 1.0), vec(4.0, -2.0, -2.0, 1.0));
  result = ClosestPointsLineSegmentLineSegment(ls0, ls1, uls0, uls1, pls0, pls1);
  CHECK(result == 0);
  CHECK(uls0 == 0.5);
  CHECK(uls1 == 0.5);
  CHECK(pls0 == vec(4.0, 0.0, 0.0, 1.0));
  CHECK(pls1 == vec(4.0, -2.0, 0.0, 1.0));

  //LineSeg-plane
  plane p;
  Real uls = 0.0;
  vec pls;

  //LineSeg parallel to plane
  p.Set(vec::yAxis(), vec(0.0, 2.0, 0.0, 1.0));
  result = TestPlaneLineSegment(p, ls0, uls, pls);
  CHECK(result == 2);
  CHECK(uls == 0.0);
  CHECK(pls == ls0.GetP0());

  //LineSeg on plane
  p.Set(vec::yAxis(), vec(0.0, 0.0, 0.0, 1.0));
  result = TestPlaneLineSegment(p, ls0, uls, pls);
  CHECK(result == 1);
  CHECK(uls == 0.0);
  CHECK(pls == ls0.GetP0());

  //LineSeg not parallel to plane, not intersecting, closest point p0
  p.Set(vec(-0.1, 1.0, 0.1, 1.0), vec(0.0, 3.0, 0.0, 1.0));
  result = TestPlaneLineSegment(p, ls0, uls, pls);
  CHECK(result == 2);
  CHECK(uls == 0.0);
  CHECK(pls == ls0.GetP0());

  //LineSeg not parallel to plane, not intersecting, closest point p1
  p.Set(vec(0.1, 1.0, 0.1, 1.0), vec(0.0, 3.0, 0.0, 1.0));
  result = TestPlaneLineSegment(p, ls0, uls, pls);
  CHECK(result == 2);
  CHECK(uls == 1.0);
  CHECK(pls == ls0.GetP1());

  //LineSeg not parallel to plane, intersecting
  p.Set(vec::xAxis(), vec(4.0, 0.0, 0.0, 1.0));
  result = TestPlaneLineSegment(p, ls0, uls, pls);
  CHECK(result == 0);
  CHECK(uls == 0.5);
  CHECK(pls == vec(4.0, 0.0, 0.0, 1.0));

  //LineSeg not parallel to plane, intersecting, switch points
  p.Set(-vec::xAxis(), vec(4.0, 0.0, 0.0, 1.0));
  result = TestPlaneLineSegment(p, ls0, uls, pls);
  CHECK(result == 0);
  CHECK(uls == 0.5);
  CHECK(pls == vec(4.0, 0.0, 0.0, 1.0));
}