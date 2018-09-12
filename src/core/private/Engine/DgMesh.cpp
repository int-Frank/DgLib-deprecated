
#include "DgMesh.h"

namespace Dg
{
  namespace MeshTools
  {
    eHandle GetEdgeHandle(vHandle a_v0, vHandle a_v1)
    {
      impl::eHandleType v0(a_v0);
      impl::eHandleType v1(a_v1);
      if (v0 > v1)
      {
        std::swap(v0, v1);
      }
      return eHandle((v1 << (sizeof(impl::vHandleType) * CHAR_BIT)) | v0);
    }

    bool HasVertex(eHandle a_eh, vHandle a_vh)
    {
      return Source(a_eh) == a_vh || Target(a_eh) == a_vh;
    }

    vHandle Source(eHandle a_h)
    {
      return vHandle(impl::vHandleType(impl::eHandleType(a_h)));
    }

    vHandle Target(eHandle a_h)
    {
      return vHandle(impl::vHandleType(impl::eHandleType(a_h) >> (sizeof(impl::eHandleType) * CHAR_BIT / 2)));
    }
  }
}