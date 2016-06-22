#include "TestHarness.h"
#include "DgLine.h"
#include "query/DgQueryLineLine.h"
#include "query/DgQueryPointLine.h"
#include "query/DgQueryLinePlane.h"

typedef double                          Real;
typedef Dg::Vector4<Real>               vec;
typedef Dg::Plane<Real>                 plane;
typedef Dg::Matrix44<Real>              mat44;
typedef Dg::VQS<Real>                   vqs;
typedef Dg::Quaternion<Real>            quat;
typedef Dg::Line<Real>                  line;

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

  //Geometric tests

  //Line-Point
  Dg::DCPPointLine<Real>          dcpPointLine;
  Dg::DCPPointLine<Real>::Result  dcpPointLine_res;

  dcpPointLine_res = dcpPointLine(vec(7.0, -3.0, 4.0, 1.0), l0);
  CHECK(dcpPointLine_res.u == 7.0);
  CHECK(dcpPointLine_res.cp == vec(7.0, 0.0, 0.0, 1.0));
  CHECK(dcpPointLine_res.distance == 5.0);
  CHECK(dcpPointLine_res.sqDistance == 25);

  //Line-Line
  Dg::DCPLineLine<Real>             dcpLineLine;
  Dg::DCPLineLine<Real>::Result     dcpLineLine_res;

  //Lines parallel
  l1.Set(vec(1.0, 1.0, 0.0, 1.0), vec(1.0, 0.0, 0.0, 0.0));
  dcpLineLine_res = dcpLineLine(l0, l1);
  CHECK(dcpLineLine_res.code == 1);
  CHECK(dcpLineLine_res.cp0 == l0.Origin());
  CHECK(dcpLineLine_res.cp1 == vec(0.0, 1.0, 0.0, 1.0));
  CHECK(dcpLineLine_res.sqDistance == 1.0);
  CHECK(dcpLineLine_res.distance == 1.0);

  //Lines not parallel
  l1.Set(vec(1.0, 1.0, 0.0, 1.0), vec(0.0, 0.0, 1.0, 0.0));
  dcpLineLine_res = dcpLineLine(l0, l1);
  CHECK(dcpLineLine_res.code == 0);
  CHECK(dcpLineLine_res.cp0 == vec(1.0, 0.0, 0.0, 1.0));
  CHECK(dcpLineLine_res.cp1 == vec(1.0, 1.0, 0.0, 1.0));
  CHECK(dcpLineLine_res.sqDistance == 1.0);
  CHECK(dcpLineLine_res.distance == 1.0);

  //Line-Plane
  Dg::TILinePlane<Real>          tiLinePlane;
  Dg::FILinePlane<Real>          fiLinePlane;
  Dg::TILinePlane<Real>::Result  tiLinePlane_res;
  Dg::FILinePlane<Real>::Result  fiLinePlane_res;
  plane pl;

  //Line parallel to plane
  pl.Set(vec(0.0, 1.0, 0.0, 0.0), -1.0);
  tiLinePlane_res = tiLinePlane(l0, pl);
  CHECK(tiLinePlane_res.isIntersecting == false);

  fiLinePlane_res = fiLinePlane(l0, pl);
  CHECK(fiLinePlane_res.code == 2);
  CHECK(fiLinePlane_res.point == l0.Origin());
  CHECK(fiLinePlane_res.u == 0.0);

  //Line on plane
  pl.Set(vec(0.0, 1.0, 0.0, 0.0), 0.0);
  tiLinePlane_res = tiLinePlane(l0, pl);
  CHECK(tiLinePlane_res.isIntersecting == true);

  fiLinePlane_res = fiLinePlane(l0, pl);
  CHECK(fiLinePlane_res.code == 1);
  CHECK(fiLinePlane_res.point == l0.Origin());
  CHECK(fiLinePlane_res.u == 0.0);

  //Line intersecting plane
  pl.Set(vec(1.0, 0.0, 0.0, 0.0), -3.0);
  tiLinePlane_res = tiLinePlane(l0, pl);
  CHECK(tiLinePlane_res.isIntersecting == true);

  fiLinePlane_res = fiLinePlane(l0, pl);
  CHECK(fiLinePlane_res.code == 0);
  CHECK(fiLinePlane_res.point == vec(3.0, 0.0, 0.0, 1.0));
  CHECK(fiLinePlane_res.u == 3.0);
}


TEST(Stack_DgLineTransform, DgLineTransform)
{
  line x0(vec(2.0, 0.0, 0.0, 1.0), vec::xAxis());
  line x1 = x0;
  line x2 = x0;
  line xFinal(vec(1.0, 2.0, 7.0, 1.0), vec::zAxis());

  Real scale = 2.0;
  vec trans(1.0, 2.0, 3.0, 0.0);
  Real rx = Dg::PI / 2.0;
  Real ry = 0.0;
  Real rz = Dg::PI / 2.0;
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

  x1 *= m;
  x2 = x0 * m;
  CHECK(x1 == x2);
  CHECK(x1 == xFinal);

  x1 = x0;
  x1 *= v;
  x2 = x0 * v;
  CHECK(x1 == x2);
  CHECK(x1 == xFinal);
}