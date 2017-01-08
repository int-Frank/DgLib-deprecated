#include "TestHarness.h"
#include "DgR3Vector4.h"
#include "DgR3Quaternion.h"
#include "DgR3Matrix44.h"

typedef Dg::R3::Quaternion < float >  quat;
typedef Dg::R3::Vector4 < float >     vec4;
typedef Dg::R3::Matrix44<float>       mat44;

//--------------------------------------------------------------------------------
//	Quaternion Construction
//--------------------------------------------------------------------------------
TEST(Stack_Quaternion_Construction, creation_Quaternion_Construction)
{
  float w = 2.3f, x = -1.5f, y = 4.2f, z = 9.0f;

  quat q0;
  quat q1(w, x, y, z);

  CHECK((q0[0] == 1.0f) && (q0[1] == 0.0f) && (q0[2] == 0.0f) && (q0[3] == 0.0f));
  CHECK((q1[0] == w) && (q1[1] == x) && (q1[2] == y) && (q1[3] == z));

  q0.Set(w, x, y, z);

  CHECK(q0 == q1);
  CHECK(!(q0 != q1));

  q1.MakeValid();

  CHECK(Dg::IsZero(q1.Magnitude() - 1.0f));
  CHECK(Dg::IsZero(q1.Norm() - 1.0f));
  CHECK(q1.IsUnit());
  CHECK(!q1.IsIdentity());

  q1.Set(w, x, y, z);

  q1.Normalize();
  CHECK(q1.IsUnit());

  q0.Zero();
  CHECK(Dg::IsZero(q0.Magnitude()));

  vec4 v1( 1.0f, -1.0f, 1.0f, 0.0f );
  vec4 v2( -1.0f, -1.0f, 1.0f, 0.0f );
  v1.Normalize();
  v2.Normalize();

  quat q2(v1, Dg::Constants<float>::PI * 0.5f);
  quat q3(v1, v2);
  quat q4(v1);

  CHECK((q2[0] == 0.707106769f) && (q2[1] == 0.408248305f) && (q2[2] == -0.408248305f) && (q2[3] == 0.408248305f));
  CHECK((q3[0] == 0.816496551f) && (q3[1] == 0.0f) && (q3[2] == -0.408248276f) && (q3[3] == -0.408248276f));
  CHECK((q4[0] == 0.0f) && (q4[1] == 0.577350259f) && (q4[2] == -0.577350259f) && (q4[3] == 0.577350259f));

  q0 = q2;
  q1 = quat(q2);

  CHECK((q0[0] == 0.707106769f) && (q0[1] == 0.408248305f) && (q0[2] == -0.408248305f) && (q0[3] == 0.408248305f));
  CHECK((q1[0] == 0.707106769f) && (q1[1] == 0.408248305f) && (q1[2] == -0.408248305f) && (q1[3] == 0.408248305f));

  // From-To ///////////////////////////

  v1.Set( 1.0f, 0.0f, 0.0f, 0.0f );
  v2.Set( 0.0f, 0.0f, 1.0f, 0.0f );
  q0.Set(v1, v2);
  q1.SetRotationY(-Dg::Constants<float>::PI * 0.5f);
  CHECK(q0 == q1);

  v1.Set( 1.0f, 0.0f, 0.0f, 0.0f );
  v2.Set( 1.0f, 0.0f, 1.0f, 0.0f );
  v2.Normalize();
  q0.Set(v1, v2);
  q1.SetRotationY(-Dg::Constants<float>::PI * 0.25f);
  CHECK(q0 == q1);
}

