#include "TestHarness.h"
#include "DgR3Frustum.h"
#include "DgR3Sphere.h"
#include "query/DgR3QueryFrustumSphere.h"

typedef double Real;
typedef Dg::R3::Vector4<Real>      vec;
typedef Dg::R3::Sphere<Real>       sphere;
typedef Dg::R3::Frustum<Real>      frustum;
typedef Dg::R3::FrustumData<Real>  frustumData;

TEST(Stack_DgFrustum, DgFrustum)
{
  frustum f;
  frustumData fData;
  f.GetData(fData);

  sphere s;

  Dg::R3::TIFrustumSphere<Real> tiFS;
  Dg::R3::TIFrustumSphere<Real>::Result tiFS_res = tiFS(fData, s);

}