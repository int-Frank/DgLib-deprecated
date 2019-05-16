#include "TestHarness.h"
#include "DgR3Rectangle.h"
#include "DgR3Vector.h"
#include "query/DgR3QueryRectanglePoint.h"
#include "query/DgR3QueryRectangleSphere.h"
#include "query/DgR3QueryRectangleLine.h"

typedef Dg::R3::Rectangle<float>  Rect;
typedef Dg::R3::Vector<float>     vec;
typedef Dg::R3::Sphere<float>     Sphere;
typedef Dg::R3::Line<float>       Line;

TEST(Stack_DgRectangle, DgRectangle)
{
  Rect r;
  CHECK(r.GetCenter() == vec::Origin());
  CHECK(r.GetAxis(0) == vec::xAxis());
  CHECK(r.GetAxis(1) == vec::yAxis());
  CHECK(r.GetExtent(0) == 1.0f);
  CHECK(r.GetExtent(1) == 1.0f);

  r.SetCenter(vec(1.0f, 2.0f, 3.0f, 1.0f));
  r.SetExtent(0, 2.0f);
  r.SetExtent(1, 3.0f);
  r.SetAxis(0, vec::zAxis());
  r.SetAxis(1, -vec::xAxis());

  CHECK(r.GetCenter() == vec(1.0f, 2.0f, 3.0f, 1.0f));
  CHECK(r.GetAxis(0) == vec::zAxis());
  CHECK(r.GetAxis(1) == -vec::xAxis());
  CHECK(r.GetExtent(0) == 2.0f);
  CHECK(r.GetExtent(1) == 3.0f);

  vec verts[4];
  r.GetVertices(verts[0], verts[1], verts[2], verts[3]);
  CHECK(verts[0] == vec(-2.0f, 2.0f, 1.0f, 1.0f));
  CHECK(verts[1] == vec(4.0f, 2.0f, 1.0f, 1.0f));
  CHECK(verts[2] == vec(-2.0f, 2.0f, 5.0f, 1.0f));
  CHECK(verts[3] == vec(4.0f, 2.0f, 5.0f, 1.0f));

  vec p(0.0f, 0.0f, 0.0f, 1.0f);
  Dg::R3::CPRectangleCylinder<float>  query;
  Dg::R3::CPRectangleCylinder<float>::Result result = query(r, p);
  vec cp(0.0f, 2.0f, 1.0f, 1.0f);
  vec v = cp - p;
  CHECK(result.cp == cp);
  CHECK(result.sqDistance == v.LengthSquared());
  CHECK(result.rectangleParameter[0] == -2.0f);
  CHECK(result.rectangleParameter[1] == 1.0f);

  p.Set(0.0f, 0.0f, 3.0f, 1.0f);
  result = query(r, p);
  cp.Set(0.0f, 2.0f, 3.0f, 1.0f);
  v = cp - p;
  CHECK(result.cp == cp);
  CHECK(result.sqDistance == v.LengthSquared());
  CHECK(result.rectangleParameter[0] == 0.0f);
  CHECK(result.rectangleParameter[1] == 1.0f);

  p.Set(10.0f, 0.0f, 30.0f, 1.0f);
  result = query(r, p);
  cp.Set(4.0f, 2.0f, 5.0f, 1.0f);
  v = cp - p;
  CHECK(result.cp == cp);
  CHECK(result.sqDistance == v.LengthSquared());
  CHECK(result.rectangleParameter[0] == 2.0f);
  CHECK(result.rectangleParameter[1] == -3.0f);

  p.Set(-10.0f, 10.0f, 30.0f, 1.0f);
  result = query(r, p);
  cp.Set(-2.0f, 2.0f, 5.0f, 1.0f);
  v = cp - p;
  CHECK(result.cp == cp);
  CHECK(result.sqDistance == v.LengthSquared());
  CHECK(result.rectangleParameter[0] == 2.0f);
  CHECK(result.rectangleParameter[1] == 3.0f);

  p.Set(-10.0f, 0.0f, 0.0f, 1.0f);
  result = query(r, p);
  cp.Set(-2.0f, 2.0f, 1.0f, 1.0f);
  v = cp - p;
  CHECK(result.cp == cp);
  CHECK(result.sqDistance == v.LengthSquared());
  CHECK(result.rectangleParameter[0] == -2.0f);
  CHECK(result.rectangleParameter[1] == 3.0f);
}

TEST(Stack_DgRectangle_querySphere, DgRectangle_querySphere)
{
  Rect r;
  r.SetCenter(vec(1.0f, 2.0f, 3.0f, 1.0f));
  r.SetExtent(0, 2.0f);
  r.SetExtent(1, 3.0f);
  r.SetAxis(0, vec::zAxis());
  r.SetAxis(1, -vec::xAxis());

  Sphere s;
  s.SetCenter(vec::Origin());
  s.SetRadius(1.0f);

  Dg::R3::TIRectangleSphere<float> query;
  Dg::R3::TIRectangleSphere<float>::Result result = query(r, s);
  CHECK(result.code == Dg::QC_NotIntersecting);

  s.SetRadius(4.0f);
  result = query(r, s);
  CHECK(result.code == Dg::QC_Intersecting);

  s.SetRadius(2.0f);
  s.SetCenter(vec(0.0f, 0.0f, 3.0f, 1.0f));
  result = query(r, s);
  CHECK(result.code == Dg::QC_Intersecting);

  s.SetRadius(1.9f);
  s.SetCenter(vec(0.0f, 0.0f, 3.0f, 1.0f));
  result = query(r, s);
  CHECK(result.code == Dg::QC_NotIntersecting);
}

