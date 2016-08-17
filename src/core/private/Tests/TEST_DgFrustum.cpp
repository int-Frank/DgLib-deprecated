#include "TestHarness.h"
#include "DgFrustum.h"
#include "DgSphere.h"
#include "query/DgQueryFrustumSphere.h"

typedef double Real;
typedef Dg::Vector4<Real>      vec;
typedef Dg::Sphere<Real>       sphere;
typedef Dg::Frustum<Real>      frustum;
typedef Dg::FrustumData<Real>  frustumData;

TEST(Stack_DgFrustum, DgFrustum)
{
  frustum f;
  frustumData fData;
  f.GetData(fData);

  sphere s;

  Dg::TIFrustumSphere<Real> tiFS;
  Dg::TIFrustumSphere<Real>::Result tiFS_res = tiFS(fData, s);

}