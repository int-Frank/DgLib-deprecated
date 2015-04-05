#include "TestHarness.h"
#include "Vector3.h"

typedef Dg::Vector3 < float > vec3;

//--------------------------------------------------------------------------------
//	Vector3 Construction
//--------------------------------------------------------------------------------
TEST(Stack_Vector3_Construction, creation_Vector3_Construction)
{
  vec3 v0(1.0f, 2.0f, -4.0f);
  vec3 v1(v0);

  CHECK(v1 == v0);
  CHECK(!(v1 != v0));
  CHECK(!v0.IsZero());
  CHECK(!v0.IsUnit());

  vec3 p1;
  p1.Set(2.0f, 12.4f, -8.12f);
  p1.Clean();
  v0.Zero();
  v0 = v1;
  v0.Normalize();

  CHECK(v0.IsUnit());
}


//--------------------------------------------------------------------------------
//	Vector3 Accessors
//--------------------------------------------------------------------------------
TEST(Stack_Vector3_Accessors, creation_Vector3_Accessors)
{
  float x = -1.5f;
  float y = 4.2f;
  float z = 9.0f;

  vec3 v(x, y, z);

  CHECK((v.x() == x) && (v[0] == x));
  CHECK((v.y() == y) && (v[1] == y));
  CHECK((v.z() == z) && (v[2] == z));
}


//--------------------------------------------------------------------------------
//	Vector3 Arithmetic
//--------------------------------------------------------------------------------
TEST(Stack_Vector3_Arithmetic, creation_Vector3_Arithmetic)
{
  vec3 v0(1.6431f, 2.9012345f, -4.0045f);
  vec3 v1(-0.032456f, 23.45809f, 13.375689f);

  vec3 v2 = v0 + v1;
  v2 = v0 - v2;
  v2 = v2 * 3.4f;
  v2 = 1.2f * v2;
  v2 = -v0 + v2;
  v2 = v2 / 2.3f;
  v2 += v0;
  v2 -= v1;
  v2 *= 4.3f;
  v2 /= -3.8f;

  CHECK(v2 == vec3(-1.15278351f, 71.7770462f, 44.5462036f));
}


//--------------------------------------------------------------------------------
//	Vector3 Other functions
//--------------------------------------------------------------------------------
TEST(Stack_Vector3_Other, creation_Vector3_Other)
{
  // Dot ///////////////////////////////////////////////

  float r_Dot = Dot(vec3(2.0f, 0.0f, 3.0f), vec3(0.0f, 3.2f, 0.0f));
  CHECK(r_Dot == 0.0f);

  // Cross /////////////////////////////////////////////

  vec3 c0(1.0f, 1.0f, 1.0f);
  c0.Normalize();
  vec3 c1(Perpendicular(c0));

  vec3 c2 = Cross(c0, c1);

  CHECK(Dg::IsZero(Dot(c0, c1)) &&
    Dg::IsZero(Dot(c0, c2)) &&
    Dg::IsZero(Dot(c1, c2)));

  CHECK(c2 == vec3(0.666666627f, - 0.333333313f, -0.333333313f));

  // Length ///////////////////////////////////////////

  vec3 v(12.34f, 89.002345f, -41.3785f);

  float length = v.Length();
  float lengthSquared = v.LengthSquared();

  CHECK(length == 98.9235687f);
  CHECK(lengthSquared == 9785.87305f);

  // Random vectors ///////////////////////////////////

  v = Dg::GetRandomVector<float>();
  vec3 vs = Dg::GetRandomOrthonormalVector<float>(v);

  CHECK(Dg::IsZero(Dot(v, vs)));

  v = Dg::GetRandomConeVector<float>(v, 10.3f);
}