#include "TestHarness.h"
#include "DgVector.h"
#include "Quaternion.h"
#include "Matrix44.h"

typedef Dg::Matrix44< float >     mat44;
typedef Dg::Quaternion < float >  quat;
typedef Dg::Vector < 4, float >     vec4;

//--------------------------------------------------------------------------------
//	Matrix44 Construction
//--------------------------------------------------------------------------------
TEST(Stack_Matrix44_Construction, creation_Matrix44_Construction)
{
  mat44 m0, m2;
  CHECK(m0.IsIdentity());

  float data[16] = {};
  m0.Set(data);
  CHECK(m0.IsZero());

  m0.Rotation(1.0f, -0.23f, 0.34f, Dg::EulerOrder::XZX);
  
  mat44  m1(m0);
  CHECK(m1 == m0);

  m1 = m0;
  CHECK(m1 == m0);
  CHECK(!(m1 != m0));

  CHECK(!m1.IsZero());
  CHECK(!m1.IsIdentity());

  vec4 v0({ 1.0f, 1.25f, 1.5f, 1.75f });
  vec4 v1({ 2.0f, 2.25f, 2.5f, 2.75f });
  vec4 v2({ 3.0f, 3.25f, 3.5f, 3.75f });
  vec4 v3({ 4.0f, 4.25f, 4.5f, 4.75f });

  vec4 v4({ 1.0f, 2.0f, 3.0f, 4.0f });
  vec4 v5({ 1.25f, 2.25f, 3.25f, 4.25f });
  vec4 v6({ 1.5f, 2.5f, 3.5f, 4.5f });
  vec4 v7({ 1.75f, 2.75f, 3.75f, 4.75f });

  m0.SetRow(0, v0);
  m0.SetRow(1, v1);
  m0.SetRow(2, v2);
  m0.SetRow(3, v3);

  m1.SetRows(v0, v1, v2, v3);

  CHECK(m0 == m1);

  m0.SetColumn(0, Transpose(v0));
  m0.SetColumn(1, Transpose(v1));
  m0.SetColumn(2, Transpose(v2));
  m0.SetColumn(3, Transpose(v3));

  m1.SetColumns(Transpose(v0), Transpose(v1), Transpose(v2), Transpose(v3));

  CHECK(m0 == m1);

  m0.GetRows(v0, v1, v2, v3);

  m0.Clean();
  m0.Identity();
}


//--------------------------------------------------------------------------------
//	Matrix44 Inverse
//--------------------------------------------------------------------------------
TEST(Stack_Matrix44_Inverse, creation_Matrix44_Inverse)
{
  mat44 m0, m1, m2, ms, mr, mt;
  ms.Scaling(vec4({ 1.34f, 0.39f, 14.3f, 0.0f }));
  mr.Rotation(0.234f, -1.49f, 2.457f, Dg::EulerOrder::ZXY);
  mt.Translation(vec4({ 2.3f, 53.24f, -12.9f, 0.0f }));

  m0 = mt;
  m1 = AffineInverse(m0);
  m2 = AffineInverse(m1);
  CHECK(m2 == m0);

  m0 = mr;
  m1 = AffineInverse(m0);
  m2 = AffineInverse(m1);
  CHECK(m2 == m0);

  m0 = ms;
  m1 = AffineInverse(m0);
  m2 = AffineInverse(m1);
  CHECK(m2 == m0);

  m0 = mt * mr * ms;
  m1 = AffineInverse(m0);
  m2 = AffineInverse(m1);
  CHECK(m2 == m0);
}

