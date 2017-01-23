#ifndef BREAKERSPLIT_H
#define BREAKERSPLIT_H

#include "BreakerBase.h"

typedef std::pair<Dg::vHandle, std::list<Mesh>::iterator> MeshVertex;

class BreakerFracture
{
public:

  BreakerFracture();

  void SetSource(vec4 const & a_source) { m_source = a_source; }
  void SetMinimumFacesPerFragment(size_t a_val) { m_minFaces = a_val; }
  void SetMaximumFragments(size_t a_val) { m_maxFragments = a_val; }
  void SetMaximumIterations(size_t a_val) { m_maxIterations = a_val; }

  std::list<Mesh> operator()(Mesh const &);

private:

  bool StillActive(std::list<Mesh> const &) const;
  MeshVertex GetNextOrigin(std::list<Mesh> const &) const;
  std::vector<Dg::vHandle> GetFracture(Mesh const &, Dg::vHandle) const;
  std::pair<Mesh, Mesh> SplitMesh(Mesh const &, std::vector<Dg::vHandle> const &) const;

private:

  vec4    m_source;
  size_t  m_minFaces;
  size_t  m_maxFragments;
  size_t  m_maxIterations;
};

#endif