
#include "DgR3Vector_ancillary.h"
#include "ExplosionTypes.h"

//----------------------------------------------------------------------------------
//  ExplosionTypeBase...
//----------------------------------------------------------------------------------

void ExplosionTypeBase::Init(vec4 const & a_source)
{
  m_source = a_source;
  PostInit();
}

TransformData ExplosionTypeBase::GetData(vec4 const & a_centroid)
{
  TransformData td;
  td.translation = vec4::ZeroVector();
  td.rotation = vec4::ZeroVector();
  return td;
}

//----------------------------------------------------------------------------------
//  ExplosionType_Complete...
//----------------------------------------------------------------------------------

TransformData ExplosionType_Complete::GetData(vec4 const & a_centroid)
{
  TransformData td;
  td.translation = a_centroid - m_source;
  td.rotation.Zero();

  if (td.translation.IsZero())
  {
    td.translation = Dg::R3::GetRandomVector<float>();
  }

  float mean = 2.f;
  float sd = 0.5;

  float vMod;
  do { vMod = Dg::RNG::GetNormal(mean, sd); } while (vMod < 0.f);

  float invLenSq = 1.f / td.translation.LengthSquared();
  td.translation *= (invLenSq * vMod);

  for (int i = 0; i < 3; ++i)
  {
    td.rotation[i] = Dg::RNG::GetNormal(6.f, 2.f);
  }
  return td;
}

//----------------------------------------------------------------------------------
//  ExplosionType_Chunked...
//----------------------------------------------------------------------------------

TransformData ExplosionType_Chunked::GetData(vec4 const & a_centroid)
{
  TransformData td;
  td.translation = a_centroid - m_source;
  td.rotation.Zero();
  if (td.translation.IsZero())
  {
    td.translation = Dg::R3::GetRandomVector<float>();
  }
  td.translation.Normalize();

  float cosTheta = -99.f;
  for (auto const & axis : m_axes)
  {
    float cosTheta_cur = axis.Dot(td.translation);
    if (cosTheta_cur > cosTheta)
    {
      cosTheta = cosTheta_cur;
    }
  }

  float cutoffAng = 30.f;
  float vChunk = 2.f;
  if (cosTheta < cos(cutoffAng * Dg::Constants<float>::PI / 180.f))
  {
    td.translation.Zero();
  }
  else
  {
    float mean = 6.f;
    float sd = 2.f;

    float vMod;
    do { vMod = Dg::RNG::GetNormal(mean, sd); } while (vMod < 0.f);
    td.translation *= vMod;

    for (int i = 0; i < 3; ++i)
    {
      td.rotation[i] = Dg::RNG::GetNormal(6.f, 2.f);
    }
  }

  return td;
}

void ExplosionType_Chunked::PostInit()
{
  int nAxes = 10;
  for (int i = 0; i < nAxes; ++i)
  {
    m_axes.push_back(Dg::R3::GetRandomVector<float>());
  }
}