//--------------------------------------------------------------------------------
//	Matrix44 Operations
//--------------------------------------------------------------------------------
TEST(Stack_Matrix44_Operations, creation_Matrix44_Operations)
{
  mat44 m0, m1, m2, m3;
  CHECK(m2 == (m0 * m1));

  float val0[16] = {1.0f, 2.0f, 3.0f, 4.0f,
                    5.0f, 6.0f, 7.0f, 8.0f, 
                    9.0f, 10.0f, 11.0f, 12.0f,
                    13.0f, 14.0f, 15.0f, 16.0f };

  float val1[16] = {1.0f, -2.0f, 3.0f, -4.0f,
                    -5.0f, 6.0f, -7.0f, 8.0f, 
                    9.0f, -10.0f, 11.0f, -12.0f,
                    -13.0f, 14.0f, 15.0f, 16.0f };

  float res_mul[16] = {-34.0f, 36.0f, 82.0f, 40.0f,
                    -66.0f, 68.0f, 170.0f, 72.0f, 
                    -98.0f, 100.0f, 258.0f, 104.0f,
                    -130.0f, 132.0f, 346.0f, 136.0f };

  float res_add[16] = {2.0f, 0.0f, 6.0f, 0.0f,
                    0.0f, 12.0f, 0.0f, 16.0f, 
                    18.0f, 0.0f, 22.0f, 0.0f,
                    0.0f, 28.0f, 30.0f, 32.0f };

  float res_sub[16] = {0.0f, 4.0f, 0.0f, 8.0f,
                    10.0f, 0.0f, 14.0f, 0.0f, 
                    0.0f, 20.0f, 0.0f, 24.0f,
                    26.0f, 0.0f, 0.0f, 0.0f };
  
  float res_scmul[16] = {2.0f, 4.0f, 6.0f, 8.0f,
                    10.0f, 12.0f, 14.0f, 16.0f, 
                    18.0f, 20.0f, 22.0f, 24.0f,
                    26.0f, 28.0f, 30.0f, 32.0f };

  float scalar = 2.0f;

  m0.Set(val0);
  m1.Set(val1);
  m2.Set(res_mul);
  CHECK(m2 == (m0 * m1));

  m3 = m0;
  m3 *= m1;
  CHECK(m3 == m2);

  m2.Set(res_add);
  CHECK(m2 == (m0 + m1));

  m3 = m0;
  m3 += m1;
  CHECK(m3 == m2);

  m2.Set(res_sub);
  CHECK(m2 == (m0 - m1));

  m3 = m0;
  m3 -= m1;
  CHECK(m3 == m2);

  m2.Set(res_scmul);
  CHECK(m2 == (m0 * scalar));
  CHECK(m2 == (scalar * m0));
}

//--------------------------------------------------------------------------------
//	Matrix44 VectorTransform
//--------------------------------------------------------------------------------
TEST(Stack_Matrix44_VectorTransform, creation_Matrix44_VectorTransform)
{
  mat44 m0, m1, m2, m3;
  m1.Identity();
  m2.Identity();
  m3.Identity();

  m0.Scaling(0.43f);
  m1 *= m0;
  m0.Rotation(0.32f, -2.84f, 1.29f, Dg::EulerOrder::ZXY);
  m1 *= m0;
  m0.Translation(vec4({ 12.4f, 9.3f, -6.39f, 0.0f }));
  m1 *= m0;

  m0.Scaling(1.43f);
  m2 *= m0;
  m0.Rotation(1.32f, 1.84f, -1.83f, Dg::EulerOrder::ZXY);
  m2 *= m0;
  m0.Translation(vec4({ 9.04f, -3.3f, 2.39f, 0.0f }));
  m2 *= m0;

  m0.Scaling(2.354f);
  m3 *= m0;
  m0.Rotation(0.034f, -1.23f, -1.89f, Dg::EulerOrder::ZXY);
  m3 *= m0;
  m0.Translation(vec4({ -1.3f, -5.3f, 1.39f, 0.0f }));
  m3 *= m0;

  vec4 v({ 1.34f, -18.834f, -9.38f, 1.0f });
  CHECK(v * m1 * m2 * m3 == v * (m1 * m2 * m3));
}

