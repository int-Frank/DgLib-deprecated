#include "TestHarness.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix44.h"

typedef Dg::Matrix44< float >     mat44;
typedef Dg::Quaternion < float >  quat;
typedef Dg::Vector4 < float >     vec4;

//--------------------------------------------------------------------------------
//	Matrix44 Construction
//--------------------------------------------------------------------------------
TEST(Stack_Matrix44_Construction, creation_Matrix44_Construction)
{

}

TEST(Stack4, creation4)
{
  ////-------------------------------------------------------------------------------
  ////		Element manipulation
  ////-------------------------------------------------------------------------------

  //mat44 m0;
  //CHECK(m0(0, 0) == 1.0f);
  //CHECK(m0(1, 0) == 0.0f);
  //CHECK(m0(2, 0) == 0.0f);
  //CHECK(m0(3, 0) == 0.0f);
  //CHECK(m0(0, 1) == 0.0f);
  //CHECK(m0(1, 1) == 1.0f);
  //CHECK(m0(2, 1) == 0.0f);
  //CHECK(m0(3, 1) == 0.0f);
  //CHECK(m0(0, 2) == 0.0f);
  //CHECK(m0(1, 2) == 0.0f);
  //CHECK(m0(2, 2) == 1.0f);
  //CHECK(m0(3, 2) == 0.0f);
  //CHECK(m0(0, 3) == 0.0f);
  //CHECK(m0(1, 3) == 0.0f);
  //CHECK(m0(2, 3) == 0.0f);
  //CHECK(m0(3, 3) == 1.0f);

  //m0.Rotation(1.0f, -0.23f, 0.34f);
  //
  //mat44  m1(m0);
  //CHECK(m1 == m0);

  //m1 = m0;
  //CHECK(m1 == m0);
  //CHECK(!(m1 != m0));

  //CHECK(!m1.IsZero());
  //CHECK(!m1.IsIdentity());

  //vec4 v0(1.0f, 1.25f, 1.5f, 1.75f);
  //vec4 v1(2.0f, 2.25f, 2.5f, 2.75f);
  //vec4 v2(3.0f, 3.25f, 3.5f, 3.75f);
  //vec4 v3(4.0f, 4.25f, 4.5f, 4.75f);

  //vec4 v4(1.0f, 2.0f, 3.0f, 4.0f);
  //vec4 v5(1.25f, 2.25f, 3.25f, 4.25f);
  //vec4 v6(1.5f, 2.5f, 3.5f, 4.5f);
  //vec4 v7(1.75f, 2.75f, 3.75f, 4.75f);

  //m0.SetRow(0, v0);
  //m0.SetRow(1, v1);
  //m0.SetRow(2, v2);
  //m0.SetRow(3, v3);

  //m1.SetRows(v0, v1, v2, v3);

  //CHECK(m0 == m1);

  //m0.SetColumn(0, v0);
  //m0.SetColumn(1, v1);
  //m0.SetColumn(2, v2);
  //m0.SetColumn(3, v3);

  //m1.SetColumns(v0, v1, v2, v3);

  //CHECK(m0 == m1);

  //m0.GetRows(v0, v1, v2, v3);
  //m0.GetColumns(v0, v1, v2, v3);

  //m0.Clean();
  //m0.Identity();

  ////-------------------------------------------------------------------------------
  ////		Affine transformations
  ////-------------------------------------------------------------------------------

  //m0.Rotation(1.0f, 2.0f, 3.0f);
  //float rx = 0.0f, ry = 0.0f, rz = 0.0f;
  //m0.GetFixedAngles(rx, ry, rz);

  //m0.Transpose();
  //m1 = Transpose(m0);

  //mat44 mi0;
  //mat44 mi1;

  //mi1 = AffineInverse(mi0);
  //CHECK(mi0 == mi1);

  //vec4 trans(12.4f, 89.2f, -953.345f, 0.0f);
  //vec4 scale(0.5f, 0.8f, 1.2f, 0.0f);
  //vec4 axis(trans);
  //axis.Normalize();

  //mat44 mt, mr, ms;
  //mt.Translation(trans);
  //mr.Rotation(axis, 1.98f);
  //ms.Scaling(scale);

  //m0 = mt * mr * ms;
  //m1 = AffineInverse(m0);
  //mat44 m2 = AffineInverse(m1);

  //CHECK(m0 == m2);

  ////-------------------------------------------------------------------------------
  ////		Operations
  ////-------------------------------------------------------------------------------

  //m0.Identity();
  //m1.Identity();
  //m2.Identity();
  //mat44 m3;
  //CHECK(m2 == (m0 * m1));

  //float val0[16] = {1.0f, 2.0f, 3.0f, 4.0f,
  //                  5.0f, 6.0f, 7.0f, 8.0f, 
  //                  9.0f, 10.0f, 11.0f, 12.0f,
  //                  13.0f, 14.0f, 15.0f, 16.0f };

  //float val1[16] = {1.0f, -2.0f, 3.0f, -4.0f,
  //                  -5.0f, 6.0f, -7.0f, 8.0f, 
  //                  9.0f, -10.0f, 11.0f, -12.0f,
  //                  -13.0f, 14.0f, 15.0f, 16.0f };

  //float res_mul[16] = {-34.0f, 36.0f, 82.0f, 40.0f,
  //                  -66.0f, 68.0f, 170.0f, 72.0f, 
  //                  -98.0f, 100.0f, 258.0f, 104.0f,
  //                  -130.0f, 132.0f, 346.0f, 136.0f };

  //float res_add[16] = {2.0f, 0.0f, 6.0f, 0.0f,
  //                  0.0f, 12.0f, 0.0f, 16.0f, 
  //                  18.0f, 0.0f, 22.0f, 0.0f,
  //                  0.0f, 28.0f, 30.0f, 32.0f };

  //float res_sub[16] = {0.0f, 4.0f, 0.0f, 8.0f,
  //                  10.0f, 0.0f, 14.0f, 0.0f, 
  //                  0.0f, 20.0f, 0.0f, 24.0f,
  //                  26.0f, 0.0f, 0.0f, 0.0f };
  //
  //float res_scmul[16] = {2.0f, 4.0f, 6.0f, 8.0f,
  //                  10.0f, 12.0f, 14.0f, 16.0f, 
  //                  18.0f, 20.0f, 22.0f, 24.0f,
  //                  26.0f, 28.0f, 30.0f, 32.0f };

  //float scalar = 2.0f;

  //m0.Set(val0);
  //m1.Set(val1);
  //m2.Set(res_mul);
  //CHECK(m2 == (m0 * m1));

  //m3 = m0;
  //m3 *= m1;
  //CHECK(m3 == m2);

  //m2.Set(res_add);
  //CHECK(m2 == (m0 + m1));

  //m3 = m0;
  //m3 += m1;
  //CHECK(m3 == m2);

  //m2.Set(res_sub);
  //CHECK(m2 == (m0 - m1));

  //m3 = m0;
  //m3 -= m1;
  //CHECK(m3 == m2);

  //m2.Set(res_scmul);
  //CHECK(m2 == (m0 * scalar));
  //CHECK(m2 == (scalar * m0));

  ////-------------------------------------------------------------------------------
  ////		Rotations
  ////-------------------------------------------------------------------------------

  //m0.Rotation(rz, 0.0f, 0.0f);
  //m1.RotationZ(rz);
  //CHECK(m0 == m1);

  //m0.Rotation(0.0f, ry, 0.0f);
  //m1.RotationY(ry);
  //CHECK(m0 == m1);

  //m0.Rotation(0.0f, 0.0f, rx);
  //m1.RotationX(rx);
  //CHECK(m0 == m1);

  //m0.Rotation(rz, ry, rx);
  //mat44 mrx, mry, mrz;
  //mrx.RotationX(rx);
  //mry.RotationY(ry);
  //mrz.RotationZ(rz);

  //CHECK(m0 == (mrz * mry * mrx));

  //axis = vec4(-2.34f, 0.56f, 1.83f, 0.0f);
  //axis.Normalize();
  //float angle = 1.89f;

  //vec4 axis0;
  //float angle0;
  //m0.Rotation(axis, angle);
  //m0.GetAxisAngle(axis0, angle0);
  //CHECK(axis == axis0);
  //CHECK(Dg::AreEqual(angle, angle0));

  //quat q(axis, angle);
  //m1.Rotation(q);
  //CHECK(m1 == m0);

}