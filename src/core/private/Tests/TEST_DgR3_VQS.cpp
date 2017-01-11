#include "TestHarness.h"
#include "DgR3Vector.h"
#include "DgR3Quaternion.h"
#include "DgR3Matrix.h"
#include "DgR3VQS.h"

typedef Dg::R3::Matrix< double >     mat44;
typedef Dg::R3::Quaternion < double >  quat;
typedef Dg::R3::Vector < double >     vec4;
typedef Dg::R3::VQS<double>            VQS;

//--------------------------------------------------------------------------------
//	VQS Construction
//--------------------------------------------------------------------------------
TEST(Stack_VQS_Construction, creation_VQS_Construction)
{
  vec4 v({ 23.4, -84.2, 0.2134, 0.0f }), v0;
  quat q, q0;
  q.SetRotation(2.45, 0.8354, -1.345, Dg::EulerOrder::XYX);
  double s(2.3), s0;

  VQS vqs0, vqs3;
  VQS vqs1(v, q, s);
  vqs1.MakeValid();

  VQS vqs2(vqs1);
  CHECK(vqs2 == vqs1);
  CHECK(vqs2 != vqs0);

  vqs2 = vqs1;
  CHECK(vqs2 == vqs1);

  vqs2.Set(v, q, s);
  CHECK(vqs2 == vqs1);

  vqs2.SetV(v);
  vqs2.SetQ(q);
  vqs2.SetS(s);
  CHECK(vqs2 == vqs1);

  vqs2.Get(v0, q0, s0);
  CHECK(v == v0);
  CHECK(q == q0);
  CHECK(s == s0);

  vqs0.Identity();
  vqs2.Identity();
  vqs3.Identity();

  vqs0.SetV(v);
  vqs2.SetQ(q);
  vqs3.SetS(s);
  CHECK((vqs3 * vqs2 * vqs0) == vqs1);

  vqs0.Identity();
  vqs2.Identity();
  vqs3.Identity();

  vqs0.SetV(v);
  vqs2.SetQ(q);
  vqs3.SetS(s);
  vqs3 *= vqs2;
  vqs3 *= vqs0;
  CHECK(vqs3 == vqs1);

}

//--------------------------------------------------------------------------------
//	VQS VectorTransform
//--------------------------------------------------------------------------------
TEST(Stack_VQS_VectorTransform, creation_VQS_VectorTransform)
{
  quat q;
  VQS T_A_WLD, T_B_A;
  mat44 M_A_WLD, M_B_A, mr, mt, ms;

  q.SetRotationY(-1.453);
  T_A_WLD.Set(vec4( 4.0, -2.56, 1.895, 0.0 ), q, 1.534);
  mr.RotationY(-1.453);
  ms.Scaling(1.534);
  mt.Translation(vec4( 4.0, -2.56, 1.895, 0.0 ));
  M_A_WLD = ms * mr * mt;

  q.SetRotationX(0.7473);
  T_B_A.Set(vec4( 1.73, -2.8567, 9.347, 0.0 ), q, 2.747);
  mr.RotationX(0.7473);
  ms.Scaling(2.747);
  mt.Translation(vec4( 1.73, -2.8567, 9.347, 0.0 ));
  M_B_A = ms * mr * mt;

  vec4 result_VQS, result_M;
  result_VQS = (T_B_A * T_A_WLD).TransformVector(vec4( 2.0, 0.0, 0.0, 0.0 ));
  result_M = vec4( 2.0, 0.0, 0.0, 0.0 ) * (M_B_A * M_A_WLD);

  CHECK(result_VQS == result_M);

}