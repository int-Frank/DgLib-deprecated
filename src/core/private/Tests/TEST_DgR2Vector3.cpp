#include "TestHarness.h"
#include "DgR2Vector3.h"
#include "DgR3Vector4.h"
#include "DgR2Vector3_ancillary.h"

typedef Dg::R2::Vector3 < float > vec3;
typedef Dg::R3::Vector4 < float > vec4;

//--------------------------------------------------------------------------------
//	Vector4 Construction
//--------------------------------------------------------------------------------
TEST(Stack_Vector3_Construction, creation_Vector3_Construction)
{
  vec3 v0(1.0f, -4.0f, 0.0f);
  vec3 v1(v0);

  CHECK(v1 == v0);
  CHECK(!(v1 != v0));
  CHECK(!v0.IsZero());
  CHECK(!v0.IsUnit());

  vec3 p1;
  p1.Set(2.0f, -8.12f, 1.0);
  p1.Clean();
  v0.Zero();
  v0 = v1;
  v0.Normalize();

  CHECK(v0.IsUnit());

  //Statics
  vec3 vec = vec3::Origin();
  CHECK(vec[0] == 0.0f);
  CHECK(vec[1] == 0.0f);
  CHECK(vec[2] == 1.0f);

  vec = vec3::ZeroVector();
  CHECK(vec[0] == 0.0f);
  CHECK(vec[1] == 0.0f);
  CHECK(vec[2] == 0.0f);

  vec = vec3::xAxis();
  CHECK(vec[0] == 1.0f);
  CHECK(vec[1] == 0.0f);
  CHECK(vec[2] == 0.0f);

  vec = vec3::yAxis();
  CHECK(vec[0] == 0.0f);
  CHECK(vec[1] == 1.0f);
  CHECK(vec[2] == 0.0f);

  vec4 v4(0.5f, 2.0f, 3.0f, 1.0f);
  vec3 v3(v4);
  CHECK(v3.x() == 0.5f);
  CHECK(v3.y() == 2.0f);
  CHECK(v3.w() == 1.0f);
}


//--------------------------------------------------------------------------------
//	Vector4 Accessors
//--------------------------------------------------------------------------------
TEST(Stack_Vector3_Accessors, creation_Vector3_Accessors)
{
  float x = -1.5f;
  float y = 4.2f;
  float w = 2.3f;

  vec3 v(x, y, w);

  CHECK(v[0] == x);
  CHECK(v[1] == y);
  CHECK(v[2] == w);
}


//--------------------------------------------------------------------------------
//	Vector4 Arithmetic
//--------------------------------------------------------------------------------
TEST(Stack_Vector3_Arithmetic, creation_Vector3_Arithmetic)
{
  vec3 v0(1.6431f, 2.9012345f, 0.0f);
  vec3 v1(-0.032456f, 23.45809f, 0.0f);

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

  CHECK(v2 == vec3(-1.15278351f, 71.7770462f, 0.0f));
}


//--------------------------------------------------------------------------------
//	Vector4 Other functions
//--------------------------------------------------------------------------------
TEST(Stack_Vector3_Other, creation_Vector3_Other)
{
  // Dot ///////////////////////////////////////////////

  float r_Dot = vec3(2.0f, 0.0f, 0.0f).Dot(vec3(0.0f, 3.2f, -3.5f));
  CHECK(r_Dot == 0.0f);

  // Cross /////////////////////////////////////////////

  vec3 c0(1.0f, 1.0f, 0.0f);
  c0.Normalize();
  vec3 c1(Perpendicular(c0));

  float c2 = PerpDot(c0, c1);

  CHECK(Dg::IsZero(Dot(c0, c1)) &&
        Dg::IsZero(1.0f - c2));

  // Length ///////////////////////////////////////////

  vec3 v(3.0f, 4.0f, 0.0f);

  float length = v.Length();
  float lengthSquared = v.LengthSquared();

  CHECK(length == 5.0f);
  CHECK(lengthSquared == 25.0f);

  // Random vectors ///////////////////////////////////

  v = Dg::R2::GetRandomVector<float>();
}