//-------------------------------------------------------------------------------
//		Rotation
//-------------------------------------------------------------------------------
TEST(Stack_Quaternion_Rotation, creation_Quaternion_Rotation)
{
  quat qx, qy, qz, q3, q4;

  float xr = -1.2f;
  float yr = 0.843f;
  float zr = 1.45f;

  vec4 axis;
  float angle;

  qx.SetRotationX(xr);
  qy.SetRotationY(yr);
  qz.SetRotationZ(zr);

  q3 = qz * qy * qx;

  CHECK(q3.IsUnit());

  //Axis-angle

  q3.GetAxisAngle(axis, angle);
  q4.Set(axis, angle);
  CHECK(q4 == q3);

  q4.Set(vec4( 1.0f, 0.0f, 0.0f, 0.0f ), xr);
  CHECK(q4 == qx);

  q4.Set(vec4( 0.0f, 1.0f, 0.0f, 0.0f ), yr);
  CHECK(q4 == qy);

  q4.Set(vec4( 0.0f, 0.0f, 1.0f, 0.0f ), zr);
  CHECK(q4 == qz);

  //Euler angles

  q3.SetRotation(xr, yr, zr, Dg::EulerOrder::XYZ);
  CHECK(q3.IsUnit());
  CHECK(q3 == qx * qy * qz);

  q3.SetRotation(xr, yr, zr, Dg::EulerOrder::XZY);
  CHECK(q3.IsUnit());
  CHECK(q3 == qx * qz * qy);

  q3.SetRotation(xr, yr, zr, Dg::EulerOrder::YZX);
  CHECK(q3.IsUnit());
  CHECK(q3 == qy * qz * qx);

  q3.SetRotation(xr, yr, zr, Dg::EulerOrder::YXZ);
  CHECK(q3.IsUnit());
  CHECK(q3 == qy * qx * qz);

  q3.SetRotation(xr, yr, zr, Dg::EulerOrder::ZXY);
  CHECK(q3.IsUnit());
  CHECK(q3 == qz * qx * qy);

  q3.SetRotation(xr, yr, zr, Dg::EulerOrder::ZYX);
  CHECK(q3.IsUnit());
  CHECK(q3 == qz * qy * qx);

  qx.SetRotationX(xr);
  qy.SetRotationY(yr);
  qz.SetRotationX(zr);
  q3.SetRotation(xr, yr, zr, Dg::EulerOrder::XYX);
  CHECK(q3.IsUnit());
  CHECK(q3 == qx * qy * qz);

  qx.SetRotationX(xr);
  qy.SetRotationZ(yr);
  qz.SetRotationX(zr);
  q3.SetRotation(xr, yr, zr, Dg::EulerOrder::XZX);
  CHECK(q3.IsUnit());
  CHECK(q3 == qx * qy * qz);

  qx.SetRotationY(xr);
  qy.SetRotationX(yr);
  qz.SetRotationY(zr);
  q3.SetRotation(xr, yr, zr, Dg::EulerOrder::YXY);
  CHECK(q3.IsUnit());
  CHECK(q3 == qx * qy * qz);

  qx.SetRotationY(xr);
  qy.SetRotationZ(yr);
  qz.SetRotationY(zr);
  q3.SetRotation(xr, yr, zr, Dg::EulerOrder::YZY);
  CHECK(q3.IsUnit());
  CHECK(q3 == qx * qy * qz);

  qx.SetRotationZ(xr);
  qy.SetRotationX(yr);
  qz.SetRotationZ(zr);
  q3.SetRotation(xr, yr, zr, Dg::EulerOrder::ZXZ);
  CHECK(q3.IsUnit());
  CHECK(q3 == qx * qy * qz);

  qx.SetRotationZ(xr);
  qy.SetRotationY(yr);
  qz.SetRotationZ(zr);
  q3.SetRotation(xr, yr, zr, Dg::EulerOrder::ZYZ);
  CHECK(q3.IsUnit());
  CHECK(q3 == qx * qy * qz);

}


//-------------------------------------------------------------------------------
//		Conversion
//-------------------------------------------------------------------------------
TEST(Stack_Quaternion_Conversion, creation_Quaternion_Conversion)
{
  quat q;
  vec4 v0, v1, v2;

  q.SetRotationX(Dg::Constants<float>::PI / 2.0f);
  q.GetBasis(v0, v1, v2);
  CHECK(v0 == vec4( 1.0f, 0.0f, 0.0f, 0.0f ));
  CHECK(v1 == vec4( 0.0f, 0.0f, 1.0f, 0.0f ));
  CHECK(v2 == vec4( 0.0f, -1.0f, 0.0f, 0.0f ));

  q.SetRotationY(Dg::Constants<float>::PI / 2.0f);
  q.GetBasis(v0, v1, v2);
  CHECK(v0 == vec4( 0.0f, 0.0f, -1.0f, 0.0f ));
  CHECK(v1 == vec4( 0.0f, 1.0f, 0.0f, 0.0f ));
  CHECK(v2 == vec4( 1.0f, 0.0f, 0.0f, 0.0f ));

  q.SetRotationZ(Dg::Constants<float>::PI / 2.0f);
  q.GetBasis(v0, v1, v2);
  CHECK(v0 == vec4( 0.0f, 1.0f, 0.0f, 0.0f ));
  CHECK(v1 == vec4( -1.0f, 0.0f, 0.0f, 0.0f ));
  CHECK(v2 == vec4( 0.0f, 0.0f, 1.0f, 0.0f ));

  q.SetRotationX(-Dg::Constants<float>::PI / 2.0f);
  q.GetBasis(v0, v1, v2);
  CHECK(v0 == vec4( 1.0f, 0.0f, 0.0f, 0.0f ));
  CHECK(v1 == vec4( 0.0f, 0.0f, -1.0f, 0.0f ));
  CHECK(v2 == vec4( 0.0f, 1.0f, 0.0f, 0.0f ));

  q.SetRotationY(-Dg::Constants<float>::PI / 2.0f);
  q.GetBasis(v0, v1, v2);
  CHECK(v0 == vec4( 0.0f, 0.0f, 1.0f, 0.0f ));
  CHECK(v1 == vec4( 0.0f, 1.0f, 0.0f, 0.0f ));
  CHECK(v2 == vec4( -1.0f, 0.0f, 0.0f, 0.0f ));

  q.SetRotationZ(-Dg::Constants<float>::PI / 2.0f);
  q.GetBasis(v0, v1, v2);
  CHECK(v0 == vec4( 0.0f, -1.0f, 0.0f, 0.0f ));
  CHECK(v1 == vec4( 1.0f, 0.0f, 0.0f, 0.0f ));
  CHECK(v2 == vec4( 0.0f, 0.0f, 1.0f, 0.0f ));
}


