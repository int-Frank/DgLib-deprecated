#include "TestHarness.h"
#include "Vector4.h"
#include "Quaternion.h"

typedef Dg::Quaternion < float >  quat;
typedef Dg::Vector4 < float >     vec4;

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

  vec4 v1(1.0f, -1.0f, 1.0f, 0.0f);
  vec4 v2(-1.0f, -1.0f, 1.0f, 0.0f);
  v1.Normalize();
  v2.Normalize();

  quat q2(v1, Dg::PI_f * 0.5f);
  quat q3(v1, v2);
  quat q4(v1);

  CHECK((q2[0] == 0.707106769f) && (q2[1] == 0.408248305f) && (q2[2] == -0.408248305f) && (q2[3] == 0.408248305f));
  CHECK((q3[0] == 0.816496551f) && (q3[1] == 0.0f) && (q3[2] == -0.408248276f) && (q3[3] == -0.408248276f));
  CHECK((q4[0] == 0.0f) && (q4[1] == 0.577350259f) && (q4[2] == -0.577350259f) && (q4[3] == 0.577350259f));

  q0 = q2;
  q1 = quat(q2);

  CHECK((q0[0] == 0.707106769f) && (q0[1] == 0.408248305f) && (q0[2] == -0.408248305f) && (q0[3] == 0.408248305f));
  CHECK((q1[0] == 0.707106769f) && (q1[1] == 0.408248305f) && (q1[2] == -0.408248305f) && (q1[3] == 0.408248305f));
}

//-------------------------------------------------------------------------------
//		Rotation
//-------------------------------------------------------------------------------
TEST(Stack_Quaternion_Rotation, creation_Quaternion_Rotation)
{
  quat q0, q1, q2;

  float xr = -1.2f;
  float yr = 0.843f;
  float zr = 1.45f;

  vec4 axis;
  float angle;

  q0.Set(xr, yr, zr);
  q0.Clean();

  q0.GetAxisAngle(axis, angle);
  q1.Set(axis, angle);
  CHECK(q0 == q1);
}


//-------------------------------------------------------------------------------
//		Operations
//-------------------------------------------------------------------------------
TEST(Stack_Quaternion_Operations, creation_Quaternion_Operations)
{
  quat q0, q1;
  q0.Set(1.0f, -2.0f, 3.0f);

  q1 = Dg::Inverse(q0);
  CHECK((q1[0] == 0.482423782f) && (q1[1] == 0.794853806f) && (q1[2] == 0.343733639f) && (q1[3] == -0.131613404f));

  q1 = q0;
  q1.Inverse();
  CHECK((q1[0] == 0.482423782f) && (q1[1] == 0.794853806f) && (q1[2] == 0.343733639f) && (q1[3] == -0.131613404f));

  quat q2 = q1 * q0;

  CHECK(q2.IsIdentity());

  q1.Conjugate();
  q2 = q1;
  CHECK((q2[0] == 0.482423782f) && (q2[1] == -0.794853806f) && (q2[2] == -0.343733639f) && (q2[3] == 0.131613404f));

  q2 = Dg::Conjugate(q1);
  CHECK((q2[0] == 0.482423782f) && (q2[1] == 0.794853806f) && (q2[2] == 0.343733639f) && (q2[3] == -0.131613404f));

  q0.Set(1.0f, -2.0f, 3.0f);
  q1.Set(1.2f, -2.3f, 3.1f);
  q1 = q1 + q0;
  q1 = q0 * q1;
  q1 = q1 - q0;
  q1 += q0;
  q1 -= q0;
  q1 *= q0;

  CHECK((q1[0] == -1.43647385f) && (q1[1] == 0.729969800f) && (q1[2] == 0.397171229f) && (q1[3] == -0.344405800f));

  float r_dot = Dg::Dot(q1, q0);

  vec4 v(1.0f, 0.0f, 0.0f, 0.0f);
  q0.Set(0.0f, 0.0f, Dg::PI_f * 0.5f);

  vec4 result = q0.Rotate(v);
  CHECK(result == vec4(0.0f, 1.0f, 0.0f, 0.0f));

  q0.RotateSelf(v);
  CHECK(v == vec4(0.0f, 1.0f, 0.0f, 0.0f));
}


//-------------------------------------------------------------------------------
//		s/lerp
//-------------------------------------------------------------------------------
TEST(Stack_Quaternion_Slerp, creation_Quaternion_Slerp)
{
  quat q0, q1, q2;

  q0.Set(1.2f, -5.3f, 0.1f);
  q1.Set(-0.534f, 2.56f, -1.895f);

  Dg::Lerp(q2, q0, q1, 1.0f);
  CHECK(q2 == q1);

  Dg::Slerp(q2, q0, q1, 1.0f);
  CHECK(q2 == q1);

  Dg::ApproxSlerp(q2, q0, q1, 1.0f);
  CHECK(q2 == q1);
}