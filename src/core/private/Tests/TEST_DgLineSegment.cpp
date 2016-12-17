#include "TestHarness.h"
#include "DgSegment.h"
#include "DgMatrix44.h"
#include "DgVQS.h"
#include "query/DgQueryPointSegment.h"
#include "query/DgQuerySegmentLine.h"
#include "query/DgQuerySegmentRay.h"
#include "query/DgQuerySegmentSegment.h"
#include "query/DgQuerySegmentPlane.h"

typedef double Real;
typedef Dg::Vector4<Real>               vec;
typedef Dg::Plane<Real>                 plane;
typedef Dg::Line<Real>                  line;
typedef Dg::Matrix44<Real>              mat44;
typedef Dg::VQS<Real>                   vqs;
typedef Dg::Quaternion<Real>            quat;
typedef Dg::Ray<Real>                   ray;
typedef Dg::Segment<Real>           lineSeg;

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
  Dg::CPPointSegment<Real>           dcpPointLS;
  Dg::CPPointSegment<Real>::Result   dcpPointLS_res;
  ls0.Set(vec(2.0, 0.0, 0.0, 1.0), vec(6.0, 0.0, 0.0, 1.0));
  vec pIn;

  //Point behind p0
  pIn.Set(0.0, 3.0, -6.0, 1.0);
  dcpPointLS_res = dcpPointLS(pIn, ls0);
  CHECK(dcpPointLS_res.u == 0.0);
  CHECK(dcpPointLS_res.cp == ls0.GetP0());
  //CHECK(dcpPointLS_res.distance == 7.0);
  //CHECK(dcpPointLS_res.sqDistance == 49.0);

  //Point behind p1
  pIn.Set(8.0, 3.0, -6.0, 1.0);
  dcpPointLS_res = dcpPointLS(pIn, ls0);
  CHECK(dcpPointLS_res.u == 1.0);
  CHECK(dcpPointLS_res.cp == ls0.GetP1());
  //CHECK(dcpPointLS_res.distance == 7.0);
  //CHECK(dcpPointLS_res.sqDistance == 49.0);

  //Closest point along lineSeg
  pIn.Set(3.0, 3.0, -4.0, 1.0);
  dcpPointLS_res = dcpPointLS(pIn, ls0);
  CHECK(dcpPointLS_res.u == 0.25);
  CHECK(dcpPointLS_res.cp == vec(3.0, 0.0, 0.0, 1.0));
  //CHECK(dcpPointLS_res.distance == 5.0);
  //CHECK(dcpPointLS_res.sqDistance == 25.0);

  //lineSeg-Line
  Dg::CPSegmentLine<Real>           dcpLSLine;
  Dg::CPSegmentLine<Real>::Result   dcpLSLine_res;
  line l;

  //LineSeg parallel to line
  l.Set(vec(3.0, 3.0, 4.0, 1.0), vec::xAxis());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == 1);
  CHECK(dcpLSLine_res.ul == -1.0);
  CHECK(dcpLSLine_res.us == 0.0);
  CHECK(dcpLSLine_res.cps == ls0.GetP0());
  CHECK(dcpLSLine_res.cpl == vec(2.0, 3.0, 4.0, 1.0));
  //CHECK(dcpLSLine_res.distance == 5.0);
  //CHECK(dcpLSLine_res.sqDistance == 25.0);

  //LineSeg parallel to line, opposite direction
  l.Set(vec(3.0, 3.0, 4.0, 1.0), -vec::xAxis());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == 1);
  CHECK(dcpLSLine_res.ul == 1.0);
  CHECK(dcpLSLine_res.us == 0.0);
  CHECK(dcpLSLine_res.cps == ls0.GetP0());
  CHECK(dcpLSLine_res.cpl == vec(2.0, 3.0, 4.0, 1.0));
  //CHECK(dcpLSLine_res.distance == 5.0);
  //CHECK(dcpLSLine_res.sqDistance == 25.0);

  //lineSeg-p0 closest point
  l.Set(vec(-1.0, 4.0, 3.0, 1.0), -vec::zAxis());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == 0);
  CHECK(dcpLSLine_res.ul == 3.0);
  CHECK(dcpLSLine_res.us == 0.0);
  CHECK(dcpLSLine_res.cps == ls0.GetP0());
  CHECK(dcpLSLine_res.cpl == vec(-1.0, 4.0, 0.0, 1.0));
  //CHECK(dcpLSLine_res.distance == 5.0);
  //CHECK(dcpLSLine_res.sqDistance == 25.0);

  //lineSeg-p1 closest point
  l.Set(vec(9.0, 4.0, 3.0, 1.0), -vec::zAxis());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == 0);
  CHECK(dcpLSLine_res.ul == 3.0);
  CHECK(dcpLSLine_res.us == 1.0);
  CHECK(dcpLSLine_res.cps == ls0.GetP1());
  CHECK(dcpLSLine_res.cpl == vec(9.0, 4.0, 0.0, 1.0));
  //CHECK(dcpLSLine_res.distance == 5.0);
  //CHECK(dcpLSLine_res.sqDistance == 25.0);

  //Closest point along lineSeg
  l.Set(vec(3.0, 4.0, 3.0, 1.0), vec::zAxis());
  dcpLSLine_res = dcpLSLine(ls0, l);
  CHECK(dcpLSLine_res.code == 0);
  CHECK(dcpLSLine_res.ul == -3.0);
  CHECK(dcpLSLine_res.us == 0.25);
  CHECK(dcpLSLine_res.cps == vec(3.0, 0.0, 0.0, 1.0));
  CHECK(dcpLSLine_res.cpl == vec(3.0, 4.0, 0.0, 1.0));
  //CHECK(dcpLSLine_res.distance == 4.0);
  //CHECK(dcpLSLine_res.sqDistance == 16.0);

  //lineSeg-Ray
  ray r;
  Dg::CPSegmentRay<Real>           dcpLSRay;
  Dg::CPSegmentRay<Real>::Result   dcpLSRay_res;

  //LineSeg parallel to ray, but behind ray origin
  r.Set(vec(-1.0, 4.0, 12.0, 1.0), -vec::xAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == r.Origin());
  //CHECK(dcpLSRay_res.distance == 13.0);
  //CHECK(dcpLSRay_res.sqDistance == 13.0 * 13.0);

  //LineSeg parallel to ray, but behind ray origin, switch LineSeg direction
  r.Set(vec(9.0, 4.0, -12.0, 1.0), vec::xAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 1.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP1());
  CHECK(dcpLSRay_res.cpr == r.Origin());
  //CHECK(dcpLSRay_res.distance == 13.0);
  //CHECK(dcpLSRay_res.sqDistance == 13.0 * 13.0);

  //LineSeg parallel to ray, p0 behind ray origin, p1 along ray
  r.Set(vec(4.0, 3.0, 4.0, 1.0), vec::xAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 1);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 0.5);
  CHECK(dcpLSRay_res.cps == vec(4.0, 0.0, 0.0, 1.0));
  CHECK(dcpLSRay_res.cpr == r.Origin());
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg parallel to ray, p1 behind ray origin, p0 along ray
  r.Set(vec(4.0, 3.0, 4.0, 1.0), -vec::xAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 1);
  CHECK(dcpLSRay_res.ur == 2.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == vec(2.0, 3.0, 4.0, 1.0));
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg parallel to ray, completely in front of ray, p0 closer to ray origin
  r.Set(vec(-2.0, 3.0, 4.0, 1.0), vec::xAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 1);
  CHECK(dcpLSRay_res.ur == 4.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls0.Origin());
  CHECK(dcpLSRay_res.cpr == vec(2.0, 3.0, 4.0, 1.0));
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg parallel to ray, completely in front of ray, p1 closer to ray origin
  r.Set(vec(10.0, 3.0, 4.0, 1.0), -vec::xAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 1);
  CHECK(dcpLSRay_res.ur == 8.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == vec(2.0, 3.0, 4.0, 1.0));
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg not parallel, behind ray, Closest points are ls-p0, ray-origin
  r.Set(vec(1.0, 6.0, -18.0, 1.0), -vec::zAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == r.Origin());
  //CHECK(dcpLSRay_res.distance == 19.0);
  //CHECK(dcpLSRay_res.sqDistance == 19.0 * 19.0);
  
  //LineSeg not parallel, behind ray, Closest points are ls-p1, ray-origin
  r.Set(vec(7.0, -6.0, 18.0, 1.0), -vec::yAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 1.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP1());
  CHECK(dcpLSRay_res.cpr == r.Origin());
  //CHECK(dcpLSRay_res.distance == 19.0);
  //CHECK(dcpLSRay_res.sqDistance == 19.0 * 19.0);
  
  //LineSeg not parallel, behind ray, Closest points are along the ls, ray-origin
  r.Set(vec(5.0, -3.0, 4.0, 1.0), -vec::yAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 0.0);
  CHECK(dcpLSRay_res.us == 0.75);
  CHECK(dcpLSRay_res.cps == vec(5.0, 0.0, 0.0, 1.0));
  CHECK(dcpLSRay_res.cpr == r.Origin());
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);
  
  //LineSeg in front of ray, closest point p0
  r.Set(vec(-1.0, -4.0, -3.0, 1.0), vec::zAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 3.0);
  CHECK(dcpLSRay_res.us == 0.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP0());
  CHECK(dcpLSRay_res.cpr == vec(-1.0, -4.0, 0.0, 1.0));
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg in front of ray, closest point p1
  r.Set(vec(9.0, -4.0, -3.0, 1.0), vec::zAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 3.0);
  CHECK(dcpLSRay_res.us == 1.0);
  CHECK(dcpLSRay_res.cps == ls0.GetP1());
  CHECK(dcpLSRay_res.cpr == vec(9.0, -4.0, 0.0, 1.0));
  //CHECK(dcpLSRay_res.distance == 5.0);
  //CHECK(dcpLSRay_res.sqDistance == 25.0);

  //LineSeg in front of ray, closest point along ls
  r.Set(vec(5.0, -4.0, -3.0, 1.0), vec::zAxis());
  dcpLSRay_res = dcpLSRay(ls0, r);
  CHECK(dcpLSRay_res.code == 0);
  CHECK(dcpLSRay_res.ur == 3.0);
  CHECK(dcpLSRay_res.us == 0.75);
  CHECK(dcpLSRay_res.cps == vec(5.0, 0.0, 0.0, 1.0));
  CHECK(dcpLSRay_res.cpr == vec(5.0, -4.0, 0.0, 1.0));
  //CHECK(dcpLSRay_res.distance == 4.0);
  //CHECK(dcpLSRay_res.sqDistance == 16.0);

  //lineSeg-LineSeg
  Dg::CPSegmentSegment<Real>           dcpLSLS;
  Dg::CPSegmentSegment<Real>::Result   dcpLSLS_res;

  //LineSegs parallel, no overlap, closest points ls0-p0, ls1-p0
  ls1.Set(vec(-1.0, -4.0, 12.0, 1.0), vec(-5.0, -4.0, 12.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 0);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 13.0);
  //CHECK(dcpLSLS_res.sqDistance == 169.0);

  //LineSegs parallel, no overlap, closest points ls0-p0, ls1-p1
  ls1.Set(vec(-5.0, -4.0, 12.0, 1.0), vec(-1.0, -4.0, 12.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 0);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 1.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP1());
  //CHECK(dcpLSLS_res.distance == 13.0);
  //CHECK(dcpLSLS_res.sqDistance == 169.0);
  
  //LineSegs parallel, no overlap, closest points ls0-p1, ls1-p0
  ls1.Set(vec(9.0, -4.0, 12.0, 1.0), vec(18.0, -4.0, 12.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 0);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 13.0);
  //CHECK(dcpLSLS_res.sqDistance == 169.0);
  
  //LineSegs parallel, no overlap, closest points ls0-p1, ls1-p1
  ls1.Set(vec(10.0, -4.0, 12.0, 1.0), vec(9.0, -4.0, 12.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 0);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 1.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP1());
  //CHECK(dcpLSLS_res.distance == 13.0);
  //CHECK(dcpLSLS_res.sqDistance == 169.0);
  
  //LineSegs parallel, overlap, ls0p0---ls1p0---ls0p1---ls1p1
  ls1.Set(vec(4.0, -3.0, 4.0, 1.0), vec(10.0, -3.0, 4.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 1);
  CHECK(dcpLSLS_res.u0 == 0.5);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == vec(4.0, 0.0, 0.0, 1.0));
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 5.0);
  //CHECK(dcpLSLS_res.sqDistance == 25.0);

  //LineSegs parallel, overlap, ls0p1---ls1p0---ls0p0---ls1p1
  ls1.Set(vec(4.0, -3.0, 4.0, 1.0), vec(0.0, -3.0, 4.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 1);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 0.5);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == vec(2.0, -3.0, 4.0, 1.0));
  //CHECK(dcpLSLS_res.distance == 5.0);
  //CHECK(dcpLSLS_res.sqDistance == 25.0);

  //LineSegs parallel, overlap, ls0p0---ls1p0---ls1p1---ls0p1
  ls1.Set(vec(4.0, -3.0, 4.0, 1.0), vec(5.0, -3.0, 4.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 1);
  CHECK(dcpLSLS_res.u0 == 0.5);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == vec(4.0, 0.0, 0.0, 1.0));
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 5.0);
  //CHECK(dcpLSLS_res.sqDistance == 25.0);

  //LineSegs parallel, overlap, ls0p1---ls1p0---ls1p1---ls0p0
  ls1.Set(vec(4.0, -3.0, 4.0, 1.0), vec(8.0, -3.0, 4.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 1);
  CHECK(dcpLSLS_res.u0 == 0.5);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == vec(4.0, 0.0, 0.0, 1.0));
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 5.0);
  //CHECK(dcpLSLS_res.sqDistance == 25.0);

  //LineSegs not parallel, closest points: ls0p0, ls1p0
  ls1.Set(vec(-2.0, -5.0, 20.0, 1.0), vec(-2.0, -5.0, 23.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 0);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 21.0);
  //CHECK(dcpLSLS_res.sqDistance == 21.0 * 21.0);

  //LineSegs not parallel, closest points: ls0p0, ls1p1
  ls1.Set(vec(-2.0, -5.0, 23.0, 1.0), vec(-2.0, -5.0, 20.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 0);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 1.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP1());
  //CHECK(dcpLSLS_res.distance == 21.0);
  //CHECK(dcpLSLS_res.sqDistance == 21.0 * 21.0);

  //LineSegs not parallel, closest points: ls0p1, ls1p0
  ls1.Set(vec(10.0, -5.0, 20.0, 1.0), vec(10.0, -5.0, 23.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 0);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 0.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP0());
  //CHECK(dcpLSLS_res.distance == 21.0);
  //CHECK(dcpLSLS_res.sqDistance == 21.0 * 21.0);

  //LineSegs not parallel, closest points: ls0p1, ls1p1
  ls1.Set(vec(10.0, -5.0, 23.0, 1.0), vec(10.0, -5.0, 20.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 0);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 1.0);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == ls1.GetP1());
  //CHECK(dcpLSLS_res.distance == 21.0);
  //CHECK(dcpLSLS_res.sqDistance == 21.0 * 21.0);

  //LineSegs not parallel, closest points: ls0p0, ls1-along ls
  ls1.Set(vec(-1.0, 4.0, -3.0, 1.0), vec(-1.0, 4.0, 3.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 0);
  CHECK(dcpLSLS_res.u0 == 0.0);
  CHECK(dcpLSLS_res.u1 == 0.5);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP0());
  CHECK(dcpLSLS_res.cp1 == vec(-1.0, 4.0, 0.0, 1.0));
  //CHECK(dcpLSLS_res.distance == 5.0);
  //CHECK(dcpLSLS_res.sqDistance == 25.0);

  //LineSegs not parallel, closest points: ls0p1, ls1-along ls
  ls1.Set(vec(9.0, 4.0, -3.0, 1.0), vec(9.0, 4.0, 3.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 0);
  CHECK(dcpLSLS_res.u0 == 1.0);
  CHECK(dcpLSLS_res.u1 == 0.5);
  CHECK(dcpLSLS_res.cp0 == ls0.GetP1());
  CHECK(dcpLSLS_res.cp1 == vec(9.0, 4.0, 0.0, 1.0));
  //CHECK(dcpLSLS_res.distance == 5.0);
  //CHECK(dcpLSLS_res.sqDistance == 25.0);

  //LineSegs not parallel, closest points: ls0-along ls, ls1-along ls
  ls1.Set(vec(4.0, 4.0, -3.0, 1.0), vec(4.0, -4.0, -3.0, 1.0));
  dcpLSLS_res = dcpLSLS(ls0, ls1);
  CHECK(dcpLSLS_res.code == 0);
  CHECK(dcpLSLS_res.u0 == 0.5);
  CHECK(dcpLSLS_res.u1 == 0.5);
  CHECK(dcpLSLS_res.cp0 == vec(4.0, 0.0, 0.0, 1.0));
  CHECK(dcpLSLS_res.cp1 == vec(4.0, 0.0, -3.0, 1.0));
  //CHECK(dcpLSLS_res.distance == 3.0);
  //CHECK(dcpLSLS_res.sqDistance == 9.0);

  //LineSeg-plane
  Dg::TISegmentPlane<Real>          tiLSPlane;
  Dg::FISegmentPlane<Real>          fiLSPlane;
  Dg::TISegmentPlane<Real>::Result  tiLSPlane_res;
  Dg::FISegmentPlane<Real>::Result  fiLSPlane_res;
  plane pl;
  
  //LineSeg parallel to plane
  pl.Set(vec::yAxis(), vec(0.0, 2.0, 0.0, 1.0));
  tiLSPlane_res = tiLSPlane(ls0, pl);
  CHECK(tiLSPlane_res.isIntersecting == false);

  fiLSPlane_res = fiLSPlane(ls0, pl);
  CHECK(fiLSPlane_res.code == 2);
  CHECK(fiLSPlane_res.point == ls0.Origin());
  CHECK(fiLSPlane_res.u == 0.0);

  //LineSeg on plane
  pl.Set(vec::yAxis(), vec(0.0, 0.0, 0.0, 1.0));
  tiLSPlane_res = tiLSPlane(ls0, pl);
  CHECK(tiLSPlane_res.isIntersecting == true);

  fiLSPlane_res = fiLSPlane(ls0, pl);
  CHECK(fiLSPlane_res.code == 1);
  CHECK(fiLSPlane_res.point == ls0.Origin());
  CHECK(fiLSPlane_res.u == 0.0);

  //LineSeg not parallel to plane, not intersecting, closest point p0
  pl.Set(vec(-0.1, 1.0, 0.1, 1.0), vec(0.0, 3.0, 0.0, 1.0));
  tiLSPlane_res = tiLSPlane(ls0, pl);
  CHECK(tiLSPlane_res.isIntersecting == false);

  fiLSPlane_res = fiLSPlane(ls0, pl);
  CHECK(fiLSPlane_res.code == 2);
  CHECK(fiLSPlane_res.point == ls0.Origin());
  CHECK(fiLSPlane_res.u == 0.0);

  //LineSeg not parallel to plane, not intersecting, closest point p1
  pl.Set(vec(0.1, 1.0, 0.1, 1.0), vec(0.0, 3.0, 0.0, 1.0));
  tiLSPlane_res = tiLSPlane(ls0, pl);
  CHECK(tiLSPlane_res.isIntersecting == false);

  fiLSPlane_res = fiLSPlane(ls0, pl);
  CHECK(fiLSPlane_res.code == 2);
  CHECK(fiLSPlane_res.point == ls0.GetP1());
  CHECK(fiLSPlane_res.u == 1.0);

  //LineSeg not parallel to plane, intersecting
  pl.Set(vec::xAxis(), vec(4.0, 0.0, 0.0, 1.0));
  tiLSPlane_res = tiLSPlane(ls0, pl);
  CHECK(tiLSPlane_res.isIntersecting == true);

  fiLSPlane_res = fiLSPlane(ls0, pl);
  CHECK(fiLSPlane_res.code == 0);
  CHECK(fiLSPlane_res.point == vec(4.0, 0.0, 0.0, 1.0));
  CHECK(fiLSPlane_res.u == 0.5);

  //LineSeg not parallel to plane, intersecting, switch points
  pl.Set(-vec::xAxis(), vec(4.0, 0.0, 0.0, 1.0));
  tiLSPlane_res = tiLSPlane(ls0, pl);
  CHECK(tiLSPlane_res.isIntersecting == true);

  fiLSPlane_res = fiLSPlane(ls0, pl);
  CHECK(fiLSPlane_res.code == 0);
  CHECK(fiLSPlane_res.point == vec(4.0, 0.0, 0.0, 1.0));
  CHECK(fiLSPlane_res.u == 0.5);
}

