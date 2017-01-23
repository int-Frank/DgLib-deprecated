#ifndef MFS_RAY_H
#define MFS_RAY_H

#include "Types.h"
#include "MeshFaceSelector.h"
#include "Mesh.h"

class MFS_Ray : public MeshFaceSelector
{
public:

  MFS_Ray(Mesh const &, vec4 const & source);

  Dg::fHandle operator()();

private:

  static int const s_maxAttempts = 128;

private:

  Dg::fHandle AttemptOnce();

private:

  Mesh const & m_mesh;

  bool         m_useCone;
  vec4         m_axis;
  vec4         m_source;
  float        m_angle;
};

#endif