//--------------------------------------------------------------------------------
//	Matrix44 Rotation
//--------------------------------------------------------------------------------
TEST(Stack_Matrix44_Rotation, creation_Matrix44_Rotation)
{
  mat44 m0, m1, mx, my, mz, ms, mt;
  quat q0, q1;
  mx.RotationX(Dg::PI_f * 0.5f);
  my.RotationY(Dg::PI_f * 0.5f);
  mz.RotationZ(Dg::PI_f * 0.5f);
  ms.Scaling(2.0f);
  mt.Translation(vec4({ -10.0f, 0.0f, 0.0f, 0.0f }));
  vec4 v({ 1.0f, 0.0f, 0.0f, 0.0f }), vr;

  v = v * my;
  CHECK(v == vec4({ 0.0f, 0.0f, -1.0f, 0.0f }));

  v = v * mx;
  CHECK(v == vec4({ 0.0f, 1.0f, 0.0f, 0.0f }));

  v = v * mz;
  CHECK(v == vec4({ -1.0f, 0.0f, 0.0f, 0.0f }));

  v.Set({ 1.0f, 0.0f, 0.0f, 1.0f });
  v = v * my * mx * mz * ms * mt;
  CHECK(v == vec4({ -12.0f, 0.0f, 0.0f, 1.0f }));

  float rx = 1.43f;
  float ry = 0.2901f;
  float rz = -0.934f;

  mx.RotationX(rx);
  my.RotationY(ry);
  mz.RotationZ(rz);

  v.Set({ 1.32f, -7.934f, -18.3896f, 0.0f });

  //Euler angles

  m0.Rotation(rx, ry, rz, Dg::EulerOrder::XYZ);
  CHECK(m0 == mx * my * mz);            //Check: Matrix concatenation
  q0.SetRotation(rx, ry, rz, Dg::EulerOrder::XYZ);
  vr = v * m0;
  CHECK(q0.Rotate(v) == v * m0);        //Check: Equivanent matrix/quaternion concatenation
  CHECK(vr == v * m0);
  m0.Rotation(q0);
  CHECK(q0.Rotate(v) == v * m0);        //Check: Set matrix fom quaternion
  CHECK(vr == v * m0);
  q0 = m0.GetQuaternion();
  m0.GetQuaternion(q1);
  CHECK(q0 == q1);
  CHECK(q0.Rotate(v) == v * m0);        //Check: Set quaternion from matrix
  CHECK(vr == v * m0);

  m0.Rotation(rx, ry, rz, Dg::EulerOrder::XZY);
  CHECK(m0 == mx * mz * my);
  q0.SetRotation(rx, ry, rz, Dg::EulerOrder::XZY);
  vr = v * m0;
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  m0.Rotation(q0);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  q0 = m0.GetQuaternion();
  m0.GetQuaternion(q1);
  CHECK(q0 == q1);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);

  m0.Rotation(rx, ry, rz, Dg::EulerOrder::YXZ);
  CHECK(m0 == my * mx * mz);
  q0.SetRotation(rx, ry, rz, Dg::EulerOrder::YXZ);
  vr = v * m0;
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  m0.Rotation(q0);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  q0 = m0.GetQuaternion();
  m0.GetQuaternion(q1);
  CHECK(q0 == q1);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);

  m0.Rotation(rx, ry, rz, Dg::EulerOrder::YZX);
  CHECK(m0 == my * mz * mx);
  q0.SetRotation(rx, ry, rz, Dg::EulerOrder::YZX);
  vr = v * m0;
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  m0.Rotation(q0);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  q0 = m0.GetQuaternion();
  m0.GetQuaternion(q1);
  CHECK(q0 == q1);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);

  m0.Rotation(rx, ry, rz, Dg::EulerOrder::ZYX);
  CHECK(m0 == mz * my * mx);
  q0.SetRotation(rx, ry, rz, Dg::EulerOrder::ZYX);
  vr = v * m0;
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  m0.Rotation(q0);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  q0 = m0.GetQuaternion();
  m0.GetQuaternion(q1);
  CHECK(q0 == q1);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);

  m0.Rotation(rx, ry, rz, Dg::EulerOrder::ZXY);
  CHECK(m0 == mz * mx * my);
  q0.SetRotation(rx, ry, rz, Dg::EulerOrder::ZXY);
  vr = v * m0;
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  m0.Rotation(q0);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  q0 = m0.GetQuaternion();
  m0.GetQuaternion(q1);
  CHECK(q0 == q1);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);

  m0.Rotation(rx, ry, rx, Dg::EulerOrder::XYX);
  CHECK(m0 == mx * my * mx);
  q0.SetRotation(rx, ry, rx, Dg::EulerOrder::XYX);
  vr = v * m0;
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  m0.Rotation(q0);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  q0 = m0.GetQuaternion();
  m0.GetQuaternion(q1);
  CHECK(q0 == q1);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);

  m0.Rotation(rx, rz, rx, Dg::EulerOrder::XZX);
  CHECK(m0 == mx * mz * mx);
  q0.SetRotation(rx, rz, rx, Dg::EulerOrder::XZX);
  vr = v * m0;
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  m0.Rotation(q0);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  q0 = m0.GetQuaternion();
  m0.GetQuaternion(q1);
  CHECK(q0 == q1);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  
  m0.Rotation(ry, rx, ry, Dg::EulerOrder::YXY);
  CHECK(m0 == my * mx * my);
  q0.SetRotation(ry, rx, ry, Dg::EulerOrder::YXY);
  vr = v * m0;
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  m0.Rotation(q0);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  q0 = m0.GetQuaternion();
  m0.GetQuaternion(q1);
  CHECK(q0 == q1);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);

  m0.Rotation(ry, rz, ry, Dg::EulerOrder::YZY);
  CHECK(m0 == my * mz * my);
  q0.SetRotation(ry, rz, ry, Dg::EulerOrder::YZY);
  vr = v * m0;
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  m0.Rotation(q0);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  q0 = m0.GetQuaternion();
  m0.GetQuaternion(q1);
  CHECK(q0 == q1);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);

  m0.Rotation(rz, rx, rz, Dg::EulerOrder::ZXZ);
  CHECK(m0 == mz * mx * mz);
  q0.SetRotation(rz, rx, rz, Dg::EulerOrder::ZXZ);
  vr = v * m0;
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  m0.Rotation(q0);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  q0 = m0.GetQuaternion();
  m0.GetQuaternion(q1);
  CHECK(q0 == q1);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);

  m0.Rotation(rz, ry, rz, Dg::EulerOrder::ZYZ);
  CHECK(m0 == mz * my * mz);
  q0.SetRotation(rz, ry, rz, Dg::EulerOrder::ZYZ);
  vr = v * m0;
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  m0.Rotation(q0);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);
  q0 = m0.GetQuaternion();
  m0.GetQuaternion(q1);
  CHECK(q0 == q1);
  CHECK(q0.Rotate(v) == v * m0);
  CHECK(vr == v * m0);

  //Axis-angle

  m0.RotationX(rx);
  m1.Rotation(vec4({ 1.0f, 0.0f, 0.0f, 0.0f }), rx);
  CHECK(m0 == m1);
  q0.Set(vec4({ 1.0f, 0.0f, 0.0f, 0.0f }), rx);
  CHECK(q0.Rotate(v) == v * m0);

  m0.RotationY(rx);
  m1.Rotation(vec4({ 0.0f, 1.0f, 0.0f, 0.0f }), rx);
  CHECK(m0 == m1);
  q0.Set(vec4({ 0.0f, 1.0f, 0.0f, 0.0f }), rx);
  CHECK(q0.Rotate(v) == v * m0);

  m0.RotationZ(rx);
  m1.Rotation(vec4({ 0.0f, 0.0f, 1.0f, 0.0f }), rx);
  CHECK(m0 == m1);
  q0.Set(vec4({ 0.0f, 0.0f, 1.0f, 0.0f }), rx);
  CHECK(q0.Rotate(v) == v * m0);

  vec4 axis({ 1.25f, 0.43f, -0.9345f, 0.0f });
  axis.Normalize();
  float angle = 0.9435f;

  m0.Rotation(axis, angle);
  q0.Set(axis, angle);

  CHECK(q0.Rotate(v) == v * m0);

}