//-------------------------------------------------------------------------------
//		Operations
//-------------------------------------------------------------------------------
TEST(Stack_Quaternion_Operations, creation_Quaternion_Operations)
{
  quat q0, q1, q2, q3;

  float xr = 1.03f;
  float yr = -2.54f;
  float zr = 0.053f;

  q0.SetRotationX(xr);
  q1.SetRotationY(yr);
  q2.SetRotationZ(zr);

  q3 = q2 * q1 * q0;

  q1 = Dg::R3::Inverse(q3);
  q1 = q3;
  q1.Inverse();
  q2 = q1 * q3;

  CHECK(q2.IsIdentity());

  q1.Conjugate();
  q2 = q1;
  q2 = Dg::R3::Conjugate(q1);

  q1 = q1 + q0;
  q1 = q0 * q1;
  q1 = q1 - q0;
  q1 += q0;
  q1 -= q0;
  q1 *= q0;

  float r_dot = Dg::R3::Dot(q1, q0);

  vec4 v(1.0f, 0.0f, 0.0f, 0.0f);
  vec4 v0 = v;

  q0.SetRotationX(Dg::Constants<float>::PI * 0.5f);
  q1.SetRotationY(Dg::Constants<float>::PI * 0.5f);
  q2.SetRotationZ(Dg::Constants<float>::PI * 0.5f);

  q1.RotateSelf(v);
  CHECK(v == vec4(0.0f, 0.0f, -1.0f, 0.0f));

  q0.RotateSelf(v);
  CHECK(v == vec4(0.0f, 1.0f, 0.0f, 0.0f));

  q2.RotateSelf(v);
  CHECK(v == vec4(-1.0f, 0.0f, 0.0f, 0.0f));

  CHECK((q1 * q0 * q2).Rotate(v0) == vec4(-1.0f, 0.0f, 0.0f, 0.0f));

}

//-------------------------------------------------------------------------------
//		s/lerp
//-------------------------------------------------------------------------------
TEST(Stack_Quaternion_Slerp, creation_Quaternion_Slerp)
{
  quat q0, q1, q2, q3;

  float xr = 1.03f;
  float yr = -2.54f;
  float zr = 0.053f;

  q0.SetRotationX(xr);
  q1.SetRotationY(yr);
  q2.SetRotationZ(zr);

  q3 = q2 * q1 * q0;

  Dg::R3::Lerp(q2, q0, q1, 1.0f);
  CHECK(q2 == q1);

  Dg::R3::Slerp(q2, q0, q1, 1.0f);
  CHECK(q2 == q1);

  Dg::R3::ApproxSlerp(q2, q0, q1, 1.0f);
  CHECK(q2 == q1);
}


//-------------------------------------------------------------------------------
//		Get matrix
//-------------------------------------------------------------------------------
TEST(Stack_Quaternion_Matrix, creation_Quaternion_Matrix)
{
  mat44 m;
  quat q;
  vec4 v(0.5f, 0.5f, 0.5f, 0.0f);

  q.SetRotation(0.1f, 0.2f, 0.3f, Dg::EulerOrder::XYZ);
  m.Rotation(0.1f, 0.2f, 0.3f, Dg::EulerOrder::XYZ);

  CHECK(q.Rotate(v) == v * m);

  m.Rotation(q);
  CHECK(q.Rotate(v) == v * m);
}