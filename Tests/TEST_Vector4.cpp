#include "TestHarness.h"
#include "Vector4.h"

typedef Dg::Vector4 < float > vec4;

//--------------------------------------------------------------------------------
//	Vector4 Construction
//--------------------------------------------------------------------------------
TEST(Stack_Vector4_Construction, creation_Vector4_Construction)
{
  vec4 v0({ 1.0f, 2.0f, -4.0f, 0.0f });
  vec4 v1(v0);

  CHECK(v1 == v0);
  CHECK(!(v1 != v0));
  CHECK(!v0.IsZero());
  CHECK(!v0.IsUnit());

  vec4 p1;
  p1.Set( 2.0f, 12.4f, -8.12f, 1.0 );
  p1.Clean();
  v0.Zero();
  v0 = v1;
  v0.Normalize();

  CHECK(v0.IsUnit());
}


//--------------------------------------------------------------------------------
//	Vector4 Accessors
//--------------------------------------------------------------------------------
TEST(Stack_Vector4_Accessors, creation_Vector4_Accessors)
{
  float x = -1.5f;
  float y = 4.2f;
  float z = 9.0f;
  float w = 2.3f;

  vec4 v( x, y, z, w );

  CHECK(v[0] == x);
  CHECK(v[1] == y);
  CHECK(v[2] == z);
  CHECK(v[3] == w);
}


//--------------------------------------------------------------------------------
//	Vector4 Arithmetic
//--------------------------------------------------------------------------------
TEST(Stack_Vector4_Arithmetic, creation_Vector4_Arithmetic)
{
  vec4 v0( 1.6431f, 2.9012345f, -4.0045f, 0.0f );
  vec4 v1( -0.032456f, 23.45809f, 13.375689f, 0.0f );

  vec4 v2 = v0 + v1;
  v2 = v0 - v2;
  v2 = v2 * 3.4f;
  v2 = 1.2f * v2;
  v2 = -v0 + v2;
  v2 = v2 / 2.3f;
  v2 += v0;
  v2 -= v1;
  v2 *= 4.3f;
  v2 /= -3.8f;

  CHECK(v2 == vec4( -1.15278351f, 71.7770462f, 44.5462036f, 0.0f ));
}


//--------------------------------------------------------------------------------
//	Vector4 Other functions
//--------------------------------------------------------------------------------
TEST(Stack_Vector4_Other, creation_Vector4_Other)
{
  // Dot ///////////////////////////////////////////////

  float r_Dot = Dg::Dot(vec4( 2.0f, 0.0f, 3.0f, 0.0f ), vec4( 0.0f, 3.2f, 0.0f, -3.5f ));
  CHECK(r_Dot == 0.0f);

  // Cross /////////////////////////////////////////////

  vec4 c0( 1.0f, 1.0f, 1.0f, 0.0f );
  c0.Normalize();
  vec4 c1(Perpendicular(c0));

  vec4 c2 = Cross(c0, c1);

  CHECK(Dg::IsZero(Dot(c0, c1)) &&
    Dg::IsZero(Dot(c0, c2)) &&
    Dg::IsZero(Dot(c1, c2)));

  // Length ///////////////////////////////////////////

  vec4 v( 12.34f, 89.002345f, -41.3785f, 0.0f );

  float length = v.Length();
  float lengthSquared = v.LengthSquared();

  CHECK(length == 98.9235687f);
  CHECK(lengthSquared == 9785.87305f);

  // Random vectors ///////////////////////////////////

  v = Dg::GetRandomVector<float>();
  vec4 vs = Dg::GetRandomOrthonormalVector<float>(v);

  CHECK(Dg::IsZero(Dot(v, vs)));

  v = Dg::GetRandomVector<float>(v, 10.3f);

  // Basis ///////////////////////////////////////////////

  vec4 v0, v1, v2;

  Dg::GetBasis(vec4( 0.0f, 12.0f, 0.0f, 0.0f ),
    vec4(0.0f, 0.0f, 12.3f, 0.0f),
    v0, v1, v2);
  CHECK(v0 == vec4( 0.0f, 1.0f, 0.0f, 0.0f ));
  CHECK(v1 == vec4( 0.0f, 0.0f, 1.0f, 0.0f ));
  CHECK(v2 == vec4( 1.0f, 0.0f, 0.0f, 0.0f ));

  Dg::GetBasis(vec4(0.0f, 0.0f, 0.0f, 0.0f), 
               vec4(0.0f, 0.0f, 0.0f, 0.0f),
               v0, v1, v2);
  CHECK(v0 == vec4(1.0f, 0.0f, 0.0f, 0.0f));
  CHECK(v1 == vec4(0.0f, 1.0f, 0.0f, 0.0f));
  CHECK(v2 == vec4(0.0f, 0.0f, 1.0f, 0.0f));
}