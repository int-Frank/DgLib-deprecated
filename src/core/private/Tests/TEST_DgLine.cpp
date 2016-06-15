#include "TestHarness.h"
#include "DgLine.h"
#include "query/DgQueryLineLine.h"
#include "query/DgQueryPointLine.h"
#include "query/DgQueryLinePlane.h"

typedef double                          Real;
typedef Dg::Vector4<Real>               vec;
typedef Dg::Plane<Real>                 plane;
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