TEST(Stack_DgRectangle_queryLine, DgRectangle_queryLine)
{
  Rect r;
  r.SetCenter(vec(1.0f, 2.0f, 3.0f, 1.0f));
  r.SetExtent(0, 2.0f);
  r.SetExtent(1, 3.0f);
  r.SetAxis(0, vec::zAxis());
  r.SetAxis(1, -vec::xAxis());

  Dg::R3::FIRectangleLine<float> query;
  Dg::R3::FIRectangleLine<float>::Result result;
  Line l;

  //No intersections...
  l.Set(vec(-3.0f, 0.0f, 0.0f, 1.0f), vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(0.0f, 0.0f, 0.0f, 1.0f), vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(5.0f, 0.0f, 0.0f, 1.0f), vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(-5.0f, 0.0f, 3.0f, 1.0f), vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(5.0f, 0.0f, 3.0f, 1.0f), vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(-5.0f, 0.0f, 33.0f, 1.0f), vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(1.0f, 0.0f, 33.0f, 1.0f), vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(10.0f, 0.0f, 33.0f, 1.0f), vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  //Now the same but from the opposite side
  l.Set(vec(-3.0f, 10.0f, 0.0f, 1.0f), -vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(0.0f, 20.0f, 0.0f, 1.0f), -vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(5.0f, 30.0f, 0.0f, 1.0f), -vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(-5.0f, 10.0f, 3.0f, 1.0f), -vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(5.0f, 20.0f, 3.0f, 1.0f), -vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(-5.0f, 30.0f, 33.0f, 1.0f), -vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(1.0f, 10.0f, 33.0f, 1.0f), -vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(10.0f, 20.0f, 33.0f, 1.0f), -vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  //Intersecting...

  //Absolutely intersecting..
  l.Set(vec(-1.0f, 0.0f, 2.0f, 1.0f), vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(-1.0f, 2.0f, 2.0f, 1.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == 2.0f);

  l.Set(vec(-1.0f, 0.0f, 4.0f, 1.0f), vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(-1.0f, 2.0f, 4.0f, 1.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 1.0f);
  CHECK(result.rectangleParameter[1] == 2.0f);

  l.Set(vec(3.0f, 0.0f, 4.0f, 1.0f), vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(3.0f, 2.0f, 4.0f, 1.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 1.0f);
  CHECK(result.rectangleParameter[1] == -2.0f);

  l.Set(vec(3.0f, 0.0f, 2.0f, 1.0f), vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(3.0f, 2.0f, 2.0f, 1.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == -2.0f);

  //From the other side...
  l.Set(vec(-1.0f, 4.0f, 2.0f, 1.0f), -vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(-1.0f, 2.0f, 2.0f, 1.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == 2.0f);

  l.Set(vec(-1.0f, 4.0f, 4.0f, 1.0f), -vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(-1.0f, 2.0f, 4.0f, 1.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 1.0f);
  CHECK(result.rectangleParameter[1] == 2.0f);

  l.Set(vec(3.0f, 4.0f, 4.0f, 1.0f), -vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(3.0f, 2.0f, 4.0f, 1.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 1.0f);
  CHECK(result.rectangleParameter[1] == -2.0f);

  l.Set(vec(3.0f, 4.0f, 2.0f, 1.0f), -vec::yAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(3.0f, 2.0f, 2.0f, 1.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == -2.0f);

  //Parallel, not intersecting...

  l.Set(vec(1.0f, 1.0f, 1.5f, 1.0f), vec::xAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  l.Set(vec(1.0f, 2.0f, 0.5f, 1.0f), vec::xAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_NotIntersecting);

  //Parallel, intersecting
  //line direction is xAxis
  l.Set(vec(1.0f, 2.0f, 2.0f, 1.0f), vec::xAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(-2.0f, 2.0f, 2.0f, 1.0f));
  CHECK(result.u == -3.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == 3.0f);

  l.Set(vec(-3.0f, 2.0f, 2.0f, 1.0f), vec::xAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(-2.0f, 2.0f, 2.0f, 1.0f));
  CHECK(result.u == 1.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == 3.0f);

  l.Set(vec(5.0f, 2.0f, 2.0f, 1.0f), -vec::xAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(4.0f, 2.0f, 2.0f, 1.0f));
  CHECK(result.u == 1.0f);
  CHECK(result.rectangleParameter[0] == -1.0f);
  CHECK(result.rectangleParameter[1] == -3.0f);

  //line direction is zAxis
  l.Set(vec(1.0f, 2.0f, -1.0f, 1.0f), vec::zAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(1.0f, 2.0f, 1.0f, 1.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == -2.0f);
  CHECK(result.rectangleParameter[1] == 0.0f);

  l.Set(vec(1.0f, 2.0f, 7.0f, 1.0f), -vec::zAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(1.0f, 2.0f, 5.0f, 1.0f));
  CHECK(result.u == 2.0f);
  CHECK(result.rectangleParameter[0] == 2.0f);
  CHECK(result.rectangleParameter[1] == 0.0f);

  //On rectangle edge
  l.Set(vec(-3.0f, 2.0f, 1.0f, 1.0f), vec::xAxis());
  result = query(r, l);
  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.point == vec(-2.0f, 2.0f, 1.0f, 1.0f));
  CHECK(result.u == 1.0f);
  CHECK(result.rectangleParameter[0] == -2.0f);
  CHECK(result.rectangleParameter[1] == 3.0f);
}