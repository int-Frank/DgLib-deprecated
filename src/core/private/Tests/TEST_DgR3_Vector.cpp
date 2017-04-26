#include "TestHarness.h"
#include "DgR3Vector.h"
#include "DgR3Vector_ancillary.h"

typedef Dg::R3::Vector < float > vec3;

//--------------------------------------------------------------------------------
//	Vector Construction
//--------------------------------------------------------------------------------
TEST(Stack_Vector4_Construction, creation_Vector4_Construction)
{
  vec3 v0({ 1.0f, 2.0f, -4.0f, 0.0f });
  vec3 v1(v0);

  CHECK(v0.x() == 1.0f);
  CHECK(v0.y() == 2.0f);
  CHECK(v0.z() == -4.0f);
  CHECK(v0.w() == 0.0f);

  CHECK(v1 == v0);
  CHECK(!(v1 != v0));
  CHECK(!v0.IsZero());
  CHECK(!v0.IsUnit());

  vec3 p1;
  p1.Set( 2.0f, 12.4f, -8.12f, 1.0 );
  p1.Clean();
  v0.Zero();
  v0 = v1;
  v0.Normalize();

  CHECK(v0.IsUnit());

  //Statics
  vec3 vec = vec3::Origin();
  CHECK(vec[0] == 0.0f);
  CHECK(vec[1] == 0.0f);
  CHECK(vec[2] == 0.0f);
  CHECK(vec[3] == 1.0f);

  vec = vec3::ZeroVector();
  CHECK(vec[0] == 0.0f);
  CHECK(vec[1] == 0.0f);
  CHECK(vec[2] == 0.0f);
  CHECK(vec[3] == 0.0f);

  vec = vec3::xAxis();
  CHECK(vec[0] == 1.0f);
  CHECK(vec[1] == 0.0f);
  CHECK(vec[2] == 0.0f);
  CHECK(vec[3] == 0.0f);

  vec = vec3::yAxis();
  CHECK(vec[0] == 0.0f);
  CHECK(vec[1] == 1.0f);
  CHECK(vec[2] == 0.0f);
  CHECK(vec[3] == 0.0f);

  vec = vec3::zAxis();
  CHECK(vec[0] == 0.0f);
  CHECK(vec[1] == 0.0f);
  CHECK(vec[2] == 1.0f);
  CHECK(vec[3] == 0.0f);

}


//--------------------------------------------------------------------------------
//	Vector Accessors
//--------------------------------------------------------------------------------
TEST(Stack_Vector4_Accessors, creation_Vector4_Accessors)
{
  float x = -1.5f;
  float y = 4.2f;
  float z = 9.0f;
  float w = 2.3f;

  vec3 v( x, y, z, w );

  CHECK(v[0] == x);
  CHECK(v[1] == y);
  CHECK(v[2] == z);
  CHECK(v[3] == w);
}


//--------------------------------------------------------------------------------
//	Vector Arithmetic
//--------------------------------------------------------------------------------
TEST(Stack_Vector4_Arithmetic, creation_Vector4_Arithmetic)
{
  vec3 v0( 1.6431f, 2.9012345f, -4.0045f, 0.0f );
  vec3 v1( -0.032456f, 23.45809f, 13.375689f, 0.0f );

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

  CHECK(v2 == vec3( -1.15278351f, 71.7770462f, 44.5462036f, 0.0f ));
}


//--------------------------------------------------------------------------------
//	Vector Other functions
//--------------------------------------------------------------------------------
TEST(Stack_Vector4_Other, creation_Vector4_Other)
{
  // Dot ///////////////////////////////////////////////

  float r_Dot = vec3( 2.0f, 0.0f, 3.0f, 0.0f ).Dot(vec3( 0.0f, 3.2f, 0.0f, -3.5f ));
  CHECK(r_Dot == 0.0f);

  // Cross /////////////////////////////////////////////

  vec3 c0( 1.0f, 1.0f, 1.0f, 0.0f );
  c0.Normalize();
  vec3 c1(c0.Perpendicular());

  vec3 c2 = c0.Cross(c1);

  CHECK(Dg::IsZero(Dot(c0, c1)) &&
    Dg::IsZero(c0.Dot(c2)) &&
    Dg::IsZero(c1.Dot(c2)));

  // Length ///////////////////////////////////////////

  vec3 v( 12.34f, 89.002345f, -41.3785f, 0.0f );

  float length = v.Length();
  float lengthSquared = v.LengthSquared();

  CHECK(length == 98.9235687f);
  CHECK(lengthSquared == 9785.87305f);

  // Random vectors ///////////////////////////////////

  v = Dg::R3::GetRandomVector<float>();
  vec3 vs = Dg::R3::GetRandomOrthonormalVector<float>(v);

  CHECK(Dg::IsZero(v.Dot(vs)));

  v = Dg::R3::GetRandomVector<float>(v, 10.3f);

  // Basis ///////////////////////////////////////////////

  vec3 v0, v1, v2;

  Dg::R3::GetBasis(vec3( 0.0f, 12.0f, 0.0f, 0.0f ),
    vec3(0.0f, 0.0f, 12.3f, 0.0f),
    v0, v1, v2);
  CHECK(v0 == vec3( 0.0f, 1.0f, 0.0f, 0.0f ));
  CHECK(v1 == vec3( 0.0f, 0.0f, 1.0f, 0.0f ));
  CHECK(v2 == vec3( 1.0f, 0.0f, 0.0f, 0.0f ));

  Dg::R3::GetBasis(vec3(0.0f, 0.0f, 0.0f, 0.0f),
               vec3(0.0f, 0.0f, 0.0f, 0.0f),
               v0, v1, v2);
  CHECK(v0 == vec3(1.0f, 0.0f, 0.0f, 0.0f));
  CHECK(v1 == vec3(0.0f, 1.0f, 0.0f, 0.0f));
  CHECK(v2 == vec3(0.0f, 0.0f, 1.0f, 0.0f));
}