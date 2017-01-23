
#include <limits>

#include "DgR3Vector_ancillary.h"
#include "MFS_Ray.h"
#include "query/DgR3QueryRayTriangle.h"

MFS_Ray::MFS_Ray(Mesh const & a_mesh, vec4 const & a_source)
  : m_mesh(a_mesh)
  , m_source(a_source)
  , m_useCone(false)
  , m_angle(6.2832f)
{
  sphere sph(m_mesh.Sphere());

  float sqDist = m_source.SquaredDistance(sph.Center());
  if (sqDist > sph.Radius())
  {
    m_angle = atan(sph.Radius() / sqrt(sqDist));
    m_axis = (sph.Center() - m_source);
    m_axis.Normalize();
    m_useCone = true;
  }
}


Dg::fHandle MFS_Ray::operator()()
{
  for (int i = 0; i < s_maxAttempts; ++i)
  {
    Dg::fHandle result = AttemptOnce();
    if (result.IsValid())
    {
      return result;
    }
  }
  return Dg::fHandle();
}


Dg::fHandle MFS_Ray::AttemptOnce()
{
  vec4 direction;
  if (m_useCone)
  {
    direction = Dg::R3::GetRandomVector(m_axis, m_angle);
  }
  else
  {
    direction = Dg::R3::GetRandomVector<float>();
  }

  Dg::fHandle handle;
  float uCur = std::numeric_limits<float>::max();
  for (auto fit = m_mesh.FacesBegin();
       fit != m_mesh.FacesEnd();
       ++fit)
  {
    Dg::R3::TIRayTriangle<float>          query;
    Dg::R3::TIRayTriangle<float>::Result  qResult;
    qResult = query(ray(m_source, m_axis), m_mesh.Triangle(*fit));
    if (qResult.isIntersecting && qResult.u < uCur)
    {
      handle = *fit;
      uCur = qResult.u;
    }
  }
  return handle;
}