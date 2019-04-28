#include "TestHarness.h"
#include "DgR3Ray.h"
#include "DgR3Line.h"
#include "DgR3Matrix.h"
#include "DgR3VQS.h"
#include "query/DgR3QueryPointRay.h"
#include "query/DgR3QueryRayLine.h"
#include "query/DgR3QueryRayRay.h"
#include "query/DgR3QueryRayPlane.h"

typedef double Real;
typedef Dg::R3::Vector<Real>               vec;
typedef Dg::R3::Plane<Real>                 plane;
typedef Dg::R3::Line<Real>                  line;
typedef Dg::R3::Matrix<Real>              mat44;
typedef Dg::R3::VQS<Real>                   vqs;
typedef Dg::R3::Quaternion<Real>            quat;
typedef Dg::R3::Ray<Real>                   ray;

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
  Dg::R3::CPPointRay<Real> dcpPointRay;
  Dg::R3::CPPointRay<Real>::Result dcpPointRay_res;
  vec pIn;

  //Point in front of ray
  pIn.Set(7.0, -12.0, 35.0, 1.0);
  dcpPointRay_res = dcpPointRay(pIn, r0);
  CHECK(dcpPointRay_res.u == 7.0);
  //CHECK(dcpPointRay_res.distance = 37.0);
  //CHECK(dcpPointRay_res.sqDistance = 37.0 * 37.0);
  CHECK(dcpPointRay_res.cp == vec(7.0, 0.0, 0.0, 1.0));

  //Point behind ray
  pIn.Set(-2.0, -3.0, 6.0, 1.0);
  dcpPointRay_res = dcpPointRay(pIn, r0);
  CHECK(dcpPointRay_res.u == 0.0);
  //CHECK(dcpPointRay_res.distance = 7.0);
  //CHECK(dcpPointRay_res.sqDistance = 49.0);
  CHECK(dcpPointRay_res.cp == r0.Origin());

  //Ray - Ray
  Dg::R3::CPRayRay<Real> dcpRayRay;
  Dg::R3::CPRayRay<Real>::Result dcpRayRay_res;

  //Rays parallel, no overlap, facing opposite directions
  r1.Set(vec(-3.0, 8.0, 36.0, 1.0), vec(-1.0, 0.0, 0.0, 0.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == Dg::QueryCode::QC_Success);
  CHECK(dcpRayRay_res.u0 == 0.0);
  CHECK(dcpRayRay_res.u1 == 0.0);
  CHECK(dcpRayRay_res.cp0 == r0.Origin());
  CHECK(dcpRayRay_res.cp1 == r1.Origin());
  //CHECK(dcpRayRay_res.distance == 37.0);
  //CHECK(dcpRayRay_res.sqDistance == 37.0 * 37.0);

  //Rays parallel, overlap, facing opposite directions
  r1.Set(vec(4.0, 13.0, 84.0, 1.0), vec(-1.0, 0.0, 0.0, 0.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == Dg::QueryCode::QC_Overlapping);
  CHECK(dcpRayRay_res.u0 == 0.0);
  CHECK(dcpRayRay_res.u1 == 4.0);
  CHECK(dcpRayRay_res.cp0 == r0.Origin());
  CHECK(dcpRayRay_res.cp1 == vec(0.0, 13.0, 84.0, 1.0));
  //CHECK(dcpRayRay_res.distance == 85.0);
  //CHECK(dcpRayRay_res.sqDistance == 85.0 * 85.0);

  //Rays parallel, overlap, facing same direction
  r1.Set(vec(4.0, 3.0, 4.0, 1.0), vec(1.0, 0.0, 0.0, 0.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == Dg::QueryCode::QC_Overlapping);
  CHECK(dcpRayRay_res.u0 == 4.0);
  CHECK(dcpRayRay_res.u1 == 0.0);
  CHECK(dcpRayRay_res.cp0 == vec(4.0, 0.0, 0.0, 1.0));
  CHECK(dcpRayRay_res.cp1 == r1.Origin());
  //CHECK(dcpRayRay_res.distance == 5.0);
  //CHECK(dcpRayRay_res.sqDistance == 25.0);

  //Rays parallel, overlap, facing same direction, switch rays
  r1.Set(vec(4.0, 3.0, 4.0, 1.0), vec(1.0, 0.0, 0.0, 0.0));
  dcpRayRay_res = dcpRayRay(r1, r0);
  CHECK(dcpRayRay_res.code == Dg::QueryCode::QC_Overlapping);
  CHECK(dcpRayRay_res.u1 == 4.0);
  CHECK(dcpRayRay_res.u0 == 0.0);
  CHECK(dcpRayRay_res.cp1 == vec(4.0, 0.0, 0.0, 1.0));
  CHECK(dcpRayRay_res.cp0 == r1.Origin());
  //CHECK(dcpRayRay_res.distance == 5.0);
  //CHECK(dcpRayRay_res.sqDistance == 25.0);

  //Rays not parallel, cp are along r0, r1.Origin.
  r1.Set(vec(3.0, 4.0, 3.0, 1.0), vec(0.0, 0.0, 1.0, 0.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == Dg::QueryCode::QC_Success);
  CHECK(dcpRayRay_res.u0 == 3.0);
  CHECK(dcpRayRay_res.u1 == 0.0);
  CHECK(dcpRayRay_res.cp0 == vec(3.0, 0.0, 0.0, 1.0));
  CHECK(dcpRayRay_res.cp1 == r1.Origin());
  //CHECK(dcpRayRay_res.distance == 5.0);
  //CHECK(dcpRayRay_res.sqDistance == 25.0);

  //Rays not parallel, cp are along both rays
  r1.Set(vec(3.0, 4.0, 3.0, 1.0), vec(0.0, 0.0, -1.0, 0.0));
  dcpRayRay_res = dcpRayRay(r0, r1);
  CHECK(dcpRayRay_res.code == Dg::QueryCode::QC_Success);
  CHECK(dcpRayRay_res.u0 == 3.0);
  CHECK(dcpRayRay_res.u1 == 3.0);
  CHECK(dcpRayRay_res.cp0 == vec(3.0, 0.0, 0.0, 1.0));
  CHECK(dcpRayRay_res.cp1 == vec(3.0, 4.0, 0.0, 1.0));
  //CHECK(dcpRayRay_res.distance == 4.0);
  //CHECK(dcpRayRay_res.sqDistance == 16.0);

  //Line - Ray
  line ln;
  Dg::R3::CPRayLine<Real> dcpRayLine;
  Dg::R3::CPRayLine<Real>::Result dcpRayLine_res;

  //Line-Ray Parallel, same direction
  ln.Set(vec(5.0, 11.0, 60.0, 1.0), vec(1.0, 0.0, 0.0, 0.0));
  dcpRayLine_res = dcpRayLine(r0, ln);
  CHECK(dcpRayLine_res.code == Dg::QueryCode::QC_Parallel);
  CHECK(dcpRayLine_res.ur == 0.0);
  CHECK(dcpRayLine_res.ul == -5.0);
  CHECK(dcpRayLine_res.cpr == r0.Origin());
  CHECK(dcpRayLine_res.cpl == vec(0.0, 11.0, 60.0, 1.0));
  //CHECK(dcpRayLine_res.distance == 61.0);
  //CHECK(dcpRayLine_res.sqDistance == 61.0 * 61.0);

  //Line-Ray Parallel, opposite directions
  ln.Set(vec(5.0, 11.0, 60.0, 1.0), vec(-1.0, 0.0, 0.0, 0.0));
  dcpRayLine_res = dcpRayLine(r0, ln);
  CHECK(dcpRayLine_res.code == Dg::QueryCode::QC_Parallel);
  CHECK(dcpRayLine_res.ur == 0.0);
  CHECK(dcpRayLine_res.ul == 5.0);
  CHECK(dcpRayLine_res.cpr == r0.Origin());
  CHECK(dcpRayLine_res.cpl == vec(0.0, 11.0, 60.0, 1.0));
  //CHECK(dcpRayLine_res.distance == 61.0);
  //CHECK(dcpRayLine_res.sqDistance == 61.0 * 61.0);

  //Line-Ray not parallel, closest point is ray origin
  ln.Set(vec(-9.0, 40.0, 61.0, 1.0), vec(0.0, 0.0, -1.0, 0.0));
  dcpRayLine_res = dcpRayLine(r0, ln);
  CHECK(dcpRayLine_res.code == Dg::QueryCode::QC_Success);
  CHECK(dcpRayLine_res.ur == 0.0);
  CHECK(dcpRayLine_res.ul == 61.0);
  CHECK(dcpRayLine_res.cpr == r0.Origin());
  CHECK(dcpRayLine_res.cpl == vec(-9.0, 40.0, 0.0, 1.0));
  //CHECK(dcpRayLine_res.distance == 41.0);
  //CHECK(dcpRayLine_res.sqDistance == 41.0 * 41.0);

  //Line-Ray not parallel, closest point is along the ray
  ln.Set(vec(9.0, 40.0, -61.0, 1.0), vec(0.0, 0.0, 1.0, 0.0));
  dcpRayLine_res = dcpRayLine(r0, ln);
  CHECK(dcpRayLine_res.code == Dg::QueryCode::QC_Success);
  CHECK(dcpRayLine_res.ur == 9.0);
  CHECK(dcpRayLine_res.ul == 61.0);
  CHECK(dcpRayLine_res.cpr == vec(9.0, 0.0, 0.0, 1.0));
  CHECK(dcpRayLine_res.cpl == vec(9.0, 40.0, 0.0, 1.0));
  //CHECK(dcpRayLine_res.distance == 40.0);
  //CHECK(dcpRayLine_res.sqDistance == 1600.0);

  //Ray-Plane
  Dg::R3::TIRayPlane<Real>          tiRayPlane;
  Dg::R3::FIRayPlane<Real>          fiRayPlane;
  Dg::R3::TIRayPlane<Real>::Result  tiRayPlane_res;
  Dg::R3::FIRayPlane<Real>::Result  fiRayPlane_res;
  plane pl;

  //Ray not intersecting plane, pointing away from plane
  pl.Set(vec(-1.0, 0.0, 0.0, 0.0), vec(-1.0, 0.0, 0.0, 1.0));
  tiRayPlane_res = tiRayPlane(r0, pl);
  CHECK(tiRayPlane_res.isIntersecting == false);

  fiRayPlane_res = fiRayPlane(r0, pl);
  CHECK(fiRayPlane_res.code == Dg::QueryCode::QC_NotIntersecting);
  CHECK(fiRayPlane_res.point == r0.Origin());
  CHECK(fiRayPlane_res.u == 0.0);

  //Ray not intersecting plane, parallel to plane
  pl.Set(vec(0.0, 1.0, 0.0, 0.0), vec(0.0, 1.0, 0.0, 1.0));
  tiRayPlane_res = tiRayPlane(r0, pl);
  CHECK(tiRayPlane_res.isIntersecting == false);

  fiRayPlane_res = fiRayPlane(r0, pl);
  CHECK(fiRayPlane_res.code == Dg::QueryCode::QC_NotIntersecting);
  CHECK(fiRayPlane_res.point == r0.Origin());
  CHECK(fiRayPlane_res.u == 0.0);

  //Ray lies on the plane
  pl.Set(vec(0.0, 1.0, 0.0, 0.0), vec(0.0, 0.0, 0.0, 1.0));
  tiRayPlane_res = tiRayPlane(r0, pl);
  CHECK(tiRayPlane_res.isIntersecting == true);

  fiRayPlane_res = fiRayPlane(r0, pl);
  CHECK(fiRayPlane_res.code == Dg::QueryCode::QC_Overlapping);
  CHECK(fiRayPlane_res.point == r0.Origin());
  CHECK(fiRayPlane_res.u == 0.0);

  //Ray intersects plane
  pl.Set(vec(1.0, 0.0, 0.0, 0.0), vec(3.0, 0.0, 0.0, 1.0));
  tiRayPlane_res = tiRayPlane(r0, pl);
  CHECK(tiRayPlane_res.isIntersecting == true);

  fiRayPlane_res = fiRayPlane(r0, pl);
  CHECK(fiRayPlane_res.code == Dg::QueryCode::QC_Intersecting);
  CHECK(fiRayPlane_res.point == vec(3.0, 0.0, 0.0, 1.0));
  CHECK(fiRayPlane_res.u == 3.0);

  //Ray intersects plane
  r0.Set(vec(6.0, 2.3, -5.6, 1.0), vec::xAxis());
  pl.Set(vec::xAxis(), vec(10.0, 0.0, 0.0, 1.0));
  tiRayPlane_res = tiRayPlane(r0, pl);
  CHECK(tiRayPlane_res.isIntersecting == true);

  fiRayPlane_res = fiRayPlane(r0, pl);
  CHECK(fiRayPlane_res.code == Dg::QueryCode::QC_Intersecting);
  CHECK(fiRayPlane_res.point == vec(10.0, 2.3, -5.6, 1.0));
  CHECK(fiRayPlane_res.u == 4.0);

}

TEST(Stack_DgRayTransform, DgRayTransform)
{
  ray x0(vec(2.0, 0.0, 0.0, 1.0), vec::xAxis());
  ray x1 = x0;
  ray x2 = x0;
  ray xFinal(vec(1.0, 2.0, 7.0, 1.0), vec::zAxis());

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