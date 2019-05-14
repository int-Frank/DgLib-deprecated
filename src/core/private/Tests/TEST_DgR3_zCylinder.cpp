#include "TestHarness.h"
#include "DgR3zCylinder.h"
#include "query/DgR3QueryPointZCylinder.h"

typedef Dg::R3::Vector<float>      vec;
typedef Dg::R3::ZCylinder<float>   zCyl;

TEST(Stack_DgZCyl0, DgDgZCyl0)
{
  zCyl zc(vec(1.0f, 2.0f, 3.0f, 3.0f), 2.0f, 23.0f);

  CHECK(zc.GetHeight() == 23.0f);
  CHECK(zc.GetRadius() == 2.0f);
  CHECK(zc.GetBottomCenter() == vec(1.0f, 2.0f, 3.0f, 1.0f));

  zc.SetRadius(1.0f);
  zc.SetHeight(2.0f);
  zc.SetBottom(vec(1.0f, 1.0f, 1.0f, -3.0f));

  CHECK(zc.GetHeight() == 2.0f);
  CHECK(zc.GetRadius() == 1.0f);
  CHECK(zc.GetBottomCenter() == vec(1.0f, 1.0f, 1.0f, 1.0f));

  vec p = vec::Origin();

  Dg::R3::CPPointZCylinder<float> query;
  Dg::R3::CPPointZCylinder<float>::Result result = query(p, zc);
  
  float cos45 = 0.70710678118654752440084436210485f;  //or 1/sqrt(2)
  float sqrt2 = 1.4142135623730950488016887242097f;

  //outside infinite cylinder, below
  float val = (sqrt2 - 1.0f)*cos45;
  CHECK(result.cp == vec(val, val, 1.0f, 1.0f));
  CHECK(result.code = Dg::QC_NotIntersecting);

  //outside infinite cylinder, above
  p.Set(0.0f, 0.0f, 10.0f, 1.0f);
  result = query(p, zc);
  CHECK(result.cp == vec(val, val, 3.0f, 1.0f));
  CHECK(result.code = Dg::QC_NotIntersecting);

  //outside infinite cylinder, inner
  p.Set(3.0f, 3.0f, 2.0f, 1.0f);
  val = cos45 + 1.0f;
  result = query(p, zc);
  CHECK(result.cp == vec(val, val, 2.0f, 1.0f));
  CHECK(result.code = Dg::QC_NotIntersecting);

  //inside infinite cylinder, above
  p.Set(1.0f, 1.0f, 12.0f, 1.0f);
  result = query(p, zc);
  CHECK(result.cp == vec(1.0f, 1.0f, 3.0f, 1.0f));
  CHECK(result.code = Dg::QC_NotIntersecting);

  //inside infinite cylinder, below
  p.Set(1.0f, 1.0f, -12.0f, 1.0f);
  result = query(p, zc);
  CHECK(result.cp == vec(1.0f, 1.0f, 1.0f, 1.0f));
  CHECK(result.code = Dg::QC_NotIntersecting);

  //On cylinder top boundary
  p.Set(1.0f, 1.0f, 3.0f, 1.0f);
  result = query(p, zc);
  CHECK(result.cp == p);
  CHECK(result.code = Dg::QC_Intersecting);

  //Inside
  p.Set(1.0f, 1.0f, 1.5f, 1.0f);
  result = query(p, zc);
  CHECK(result.cp == p);
  CHECK(result.code = Dg::QC_Intersecting);

  //On cylinder bottom boundary
  p.Set(1.0f, 1.0f, 1.0f, 1.0f);
  result = query(p, zc);
  CHECK(result.cp == p);
  CHECK(result.code = Dg::QC_Intersecting);
}
