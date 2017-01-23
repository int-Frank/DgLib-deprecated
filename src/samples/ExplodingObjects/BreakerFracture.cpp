#include "BreakerFracture.h"


BreakerFracture::BreakerFracture()
  : m_source(vec4::Origin())
  , m_minFaces(16)
  , m_maxFragments(64)
{

}

std::list<Mesh> BreakerFracture::operator()(Mesh const & a_mesh)
{
  std::list<Mesh> fragments;
  fragments.push_back(a_mesh);

  while (StillActive(fragments))
  {
    MeshVertex mv = GetNextOrigin(fragments);
    std::vector<Dg::vHandle> fractureTrace = GetFracture(*(mv.second), mv.first);
    std::pair<Mesh, Mesh> resultingFragments = SplitMesh(*(mv.second), fractureTrace);

    fragments.erase(mv.second);
    fragments.push_back(resultingFragments.first);
    fragments.push_back(resultingFragments.second);
  }
}