TEST(Stack_DgLineSegmentTransform, DgLineSegmentTransform)
{
  lineSeg x0(vec(2.0, 0.0, 0.0, 1.0), vec(4.0, 0.0, 0.0, 1.0));
  lineSeg x1 = x0;
  lineSeg x2 = x0;
  lineSeg xFinal(vec(1.0, 2.0, 7.0, 1.0), vec(1.0, 2.0, 11.0, 1.0));

  Real scale = 2.0;
  vec trans(1.0, 2.0, 3.0, 0.0);
  Real rx = Dg::Constants<Real>::PI / 2.0;
  Real ry = 0.0;
  Real rz = Dg::Constants<Real>::PI / 2.0;
  Dg::EulerOrder eo = Dg::EulerOrder::ZXY;

  mat44 m, m_rot, m_scl, m_trans;
  m_rot.Rotation(rz, ry, rx, eo);
  m_scl.Scaling(scale);
  m_trans.Translation(trans);
  m = m_rot * m_scl * m_trans;

  vqs v;
  quat q;
  q.SetRotation(rx, ry, rz, eo);
  v.SetQ(q);
  v.SetS(scale);
  v.SetV(trans);

  x1.TransformSelf(m);
  x2 = x0.GetTransformed(m);
  CHECK(x1 == x2);
  CHECK(x1 == xFinal);

  x1 = x0;
  x1.TransformSelf(v);
  x2 = x0.GetTransformed(v);
  CHECK(x1 == x2);
  CHECK(x1 == xFinal);
}